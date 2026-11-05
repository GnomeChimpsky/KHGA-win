#include <stdlib.h>

#define AL_ALEXT_PROTOTYPES
#include "AL/al.h"
#include "AL/alc.h"
#include "AL/alext.h"
#include "AL/efx.h"
#include "AL/efx-presets.h"
#include "xmp.h"

#include "NOpenALDrvPrivate.h"
#include "UnRender.h"
#ifdef KHG_CDMUSIC
#include "CDMusic.h"
#endif

/*-----------------------------------------------------------------------------
	Global implementation.
-----------------------------------------------------------------------------*/

IMPLEMENT_PACKAGE(NOpenALDrv);

#ifdef KHG_GALAXYREVERB
// [KHG] The effect sources live in the loopback context (the global current one); music and CD
// calls switch the calling thread to the real device's context for their scope.
struct FALThreadCtxScope
{
	ALCcontext* Prev;
	bool Changed;
	explicit FALThreadCtxScope( ALCcontext* C ) : Prev( alcGetThreadContext() ), Changed( C && C != Prev )
	{
		if( Changed )
			alcSetThreadContext( C );
	}
	~FALThreadCtxScope()
	{
		if( Changed )
			alcSetThreadContext( Prev );
	}
};
#define MUSIC_AL_SCOPE FALThreadCtxScope MusicALScope( Ctx )
#else
#define MUSIC_AL_SCOPE
#endif

#ifdef KHG_GALAXYREVERB
#define FX_FRAMES 256        // per streamed buffer: 5.8 ms at 44100 Hz
#endif
IMPLEMENT_CLASS(UNOpenALAudioSubsystem);

#if defined(PLATFORM_ANDROID) || defined(UNREAL_ANDROID) || defined(__ANDROID__)
static UNOpenALAudioSubsystem* GUE1AndroidOpenALAudio = NULL;
extern "C" void UE1AndroidSetAudioSuspended( int Suspended )
{
	if( GUE1AndroidOpenALAudio )
		GUE1AndroidOpenALAudio->AndroidSetSuspended( Suspended ? 1 : 0 );
}
#endif

/*-----------------------------------------------------------------------------
	UNOpenALAudioSubsystem implementation.
-----------------------------------------------------------------------------*/

void UNOpenALAudioSubsystem::InternalClassInitializer( UClass* Class )
{
	guardSlow(UNOpenALAudioSubsystem::InternalClassInitializer);
	new(Class, "DeviceName",         RF_Public)UStringProperty( CPP_PROPERTY( DeviceName         ), "Audio", CPF_Config, sizeof(DeviceName)-1 );
	new(Class, "OutputRate",         RF_Public)UIntProperty   ( CPP_PROPERTY( OutputRate         ), "Audio", CPF_Config );
	new(Class, "MusicVolume",        RF_Public)UByteProperty  ( CPP_PROPERTY( MusicVolume        ), "Audio", CPF_Config );
	new(Class, "SoundVolume",        RF_Public)UByteProperty  ( CPP_PROPERTY( SoundVolume        ), "Audio", CPF_Config );
	new(Class, "MasterVolume",       RF_Public)UByteProperty  ( CPP_PROPERTY( MasterVolume       ), "Audio", CPF_Config );
	new(Class, "AmbientFactor",      RF_Public)UFloatProperty ( CPP_PROPERTY( AmbientFactor      ), "Audio", CPF_Config );
	new(Class, "DopplerFactor",      RF_Public)UFloatProperty ( CPP_PROPERTY( DopplerFactor      ), "Audio", CPF_Config );
	new(Class, "UseReverb",          RF_Public)UBoolProperty  ( CPP_PROPERTY( UseReverb          ), "Audio", CPF_Config );
	new(Class, "UseHRTF",            RF_Public)UBoolProperty  ( CPP_PROPERTY( UseHRTF            ), "Audio", CPF_Config );
	new(Class, "MusicInterpolation", RF_Public)UByteProperty  ( CPP_PROPERTY( MusicInterpolation ), "Audio", CPF_Config );
	new(Class, "UseDigitalMusic",    RF_Public)UBoolProperty  ( CPP_PROPERTY( UseDigitalMusic    ), "Audio", CPF_Config );
#ifdef KHG_CDMUSIC
	new(Class, "UseCDMusic",         RF_Public)UBoolProperty  ( CPP_PROPERTY( UseCDMusic         ), "Audio", CPF_Config );
	new(Class, "CDMusicPath",        RF_Public)UStringProperty( CPP_PROPERTY( CDMusicPath        ), "Audio", CPF_Config, sizeof(CDMusicPath)-1 );
#endif
	unguardSlow;
}

UNOpenALAudioSubsystem::UNOpenALAudioSubsystem()
{
	OutputRate = DEFAULT_OUTPUT_RATE;
	MasterVolume = 255;
	SoundVolume = 127;
	MusicVolume = 63;
	AmbientFactor = 0.6f;
	DopplerFactor = 0.01f;
	UseHRTF = true;
	UseReverb = true;
	MusicInterpolation = XMP_INTERP_LINEAR;
	UseDigitalMusic = true;
#ifdef KHG_CDMUSIC
	UseCDMusic = true;
	appStrcpy( CDMusicPath, "..\\Music" );
#endif
}

UBOOL UNOpenALAudioSubsystem::Init()
{
	guard(UNOpenALAudioSubsystem::Init)

	Viewport = NULL;
	Device = NULL;
#if defined(PLATFORM_ANDROID) || defined(UNREAL_ANDROID) || defined(__ANDROID__)
	AndroidSuspended = false;
#endif
	if( DeviceName[0] )
		Device = alcOpenDevice( DeviceName );
	if( !Device )
		Device = alcOpenDevice( NULL );
	if( !Device )
	{
		debugf( NAME_Warning, "Could not open AL device: %04x", alcGetError( NULL ) );
		return false;
	}

	if( OutputRate <= 0 )
		OutputRate = DEFAULT_OUTPUT_RATE;
	
	if( DopplerFactor < 0.f )
		DopplerFactor = 0.f;

	AmbientFactor = Clamp( AmbientFactor, 0.f, 1.f );

	if( MusicInterpolation > XMP_INTERP_SPLINE )
		MusicInterpolation = XMP_INTERP_SPLINE;

	const ALint AttrList[] = {
		ALC_FREQUENCY, OutputRate,
		ALC_SOFT_HRTF, UseHRTF,
		0
	};

	Ctx = alcCreateContext( Device, AttrList );
	if( !Ctx )
	{
		debugf( NAME_Warning, "Could not create AL context: %04x", alcGetError( Device ) );
		alcCloseDevice( Device );
		Device = NULL;
		return false;
	}

	alcMakeContextCurrent( Ctx );
	// [KHG] Say which output device OpenAL Soft opened (on Windows its first choice is WASAPI).
	debugf( NAME_Init, "OpenAL: %s, device '%s', %d Hz", alGetString( AL_VERSION ), alcGetString( Device, ALC_DEVICE_SPECIFIER ), OutputRate );

	alListenerf( AL_GAIN, MasterVolume / 255.f );

	alGenSources( 1, &MusicSource	);
	alSourcei( MusicSource, AL_SOURCE_RELATIVE, AL_TRUE );
	alSource3f( MusicSource, AL_POSITION, 0.f, 0.f, 0.f );
	alSourcef( MusicSource, AL_ROLLOFF_FACTOR, 0.f );
	alSourcef( MusicSource, AL_GAIN, MusicVolume / 255.f );

	alGenBuffers( ARRAY_COUNT( MusicBuffers ), MusicBuffers );
	for( INT i = 0; i < ARRAY_COUNT( MusicBuffers ); ++i )
	{
		alBufferData( MusicBuffers[i], AL_FORMAT_STEREO16, MusicBufferData, sizeof( MusicBufferData ), OutputRate );
		FreeMusicBuffers[i] = MusicBuffers[i];
	}
	NumFreeMusicBuffers = NUM_MUSIC_BUFFERS;

#ifdef KHG_CDMUSIC
	alGenSources( 1, &CDSource );
	alSourcei( CDSource, AL_SOURCE_RELATIVE, AL_TRUE );
	alSource3f( CDSource, AL_POSITION, 0.f, 0.f, 0.f );
	alSourcef( CDSource, AL_ROLLOFF_FACTOR, 0.f );
	alSourcef( CDSource, AL_GAIN, MusicVolume / 255.f );
	alGenBuffers( NUM_MUSIC_BUFFERS, CDBuffers );
	for( INT i = 0; i < NUM_MUSIC_BUFFERS; ++i )
		FreeCDBuffers[i] = CDBuffers[i];
	NumFreeCDBuffers = NUM_MUSIC_BUFFERS;
	CDTrack = 255;
	debugf( NAME_Init, "CD music: %s, tracks from '%s'", UseCDMusic ? "on" : "off (UseCDMusic=False)", CDMusicPath );
#endif

	debugf( NAME_Init, "Module music: %s", UseDigitalMusic ? "on" : "off (UseDigitalMusic=False)" );

#ifdef KHG_GALAXYREVERB
	// Effects on a loopback device, so Galaxy's zone echo can be added to their mix.
	if( !InitFxLoopback() )
		debugf( NAME_Warning, "[KHG] Effects loopback unavailable; sound effects play without zone reverb" );
	EfxReverb = false;
#else
	EfxReverb = UseReverb;
#endif

	// Effect sources and listener (the loopback context when there is one).
	alDistanceModel( AL_LINEAR_DISTANCE_CLAMPED );
	alDopplerFactor( Max( 0.f, DopplerFactor ) );
	alListenerf( AL_METERS_PER_UNIT, DISTANCE_SCALE );
	alListenerf( AL_GAIN, MasterVolume / 255.f );
	alGenSources( MAX_SOURCES, Sources );

	if( EfxReverb )
	{
		alGenEffects( 1, &ReverbEffect );
		InitReverbEffect();
		alGenAuxiliaryEffectSlots( 1, &ReverbSlot );
		alAuxiliaryEffectSloti( ReverbSlot, AL_EFFECTSLOT_EFFECT, AL_EFFECT_NULL );
		ReverbOn = false;
	}

	for( INT i = 0; i < MAX_SOURCES; ++i )
		Voices[i].Buffer = INVALID_BUFFER;

	MusicCtx = xmp_create_context();
	xmp_set_player( MusicCtx, XMP_PLAYER_INTERP, MusicInterpolation );

	// Set ourselves up as the audio subsystem.
	USound::Audio = this;
	UMusic::Audio = this;

	// Spawn music streaming thread.
	StartMusicThread();

#if defined(PLATFORM_ANDROID) || defined(UNREAL_ANDROID) || defined(__ANDROID__)
	GUE1AndroidOpenALAudio = this;
#endif

	return true;

	unguard;
}

void UNOpenALAudioSubsystem::Destroy()
{
	guard(UNOpenALAudioSubsystem::Destroy)

#if defined(PLATFORM_ANDROID) || defined(UNREAL_ANDROID) || defined(__ANDROID__)
	if( GUE1AndroidOpenALAudio == this )
		GUE1AndroidOpenALAudio = NULL;
#endif

	StopMusicThread();
#ifdef KHG_CDMUSIC
	StopCDTrack();
#endif
#ifdef KHG_GALAXYREVERB
	if( FxThread )
	{
		FxThreadRunning = false;
		appThreadJoin( FxThread );
		FxThread = nullptr;
	}
#endif

	USound::Audio = NULL;
	UMusic::Audio = NULL;

	if( MusicCtx )
	{
		xmp_end_player( MusicCtx );
		xmp_free_context( MusicCtx );
		MusicCtx = NULL;
	}

	if (EfxReverb)
	{
		alDeleteAuxiliaryEffectSlots(1, &ReverbSlot);
		alDeleteEffects(1, &ReverbEffect);
	}

	if( Ctx )
	{
		// If we have a context, we probably have everything else. Kill it.
		SetViewport( NULL ); // This will also stop all sounds.
		if( Buffers.Num() )
			alDeleteBuffers( Buffers.Num(), &Buffers(0) );
		alDeleteSources( MAX_SOURCES, Sources );
		{
			MUSIC_AL_SCOPE;
			alSourceStop( MusicSource );
			alSourcei( MusicSource, AL_BUFFER, 0 );
			alDeleteBuffers( ARRAY_COUNT( MusicBuffers ), MusicBuffers );
			alDeleteSources( 1, &MusicSource );
#ifdef KHG_CDMUSIC
			alDeleteBuffers( NUM_MUSIC_BUFFERS, CDBuffers );
			alDeleteSources( 1, &CDSource );
			CDSource = INVALID_SOURCE;
#endif
		}
#ifdef KHG_GALAXYREVERB
		ShutdownFxLoopback();
#endif
		alcMakeContextCurrent( NULL );
		alcDestroyContext( Ctx );
		Ctx = NULL;
		Buffers.Empty();
	}

	if( Device )
	{
		// We might still have a device even if we haven't created a context.
		alcCloseDevice( Device );
		Device = NULL;
	}

	Super::Destroy();

	unguard;
}

void UNOpenALAudioSubsystem::ShutdownAfterError()
{
	guard(UNOpenALAudioSubsystem::Destroy)

#if defined(PLATFORM_ANDROID) || defined(UNREAL_ANDROID) || defined(__ANDROID__)
	if( GUE1AndroidOpenALAudio == this )
		GUE1AndroidOpenALAudio = NULL;
#endif

	StopMusicThread();
#ifdef KHG_GALAXYREVERB
	if( FxThread )
	{
		FxThreadRunning = false;
		appThreadJoin( FxThread );
		FxThread = nullptr;
	}
#endif

	USound::Audio = NULL;
	UMusic::Audio = NULL;

	// Shutdown contexts without touching anything else.
#ifdef KHG_CDMUSIC
	if( CDStream )
	{
		CDMusicClose( CDStream );
		CDStream = nullptr;
	}
#endif
	if( MusicCtx )
	{
		xmp_free_context( MusicCtx );
		MusicCtx = NULL;
		Music = NULL;
	}
	if (EfxReverb)
	{
		alDeleteAuxiliaryEffectSlots(1, &ReverbSlot);
		alDeleteEffects(1, &ReverbEffect);
	}
#ifdef KHG_GALAXYREVERB
	if( FxCtx )
	{
		alcSetThreadContext( NULL );
		alcMakeContextCurrent( Ctx );
		alcDestroyContext( FxCtx );
		FxCtx = nullptr;
	}
	if( FxDevice )
	{
		alcCloseDevice( FxDevice );
		FxDevice = nullptr;
	}
	if( GalaxyReverb )
	{
		GalaxyReverbDestroy( GalaxyReverb );
		GalaxyReverb = nullptr;
	}
#endif
	if( Ctx )
	{
		alcMakeContextCurrent( NULL );
		alcDestroyContext( Ctx );
		Ctx = NULL;
	}
	if( Device )
	{
		alcCloseDevice( Device );
		Device = NULL;
	}

	Super::ShutdownAfterError();

	unguard;
}

void UNOpenALAudioSubsystem::PostEditChange()
{
	guard(UNOpenALAudioSubsystem::PostEditChange)

	Super::PostEditChange();

	FScopedLock Lock( MusicMutex );

	if( DopplerFactor < 0.f )
		DopplerFactor = 0.f;
	AmbientFactor = Clamp( AmbientFactor, 0.f, 1.f );
	MusicInterpolation = Clamp( MusicInterpolation, (BYTE)0, (BYTE)XMP_INTERP_SPLINE );

	if( Ctx )
	{
		alListenerf( AL_GAIN, MasterVolume / 255.f );
		alDopplerFactor( DopplerFactor );
		MUSIC_AL_SCOPE;
		alListenerf( AL_GAIN, MasterVolume / 255.f );
		alSourcef( MusicSource, AL_GAIN, Max(MusicFade, 0.f) * MusicVolume / 255.f );
#ifdef KHG_CDMUSIC
		alSourcef( CDSource, AL_GAIN, MusicVolume / 255.f );
#endif
		// Voice volumes will be updated in Update().
	}

	if( MusicCtx )
	{
		xmp_set_player( MusicCtx, XMP_PLAYER_INTERP, MusicInterpolation );
	}

	unguard;
}

void UNOpenALAudioSubsystem::SetViewport( UViewport* InViewport )
{
	guard(UNOpenALAudioSubsystem::SetViewport)

	// Stop all sounds before viewport change.
	for( INT i = 0; i < MAX_SOURCES; ++i )
		StopVoice( i );

	// Stop and free music if the viewport has changed.
	if( InViewport != Viewport )
	{
		if( Music )
		{
			UnregisterMusic( Music );
			Music = NULL;
		}
#ifdef KHG_CDMUSIC
		// Galaxy stops the CD track here too.
		StopCDTrack();
		CDTrack = 255;
#endif
	}

	Viewport = InViewport;

	unguard;
}

void UNOpenALAudioSubsystem::RegisterMusic( UMusic* Music )
{
	guard(UNOpenALAudioSubsystem::RegisterMusic)

	FScopedLock Lock( MusicMutex );

	if( Music->Handle || !Music->Data.Num() )
		return;

	INT Err = xmp_load_module_from_memory( MusicCtx, &Music->Data(0), Music->Data.Num() );
	if( Err < 0 )
	{
		debugf( NAME_Warning, "Couldn't load music `%s`: %d", Music->GetName(), Err );
		return;
	}

	Err = xmp_start_player( MusicCtx, OutputRate, 0 );
	if( Err < 0 )
	{
		xmp_release_module( MusicCtx );
		debugf( NAME_Warning, "Couldn't start player on `%s`: %d", Music->GetName(), Err );
		return;
	}

	Music->Handle = (void*)1;
	MusicIsLoaded = true;

	unguard;
}

void UNOpenALAudioSubsystem::UnregisterMusic( UMusic* Music )
{
	guard(UNOpenALAudioSubsystem::UnregisterMusic)

	FScopedLock Lock( MusicMutex );

	StopMusic();
	ClearMusicBuffers();
	if( MusicCtx )
	{
		xmp_end_player( MusicCtx );
		if( MusicIsLoaded )
			xmp_release_module( MusicCtx );
	}

	MusicIsLoaded = false;

	unguard;
}

void UNOpenALAudioSubsystem::RegisterSound( USound* Sound )
{
	guard(UNOpenALAudioSubsystem::RegisterSound)

	if( Sound->Handle )
		return;

	check( Sound->Data.Num() );

	FWaveModInfo WaveInfo;
	if( !WaveInfo.ReadWaveInfo( Sound->Data ) )
	{
		debugf( NAME_Warning, "Sound %s is not a valid WAV file", Sound->GetName() );
		return;
	}

	ALuint Buf = 0;
	alGenBuffers( 1, &Buf );
	Buffers.AddItem( Buf );

	ALenum Format = AL_FORMAT_MONO8;
	if( *WaveInfo.pChannels == 2 )
	{
		if( *WaveInfo.pBitsPerSample == 16 )
			Format = AL_FORMAT_STEREO16;
		else
			Format = AL_FORMAT_STEREO8;
	}
	else
	{
		if( *WaveInfo.pBitsPerSample == 16 )
			Format = AL_FORMAT_MONO16;
		else
			Format = AL_FORMAT_MONO8;
	}

	alBufferData( Buf, Format, (const void*)WaveInfo.SampleDataStart, WaveInfo.SampleDataSize, *WaveInfo.pSamplesPerSec );

	Sound->Handle = (void*)(PTRINT)Buf;
	Sound->Looping = ( WaveInfo.SampleLoopsNum != 0 ); // the only indication of looping in this version of UE1

	if( !GIsEditor )
		Sound->Data.Empty();

	unguard;
}

void UNOpenALAudioSubsystem::UnregisterSound( USound* Sound )
{
	guard(UNOpenALAudioSubsystem::UnregisterSound)

	check( Sound );

	if( Sound->Handle )
	{
		ALuint Buf = (ALuint)(PTRINT)Sound->Handle;
		check( alIsBuffer( Buf ) );

		for( INT i = 0; i < MAX_SOURCES; ++i )
		{
			if( Voices[i].Sound == Sound )
				StopVoice( i );
		}

		Buffers.RemoveItem( Buf );
		alDeleteBuffers( 1, &Buf );

		Sound->Handle = NULL;
	}

	unguard;
}

void UNOpenALAudioSubsystem::UpdateVoice( INT Num, const ENVoiceOp Op )
{
	guard(UNOpenALAudioSubsystem::UpdateVoice)

	FNVoice& Voice = Voices[Num];

	// Swap position and velocity into AL space.
	FVector ALVelocity;
	ALVelocity.X = Voice.Velocity.X;
	ALVelocity.Y = Voice.Velocity.Y;
	ALVelocity.Z = -Voice.Velocity.Z;

	// If the source is close enough to the listener, don't spatialize it.
	DWORD SourceRelative;
	FVector ALLocation;
	if( FDistSquared( ListenerCoords.Origin, Voice.Location ) < Square( Voice.Radius * DESPATIALIZE_FACTOR ) )
	{
		SourceRelative = AL_TRUE;
		ALLocation.X = 0.f;
		ALLocation.Y = 0.f;
		ALLocation.Z = 0.f;
		// If the source is ON the listener, also reset the velocity so we don't doppler our own voice.
		if( Viewport && Viewport->Actor && Voice.Actor == Viewport->Actor )
		{
			ALVelocity.X = 0.f;
			ALVelocity.Y = 0.f;
			ALVelocity.Z = 0.f;
		}
	}
	else
	{
		SourceRelative = AL_FALSE;
		ALLocation.X = Voice.Location.X;
		ALLocation.Y = Voice.Location.Y;
		ALLocation.Z = -Voice.Location.Z;
	}

	// Set up AL source.
	ALuint Source = Sources[Num];
	alSourcei( Source, AL_SOURCE_RELATIVE, SourceRelative );
	alSourcef( Source, AL_GAIN, Voice.Volume * ( SoundVolume / 255.f ) );
	alSourcef( Source, AL_PITCH, Voice.Pitch );
	alSourcef( Source, AL_MAX_DISTANCE, Voice.Radius );
	alSourcef( Source, AL_REFERENCE_DISTANCE, Voice.Radius * DESPATIALIZE_FACTOR );
	alSourcef( Source, AL_ROLLOFF_FACTOR, ROLLOFF_FACTOR );
	alSourcefv( Source, AL_POSITION, &ALLocation.X );
	alSourcefv( Source, AL_VELOCITY, &ALVelocity.X );
	alSourcei( Source, AL_LOOPING, Voice.Looping );

	if( Voice.BufferChanged )
	{
		Voice.BufferChanged = false;
		INT State = AL_STOPPED;
		alGetSourcei( Source, AL_SOURCE_STATE, &State );
		if( State == AL_PLAYING || State == AL_PAUSED )
			alSourceStop( Source );
		alSourcei( Source, AL_BUFFER, Voice.Buffer );
		if( State == AL_PLAYING || State == AL_PAUSED )
		{
			alSourcePlay( Source );
			if( State == AL_PAUSED )
				alSourcePause( Source );
		}
	}

	if( EfxReverb && Op == NVOP_Play )
		alSource3i( Source, AL_AUXILIARY_SEND_FILTER, (ALint)ReverbSlot, 0, AL_FILTER_NULL );

	// Play or stop if needed.
	switch( Op )
	{
		case ENVoiceOp::NVOP_Play:  alSourcePlay( Source ); break;
		case ENVoiceOp::NVOP_Pause: alSourcePause( Source ); break;
		case ENVoiceOp::NVOP_Stop:  alSourceStop( Source ); break;
		default: break;
	}

	unguard;
}

UBOOL UNOpenALAudioSubsystem::PlaySound( AActor* Actor, INT Id, USound* Sound, FVector Location, FLOAT Volume, FLOAT Radius, FLOAT Pitch )
{
	guard(UNOpenALAudioSubsystem::PlaySound)

	if( !Viewport )
		return false;

	// Allocate a new slot if requested.
	if( SOUND_SLOT_IS( Id, SLOT_None ) )
		Id = 16 * --NextId;

	FLOAT Priority = GetVoicePriority( Location, Volume, Radius );
	FLOAT MaxPriority = Priority;
	FNVoice* Voice = NULL;
	for( INT i = 0; i < MAX_SOURCES; ++i )
	{
		FNVoice* V = &Voices[i];
		if( ( V->Id & ~1 ) == ( Id & ~1 ) )
		{
			// Skip if not interruptable.
			if( Id & 1 )
				return false;
			StopVoice( i );
			Voice = V;
			break;
		}
		else if( V->Priority <= MaxPriority )
		{
			MaxPriority = V->Priority;
			Voice = V;
		}
	}

	// If we ran out of voices or the sound is too low priority, bail.
	if( !Voice || !Sound || !Sound->Handle )
		return false;

	ALuint Buf = (ALuint)(PTRINT)Sound->Handle;
	check( alIsBuffer( Buf ) );

	Voice->Id = Id;
	Voice->BufferChanged = ( Buf != Voice->Buffer );
	Voice->Buffer = Buf;
	Voice->Location = Location;
	Voice->Velocity = Actor ? Actor->Velocity : FVector();
	Voice->Volume = Clamp( Volume, 0.f, 1.f );
	Voice->Radius = Radius;
	Voice->Pitch = Pitch;
	Voice->Priority = Priority;
	Voice->Actor = Actor;
	Voice->Looping = Sound->Looping;
	Voice->Sound = Sound;

	// Start the voice.
	UpdateVoice( Voice - Voices, NVOP_Play );

	return true;

	unguard;
}

void UNOpenALAudioSubsystem::NoteDestroy( AActor* Actor )
{
	guard(UNOpenALAudioSubsystem::NoteDestroy)

	check(Actor);
	check(Actor->IsValid());
	for( INT i = 0; i < MAX_SOURCES; ++i)
	{
		if( Voices[i].Actor == Actor )
		{
			if( SOUND_SLOT_IS( Voices[i].Id, SLOT_Ambient ) )
			{
				// Stop ambient sound when actor dies.
				StopVoice( i );
			}
			else
			{
				// Unbind regular sounds from actors.
				Voices[i].Actor = NULL;
			}
		}
	}

	unguard;
}

void UNOpenALAudioSubsystem::StopVoice( INT Num )
{
	guard(UNOpenALAudioSubsystem::StopVoice)

	FNVoice& Voice = Voices[Num];

	alSourcei( Sources[Num], AL_LOOPING, AL_FALSE );
	alSourceStop( Sources[Num] );

	if( Voice.Buffer != INVALID_BUFFER )
	{
		Voice.Buffer = INVALID_BUFFER;
		alSourcei( Sources[Num], AL_BUFFER, 0 );
	}

	Voice.Priority = 0.f;
	Voice.Sound = NULL;
	Voice.Actor = NULL;
	Voice.Looping = false;
	Voice.Id = 0;

	unguard;
}

void UNOpenALAudioSubsystem::PlayMusic()
{
	guard(UNOpenALAudioSubsystem::PlayMusic)

	FScopedLock Lock( MusicMutex );
	MUSIC_AL_SCOPE;

	alSourceStop(MusicSource);
	ClearMusicBuffers();
	xmp_set_position( MusicCtx, MusicSection );

	ALint State = 0;
	alGetSourcei( MusicSource, AL_SOURCE_STATE, &State );
	if( State != AL_PLAYING )
		alSourcePlay( MusicSource );

	MusicIsPlaying = true;

	unguard;
}

void UNOpenALAudioSubsystem::StopMusic()
{
	guard(UNOpenALAudioSubsystem::StopMusic)

	FScopedLock Lock( MusicMutex );
	MUSIC_AL_SCOPE;

	MusicIsPlaying = false;
	alSourceStop( MusicSource );

	unguard;
}

#if defined(PLATFORM_ANDROID) || defined(UNREAL_ANDROID) || defined(__ANDROID__)
void UNOpenALAudioSubsystem::AndroidSetSuspended( UBOOL bSuspended )
{
	guard(UNOpenALAudioSubsystem::AndroidSetSuspended)

	if( AndroidSuspended == bSuspended )
		return;

	AndroidSuspended = bSuspended;

	if( !Ctx )
		return;

	FScopedLock Lock( MusicMutex );
	alcMakeContextCurrent( Ctx );

	if( bSuspended )
	{
		for( INT i = 0; i < MAX_SOURCES; ++i )
		{
			ALint State = AL_STOPPED;
			alGetSourcei( Sources[i], AL_SOURCE_STATE, &State );
			if( State == AL_PLAYING )
				alSourcePause( Sources[i] );
		}

		alSourcePause( MusicSource );

		if( Device && alcIsExtensionPresent( Device, "ALC_SOFT_pause_device" ) )
			alcDevicePauseSOFT( Device );
		else
			alcSuspendContext( Ctx );

		debugf( NAME_Log, "Android OpenAL suspended" );
	}
	else
	{
		if( Device && alcIsExtensionPresent( Device, "ALC_SOFT_pause_device" ) )
			alcDeviceResumeSOFT( Device );
		else
			alcProcessContext( Ctx );

		for( INT i = 0; i < MAX_SOURCES; ++i )
		{
			ALint State = AL_STOPPED;
			alGetSourcei( Sources[i], AL_SOURCE_STATE, &State );
			if( State == AL_PAUSED )
				alSourcePlay( Sources[i] );
		}

		if( MusicIsPlaying && Music )
			alSourcePlay( MusicSource );

		debugf( NAME_Log, "Android OpenAL resumed" );
	}

	unguard;
}
#endif

void UNOpenALAudioSubsystem::Update( FPointRegion Region, FCoords& Listener )
{
	guard(UNOpenALAudioSubsystem::Update)

	if( !Viewport || !Viewport->IsRealtime() )
		return;
#if defined(PLATFORM_ANDROID) || defined(UNREAL_ANDROID) || defined(__ANDROID__)
	if( AndroidSuspended )
		return;
#endif

	// Update AL listener position, velocity and orientation.
	FVector ALPosition;
	ALPosition.X = Listener.Origin.X;
	ALPosition.Y = Listener.Origin.Y;
	ALPosition.Z = -Listener.Origin.Z;
	FVector ALVelocity;
	if( Viewport->Actor )
	{
		ALVelocity.X = Viewport->Actor->Velocity.X;
		ALVelocity.Y = Viewport->Actor->Velocity.Y;
		ALVelocity.Z = -Viewport->Actor->Velocity.Z;
	}
	FLOAT ALOrientation[] = {
		+Listener.ZAxis.X,
		+Listener.ZAxis.Y,
		-Listener.ZAxis.Z,
		-Listener.YAxis.X,
		-Listener.YAxis.Y,
		+Listener.YAxis.Z,
	};
	alListenerfv( AL_POSITION, &ALPosition.X );
	alListenerfv( AL_VELOCITY, &ALVelocity.X );
	alListenerfv( AL_ORIENTATION, ALOrientation );
	ListenerCoords = Listener;

#ifdef KHG_GALAXYREVERB
	UpdateGalaxyReverb();
#else
	if( UseReverb )
		UpdateReverb( Region );
#endif

	// Start new ambient sounds if needed.
	if( Viewport->Actor && Viewport->Actor->XLevel )
	{
		for( INT i = 0; i < Viewport->Actor->XLevel->Num(); i++ )
		{
			AActor* Actor = Viewport->Actor->XLevel->Actors(i);
			if( !Actor || !Actor->IsValid() )
				continue;

			const FLOAT DistSq = FDistSquared( Viewport->Actor->Location, Actor->Location );
			const FLOAT AmbRad = Square( Actor->WorldSoundRadius() );
			if( !Actor->AmbientSound || DistSq > AmbRad )
				continue;

			// See if it's already playing.
			INT Id = AMBIENT_SOUND_ID( Actor->GetIndex() );
			INT AmbientNum;
			for( AmbientNum = 0; AmbientNum < MAX_SOURCES; ++AmbientNum )
			{
				if( Voices[AmbientNum].Id == Id )
					break;
			}

			// If not, start it.
			if( AmbientNum == MAX_SOURCES )
			{
				FLOAT Vol = AmbientFactor * Actor->SoundVolume / 255.f;
				FLOAT Rad = Actor->WorldSoundRadius();
				FLOAT Pitch = Actor->SoundPitch / 64.f;
				PlaySound( Actor, Id, Actor->AmbientSound, Actor->Location, Vol, Rad, Pitch );
			}
		}
	}

	// Update active ambient sounds.
	for( INT VoiceNum = 0; VoiceNum < MAX_SOURCES; ++VoiceNum )
	{
		FNVoice& Voice = Voices[VoiceNum];
		if( !Voice.Id || !Voice.Sound || Voice.Buffer == INVALID_BUFFER || !SOUND_SLOT_IS( Voice.Id, SLOT_Ambient ) )
			continue;

		check( Voice.Actor );

		const FLOAT DistSq = FDistSquared( Viewport->Actor->Location, Voice.Actor->Location );
		const FLOAT AmbRad = Square( Voice.Actor->WorldSoundRadius() );
		if( Voice.Sound != Voice.Actor->AmbientSound || DistSq > AmbRad )
		{
			// Sound changed or went out of range.
			StopVoice( VoiceNum );
		}
		else
		{
			// Update parameters. These will be applied in the loop below.
			Voice.Radius = Voice.Actor->WorldSoundRadius();
			Voice.Pitch = Voice.Actor->SoundPitch / 64.f;
			Voice.Volume = AmbientFactor * Voice.Actor->SoundVolume / 255.f;
			if( Voice.Actor->LightType != LT_None )
				Voice.Volume *= Voice.Actor->LightBrightness / 255.f;
		}
	}

	// Update all active voices.
	for( INT VoiceNum = 0; VoiceNum < MAX_SOURCES; ++VoiceNum )
	{
		FNVoice& Voice = Voices[VoiceNum];
		if( !Voice.Id || !Voice.Sound || Voice.Buffer == INVALID_BUFFER )
			continue;

		ALuint Source = Sources[VoiceNum];
		ALint SourceState;
		alGetSourcei( Source, AL_SOURCE_STATE, &SourceState );
		if( SourceState == AL_STOPPED )
		{
			// Voice has finished playing.
			StopVoice( VoiceNum );
		}
		else if( SourceState == AL_PLAYING )
		{
			// Voice is playing, update its location and priority.
			if( Voice.Actor && Voice.Actor->IsValid() )
			{
				Voice.Location = Voice.Actor->Location;
				Voice.Velocity = Voice.Actor->Velocity;
			}
			Voice.Priority = GetVoicePriority( Voice.Location, Voice.Volume, Voice.Radius );
			// Update AL source.
			UpdateVoice( VoiceNum );
		}
	}

	// Update music.
	DOUBLE DeltaTime = appSeconds() - MusicTime;
	MusicTime += DeltaTime;
	DeltaTime = Clamp( DeltaTime, 0.0, 1.0 );
	if( Viewport->Actor && Viewport->Actor->Transition != MTRAN_None )
	{
		// Track is changing.
		UBOOL MusicChanged = Music != Viewport->Actor->Song;
#ifdef KHG_CDMUSIC
		// Like Galaxy: a CD track that is still on counts as playing music until the transition is done.
		if( Music || CDTrack != 255 )
#else
		if( Music )
#endif
		{
			// Already playing something, figure out if we're ready to change.
			UBOOL MusicDone = false;
			if( MusicSection == 255 )
			{
				MusicDone = true;
			}
			else if( Viewport->Actor->Transition == MTRAN_Fade )
			{
				MusicFade -= DeltaTime;
				MusicDone = ( MusicFade < -2.f / 1000.f );
			}
			else if( Viewport->Actor->Transition == MTRAN_SlowFade )
			{
				MusicFade -= DeltaTime * 0.2;
				MusicDone = ( MusicFade < 0.2f * -2.f / 1000.f );
			}
			else if( Viewport->Actor->Transition == MTRAN_FastFade )
			{
				MusicFade -= DeltaTime * 3.0;
				MusicDone = ( MusicFade < 3.0f * -2.f / 1000.f );
			}
			else
			{
				MusicDone = true;
			}

			MusicMutex.Lock();

			if( MusicDone )
			{
				if( Music && MusicChanged )
					UnregisterMusic( Music );
				Music = NULL;
#ifdef KHG_CDMUSIC
				StopCDTrack();
				CDTrack = 255;
#endif
			}
			else
			{
				MUSIC_AL_SCOPE;
				alSourcef( MusicSource, AL_GAIN, Max(MusicFade, 0.f) * MusicVolume / 255.f );
			}

			MusicMutex.Unlock();
		}

		if( Music == NULL )
		{
			FScopedLock Lock( MusicMutex );
			MusicFade = 1.f;
			{
				MUSIC_AL_SCOPE;
				alSourcef( MusicSource, AL_GAIN, Max(MusicFade, 0.f) * MusicVolume / 255.f );
			}
			Music = Viewport->Actor->Song;
			MusicSection = Viewport->Actor->SongSection;
			if( Music )
			{
				if( MusicChanged )
					RegisterMusic( Music );
				// Like Galaxy, the song is still followed for transitions but only played with UseDigitalMusic.
				if( MusicSection != 255 && UseDigitalMusic )
					PlayMusic();
				else
					StopMusic();
			}
#ifdef KHG_CDMUSIC
			// Galaxy then starts PlayerPawn.CdTrack from its beginning, once, next to any song.
			CDTrack = Viewport->Actor->CdTrack;
			if( CDTrack != 255 && UseCDMusic )
				StartCDTrack( CDTrack );
#endif
			Viewport->Actor->Transition = MTRAN_None;
		}
	}

	unguard;
}

void UNOpenALAudioSubsystem::UpdateMusicBuffers()
{
	guard(UNOpenALAudioSubsystem::UpdateMusicBuffers)

	FScopedLock Lock( MusicMutex );
	MUSIC_AL_SCOPE;

#if defined(PLATFORM_ANDROID) || defined(UNREAL_ANDROID) || defined(__ANDROID__)
	if( AndroidSuspended )
		return;
#endif

	// First dequeue the buffers that are done playing and put them into the buffer pool
	ALint BuffersProcessed = 0;
	ALint BuffersQueued = 0;
	ALint State = AL_STOPPED;
	alGetSourcei( MusicSource, AL_BUFFERS_PROCESSED, &BuffersProcessed );
	alGetSourcei( MusicSource, AL_BUFFERS_QUEUED, &BuffersQueued );
	alGetSourcei( MusicSource, AL_SOURCE_STATE, &State );
	if( BuffersProcessed > 0 && NumFreeMusicBuffers < NUM_MUSIC_BUFFERS )
	{
		const INT NumToUnqueue = Min( NUM_MUSIC_BUFFERS - NumFreeMusicBuffers, BuffersProcessed );
		alSourceUnqueueBuffers( MusicSource, NumToUnqueue, &FreeMusicBuffers[NumFreeMusicBuffers] );
		NumFreeMusicBuffers += NumToUnqueue;
	}

	if( !Music || !MusicIsPlaying || MusicSection == 255 || !MusicCtx )
		return;

	// If music is playing, render and queue more buffers if available
	while( BuffersQueued < NUM_MUSIC_BUFFERS && NumFreeMusicBuffers )
	{
		if( xmp_play_buffer( MusicCtx, MusicBufferData, sizeof( MusicBufferData ), 0 ) < 0 )
			break;
		alBufferData( FreeMusicBuffers[NumFreeMusicBuffers - 1], AL_FORMAT_STEREO16, MusicBufferData, sizeof( MusicBufferData ), OutputRate );
		alSourceQueueBuffers( MusicSource, 1, &FreeMusicBuffers[NumFreeMusicBuffers - 1] );
		--NumFreeMusicBuffers;
		++BuffersQueued;
	}

	// If it stopped because it ran out of buffers, restart it
	if( BuffersQueued > 0 && ( State == AL_INITIAL || State == AL_STOPPED ) )
		alSourcePlay( MusicSource );

	unguard;
}

void UNOpenALAudioSubsystem::ClearMusicBuffers()
{
	guard(UNOpenALAudioSubsystem::ClearMusicBuffers)

	FScopedLock Lock( MusicMutex );
	MUSIC_AL_SCOPE;

	appMemset( (void*)MusicBufferData, 0, sizeof(MusicBufferData) );

	ALint BuffersProcessed = 0;
	alGetSourcei( MusicSource, AL_BUFFERS_PROCESSED, &BuffersProcessed );
	if( BuffersProcessed > 0 && NumFreeMusicBuffers < NUM_MUSIC_BUFFERS )
	{
		const INT NumToUnqueue = Min( NUM_MUSIC_BUFFERS - NumFreeMusicBuffers, BuffersProcessed );
		alSourceUnqueueBuffers( MusicSource, NumToUnqueue, &FreeMusicBuffers[NumFreeMusicBuffers] );
		NumFreeMusicBuffers += NumToUnqueue;
	}

	for( INT i = 0; i < NumFreeMusicBuffers; ++i )
		alBufferData( FreeMusicBuffers[i], AL_FORMAT_STEREO16, MusicBufferData, sizeof( MusicBufferData ), OutputRate );

	unguard;
}

void UNOpenALAudioSubsystem::UpdateReverb( FPointRegion& Region )
{
	guard(UNOpenALAudioSubsystem::UpdateReverb)

	const UBOOL bNewReverb = ( Viewport->Actor && Viewport->Actor->Region.Zone && Viewport->Actor->Region.Zone->bReverbZone );

	if( !bNewReverb && ReverbOn )
	{
		// Just turn it off.
		ReverbOn = false;
		ReverbZone = NULL;
		// TODO: is there a better way to toggle this?
		alAuxiliaryEffectSloti( ReverbSlot, AL_EFFECTSLOT_EFFECT, AL_EFFECT_NULL );
		return;
	}

	ReverbOn = bNewReverb;

	if( ReverbOn )
	{
		AZoneInfo* NewReverbZone = Viewport->Actor->Region.Zone;
		if( NewReverbZone != ReverbZone )
		{
			// Reverb zone changed.
			ReverbZone = NewReverbZone;
			// Unbind effect, change parameters, then bind again.
			alAuxiliaryEffectSloti( ReverbSlot, AL_EFFECTSLOT_EFFECT, AL_EFFECT_NULL );
			alEffectf( ReverbEffect, AL_EAXREVERB_GAIN, ReverbZone->MasterGain / 255.f );
			// TODO: figure out how to convert Galaxy Audio reverb to this
			alAuxiliaryEffectSloti( ReverbSlot, AL_EFFECTSLOT_EFFECT, ReverbEffect );
		}
	}

	unguard;
}

void UNOpenALAudioSubsystem::InitReverbEffect()
{
	guard(UNOpenALAudioSubsystem::InitReverbEffect)

	EFXEAXREVERBPROPERTIES Reverb = EFX_REVERB_PRESET_GENERIC;
	alEffecti( ReverbEffect, AL_EFFECT_TYPE, AL_EFFECT_EAXREVERB );
	alEffectf( ReverbEffect, AL_EAXREVERB_DENSITY, Reverb.flDensity );
	alEffectf( ReverbEffect, AL_EAXREVERB_DIFFUSION, Reverb.flDiffusion );
	alEffectf( ReverbEffect, AL_EAXREVERB_GAIN, Reverb.flGain );
	alEffectf( ReverbEffect, AL_EAXREVERB_GAINHF, Reverb.flGainHF );
	alEffectf( ReverbEffect, AL_EAXREVERB_GAINLF, Reverb.flGainLF );
	alEffectf( ReverbEffect, AL_EAXREVERB_DECAY_TIME, Reverb.flDecayTime );
	alEffectf( ReverbEffect, AL_EAXREVERB_DECAY_HFRATIO, Reverb.flDecayHFRatio );
	alEffectf( ReverbEffect, AL_EAXREVERB_DECAY_LFRATIO, Reverb.flDecayLFRatio );
	alEffectf( ReverbEffect, AL_EAXREVERB_REFLECTIONS_GAIN, Reverb.flReflectionsGain);
	alEffectf( ReverbEffect, AL_EAXREVERB_REFLECTIONS_DELAY, Reverb.flReflectionsDelay );
	alEffectfv( ReverbEffect, AL_EAXREVERB_REFLECTIONS_PAN, Reverb.flReflectionsPan );
	alEffectf( ReverbEffect, AL_EAXREVERB_LATE_REVERB_GAIN, Reverb.flLateReverbGain );
	alEffectf( ReverbEffect, AL_EAXREVERB_LATE_REVERB_DELAY, Reverb.flLateReverbDelay );
	alEffectfv( ReverbEffect, AL_EAXREVERB_LATE_REVERB_PAN, Reverb.flLateReverbPan );
	alEffectf( ReverbEffect, AL_EAXREVERB_ECHO_TIME, Reverb.flEchoTime );
	alEffectf( ReverbEffect, AL_EAXREVERB_ECHO_DEPTH, Reverb.flEchoDepth );
	alEffectf( ReverbEffect, AL_EAXREVERB_MODULATION_TIME, Reverb.flModulationTime );
	alEffectf( ReverbEffect, AL_EAXREVERB_MODULATION_DEPTH, Reverb.flModulationDepth );
	alEffectf( ReverbEffect, AL_EAXREVERB_AIR_ABSORPTION_GAINHF, Reverb.flAirAbsorptionGainHF );
	alEffectf( ReverbEffect, AL_EAXREVERB_HFREFERENCE, Reverb.flHFReference );
	alEffectf( ReverbEffect, AL_EAXREVERB_LFREFERENCE, Reverb.flLFReference );
	alEffectf( ReverbEffect, AL_EAXREVERB_ROOM_ROLLOFF_FACTOR, Reverb.flRoomRolloffFactor );
	alEffecti( ReverbEffect, AL_EAXREVERB_DECAY_HFLIMIT, Reverb.iDecayHFLimit );

	unguard;
}

UBOOL UNOpenALAudioSubsystem::Exec( const char* Cmd, FOutputDevice* Out )
{
	guard(UNOpenALAudioSubsystem::Exec)

#ifdef KHG_CDMUSIC
	if( ParseCommand( &Cmd, "CDTRACK" ) )
	{
		// Galaxy's console command: set PlayerPawn.CdTrack and start an instant transition.
		if( Viewport && Viewport->Actor && *Cmd )
		{
			const BYTE Track = (BYTE)atoi( Cmd );
			Out->Logf( "CD Track %i", Track );
			Viewport->Actor->CdTrack = Track;
			Viewport->Actor->Transition = MTRAN_Instant;
			return true;
		}
	}
	else
#endif
	if( ParseCommand( &Cmd, "MusicOrder") )
	{
		if( Music && MusicCtx )
		{
			FScopedLock Lock( MusicMutex );
			INT Pos = atoi( Cmd );
			Out->Logf( "Set music position to %d", Pos );
			xmp_set_position( MusicCtx, Pos );
			MusicSection = Pos;
			return true;
		}
	}
	else if( ParseCommand( &Cmd, "MusicInterp" ) )
	{
		FScopedLock Lock( MusicMutex );
		MusicInterpolation = Clamp( atoi( Cmd ), 0, XMP_INTERP_SPLINE );
		if( MusicCtx )
			xmp_set_player( MusicCtx, XMP_PLAYER_INTERP, MusicInterpolation );
		return true;
	}

	return false;

	unguard;
}

void UNOpenALAudioSubsystem::StartMusicThread()
{
	guard(UNOpenALAudioSubsystem::StartMusicThread)

	// This isn't an atomic because we only set it before the thread starts and before we wait on it to join.
	MusicThreadRunning = true;

	MusicThread = appThreadSpawn( MusicThreadProc, (void*)this, "MusicThread", true, nullptr );
	check(MusicThread);

	unguard;
}

void UNOpenALAudioSubsystem::StopMusicThread()
{
	guard(UNOpenALAudioSubsystem::StopMusicThread)

	if( MusicThread )
	{
		MusicThreadRunning = false;
		appThreadJoin( MusicThread );
		MusicThread = nullptr;
	}

	unguard;
}

#ifdef PLATFORM_WIN32
DWORD __stdcall UNOpenALAudioSubsystem::MusicThreadProc( void* Audio )
#else
void* UNOpenALAudioSubsystem::MusicThreadProc( void* Audio )
#endif
{
	UNOpenALAudioSubsystem* This = (UNOpenALAudioSubsystem*)Audio;

	while( This->MusicThreadRunning )
	{
		This->UpdateMusicBuffers();
#ifdef KHG_CDMUSIC
		This->UpdateCDBuffers();
#endif
		appSleep( 0.1f );
	}

	return (THREAD_RET)0;
}

#ifdef KHG_CDMUSIC
/*-----------------------------------------------------------------------------
	[KHG] Ripped CD tracks.
-----------------------------------------------------------------------------*/

void UNOpenALAudioSubsystem::StartCDTrack( BYTE Track )
{
	guard(UNOpenALAudioSubsystem::StartCDTrack)

	StopCDTrack();

	// CDMusicPath is relative to the System folder unless it is absolute.
	char Dir[512];
	const char* Base = appBaseDir();
	if( CDMusicPath[0] && ( CDMusicPath[1] == ':' || CDMusicPath[0] == '\\' || CDMusicPath[0] == '/' ) )
		appSprintf( Dir, "%s", CDMusicPath );
	else
		appSprintf( Dir, "%s%s", Base ? Base : "", CDMusicPath );

	std::string File = CDMusicFindTrack( Dir, Track );
#ifdef PLATFORM_WIN32
	if( File.empty() )
	{
		// [KHG] Tracks copied from the game CD when the install can't be written to (KHGDiscImport.cpp).
		char UserMusic[512];
		appSprintf( UserMusic, "%sMusic", appUserDir() );
		File = CDMusicFindTrack( UserMusic, Track );
	}
#endif
	if( File.empty() )
	{
		debugf( NAME_Log, "[KHG] CD track %i: no file for it in %s (as without the game CD)", Track, Dir );
		return;
	}
	char Error[256] = "";
	FCDMusicStream* Stream = CDMusicOpen( File.c_str(), OutputRate, Error, sizeof( Error ) );
	if( !Stream )
	{
		debugf( NAME_Warning, "[KHG] CD track %i: cannot play %s: %s", Track, File.c_str(), Error );
		return;
	}
	debugf( NAME_Log, "[KHG] CD track %i: playing %s", Track, File.c_str() );

	FScopedLock Lock( MusicMutex );
	MUSIC_AL_SCOPE;
	CDStream = Stream;
	alSourcef( CDSource, AL_GAIN, MusicVolume / 255.f );

	unguard;
}

void UNOpenALAudioSubsystem::StopCDTrack()
{
	guard(UNOpenALAudioSubsystem::StopCDTrack)

	FScopedLock Lock( MusicMutex );
	MUSIC_AL_SCOPE;

	if( CDStream )
	{
		CDMusicClose( CDStream );
		CDStream = nullptr;
	}
	if( CDSource != INVALID_SOURCE )
	{
		alSourceStop( CDSource );
		alSourcei( CDSource, AL_BUFFER, 0 ); // unqueues everything of a stopped source
		NumFreeCDBuffers = NUM_MUSIC_BUFFERS;
		for( INT i = 0; i < NUM_MUSIC_BUFFERS; ++i )
			FreeCDBuffers[i] = CDBuffers[i];
	}

	unguard;
}

void UNOpenALAudioSubsystem::UpdateCDBuffers()
{
	guard(UNOpenALAudioSubsystem::UpdateCDBuffers)

	FScopedLock Lock( MusicMutex );
	MUSIC_AL_SCOPE;

	if( CDSource == INVALID_SOURCE )
		return;

	ALint Processed = 0, Queued = 0, State = AL_STOPPED;
	alGetSourcei( CDSource, AL_BUFFERS_PROCESSED, &Processed );
	alGetSourcei( CDSource, AL_BUFFERS_QUEUED, &Queued );
	alGetSourcei( CDSource, AL_SOURCE_STATE, &State );
	if( Processed > 0 && NumFreeCDBuffers < NUM_MUSIC_BUFFERS )
	{
		const INT N = Min( NUM_MUSIC_BUFFERS - NumFreeCDBuffers, Processed );
		alSourceUnqueueBuffers( CDSource, N, &FreeCDBuffers[NumFreeCDBuffers] );
		NumFreeCDBuffers += N;
		Queued -= N;
	}

	if( !CDStream )
		return;

	const INT FramesPerBuffer = ARRAY_COUNT( CDBufferData ) / 2;
	while( Queued < NUM_MUSIC_BUFFERS && NumFreeCDBuffers )
	{
		const INT Got = CDMusicRead( CDStream, CDBufferData, FramesPerBuffer );
		if( Got <= 0 )
		{
			// The track has ended. Like MCI_PLAY on the CD it is not repeated.
			CDMusicClose( CDStream );
			CDStream = nullptr;
			break;
		}
		const ALuint Buf = FreeCDBuffers[--NumFreeCDBuffers];
		alBufferData( Buf, AL_FORMAT_STEREO16, CDBufferData, Got * 2 * sizeof( short ), OutputRate );
		alSourceQueueBuffers( CDSource, 1, &Buf );
		++Queued;
	}

	if( Queued > 0 && ( State == AL_INITIAL || State == AL_STOPPED ) )
		alSourcePlay( CDSource );

	unguard;
}
#endif

#ifdef KHG_GALAXYREVERB
/*-----------------------------------------------------------------------------
	[KHG] Galaxy zone reverb: effects loopback + streamed output.
-----------------------------------------------------------------------------*/

UBOOL UNOpenALAudioSubsystem::InitFxLoopback()
{
	guard(UNOpenALAudioSubsystem::InitFxLoopback)

	if( !alcIsExtensionPresent( NULL, "ALC_SOFT_loopback" ) || !alcIsExtensionPresent( NULL, "ALC_EXT_thread_local_context" ) )
		return false;

	// Mix the effects with the real device's speaker layout, so 5.1/7.1 output still works.
	ALCint OutMode = 0;
	if( alcIsExtensionPresent( Device, "ALC_SOFT_output_mode" ) )
		alcGetIntegerv( Device, ALC_OUTPUT_MODE_SOFT, 1, &OutMode );
	ALCint Chans = ALC_STEREO_SOFT;
	const char* FormatName = "AL_FORMAT_STEREO16";
	FxChannels = 2;
	switch( OutMode )
	{
		case ALC_QUAD_SOFT:          Chans = ALC_QUAD_SOFT;     FormatName = "AL_FORMAT_QUAD16";  FxChannels = 4; break;
		case ALC_SURROUND_5_1_SOFT:  Chans = ALC_5POINT1_SOFT;  FormatName = "AL_FORMAT_51CHN16"; FxChannels = 6; break;
		case ALC_SURROUND_6_1_SOFT:  Chans = ALC_6POINT1_SOFT;  FormatName = "AL_FORMAT_61CHN16"; FxChannels = 7; break;
		case ALC_SURROUND_7_1_SOFT:  Chans = ALC_7POINT1_SOFT;  FormatName = "AL_FORMAT_71CHN16"; FxChannels = 8; break;
		default: break;
	}
	FxOutFormat = FxChannels == 2 ? AL_FORMAT_STEREO16 : alGetEnumValue( FormatName );
	if( !FxOutFormat || FxOutFormat == -1 )
	{
		Chans = ALC_STEREO_SOFT;
		FxChannels = 2;
		FxOutFormat = AL_FORMAT_STEREO16;
	}

	FxDevice = alcLoopbackOpenDeviceSOFT( NULL );
	if( !FxDevice )
		return false;
	if( !alcIsRenderFormatSupportedSOFT( FxDevice, OutputRate, Chans, ALC_SHORT_SOFT ) )
	{
		alcCloseDevice( FxDevice );
		FxDevice = nullptr;
		return false;
	}
	const ALCint Attr[] = {
		ALC_FORMAT_CHANNELS_SOFT, Chans,
		ALC_FORMAT_TYPE_SOFT, ALC_SHORT_SOFT,
		ALC_FREQUENCY, OutputRate,
		// HRTF exactly when the real device uses it (OpenAL Soft turns it on for headphones), as before.
		ALC_HRTF_SOFT, UseHRTF && OutMode == ALC_STEREO_HRTF_SOFT ? ALC_TRUE : ALC_FALSE,
		0
	};
	FxCtx = alcCreateContext( FxDevice, Attr );
	if( !FxCtx )
	{
		alcCloseDevice( FxDevice );
		FxDevice = nullptr;
		return false;
	}

	// The effects mix plays on the real device as it is, without HRTF or panning of its own.
	alGenSources( 1, &FxOutSource );
	alSourcei( FxOutSource, AL_SOURCE_RELATIVE, AL_TRUE );
	alSource3f( FxOutSource, AL_POSITION, 0.f, 0.f, 0.f );
	alSourcef( FxOutSource, AL_ROLLOFF_FACTOR, 0.f );
	alSourcei( FxOutSource, AL_DIRECT_CHANNELS_SOFT, AL_TRUE );
	alGenBuffers( ARRAY_COUNT( FxOutBuffers ), FxOutBuffers );
	for( INT i = 0; i < ARRAY_COUNT( FxOutBuffers ); ++i )
		FreeFxOutBuffers[i] = FxOutBuffers[i];
	NumFreeFxOutBuffers = ARRAY_COUNT( FxOutBuffers );

	GalaxyReverb = GalaxyReverbCreate( OutputRate );
	GalaxyParms = FGalaxyReverbParms();
	GalaxyParmsPending = false;

	// From here on, al* calls go to the effects context unless a scope switches to Ctx.
	alcMakeContextCurrent( FxCtx );

	ALCint Hrtf = 0;
	alcGetIntegerv( FxDevice, ALC_HRTF_SOFT, 1, &Hrtf );
	debugf( NAME_Init, "[KHG] Effects: loopback mix, %d channels, HRTF %s, Galaxy zone reverb %s",
		FxChannels, Hrtf ? "on" : "off", UseReverb ? "on" : "off (UseReverb=False)" );

	FxThreadRunning = true;
	FxThread = appThreadSpawn( FxThreadProc, (void*)this, "FxThread", true, nullptr );
	check( FxThread );
	return true;

	unguard;
}

void UNOpenALAudioSubsystem::ShutdownFxLoopback()
{
	guard(UNOpenALAudioSubsystem::ShutdownFxLoopback)

	if( FxThread )
	{
		FxThreadRunning = false;
		appThreadJoin( FxThread );
		FxThread = nullptr;
	}
	if( FxOutSource != INVALID_SOURCE )
	{
		MUSIC_AL_SCOPE;
		alSourceStop( FxOutSource );
		alSourcei( FxOutSource, AL_BUFFER, 0 );
		alDeleteSources( 1, &FxOutSource );
		alDeleteBuffers( ARRAY_COUNT( FxOutBuffers ), FxOutBuffers );
		FxOutSource = INVALID_SOURCE;
	}
	if( FxCtx )
	{
		alcSetThreadContext( NULL );
		alcMakeContextCurrent( Ctx );
		alcDestroyContext( FxCtx );
		FxCtx = nullptr;
	}
	if( FxDevice )
	{
		alcCloseDevice( FxDevice );
		FxDevice = nullptr;
	}
	if( GalaxyReverb )
	{
		GalaxyReverbDestroy( GalaxyReverb );
		GalaxyReverb = nullptr;
	}

	unguard;
}

// Galaxy's UpdateReverb: the zone of the viewport's actor, read every frame; the echo restarts
// only when the values change.
void UNOpenALAudioSubsystem::UpdateGalaxyReverb()
{
	guard(UNOpenALAudioSubsystem::UpdateGalaxyReverb)

	if( !GalaxyReverb )
		return;

	FGalaxyReverbParms P;
	AZoneInfo* Zone = Viewport->Actor ? Viewport->Actor->Region.Zone : NULL;
	if( UseReverb && Zone && Zone->bReverbZone )
	{
		P.MasterGain = Zone->MasterGain;
		P.CutoffHz = Zone->CutoffHz;
		for( INT i = 0; i < 6; i++ )
		{
			P.Delay[i] = Zone->Delay[i];
			P.Gain[i] = Zone->Gain[i];
		}
	}
	if( P == GalaxyParms )
		return;
	GalaxyParms = P;

	if( P.MasterGain )
		debugf( NAME_Log, "[KHG] Reverb: zone %s MasterGain=%d CutoffHz=%d Delay=%d,%d,%d,%d,%d,%d Gain=%d,%d,%d,%d,%d,%d",
			Zone->GetName(), P.MasterGain, P.CutoffHz,
			P.Delay[0], P.Delay[1], P.Delay[2], P.Delay[3], P.Delay[4], P.Delay[5],
			P.Gain[0], P.Gain[1], P.Gain[2], P.Gain[3], P.Gain[4], P.Gain[5] );
	else
		debugf( NAME_Log, "[KHG] Reverb: off" );

	FScopedLock Lock( FxMutex );
	PendingGalaxyParms = P;
	GalaxyParmsPending = true;

	unguard;
}

// Keeps the real device's effects stream filled: render the loopback mix, add the echo, queue it.
void UNOpenALAudioSubsystem::PumpFxStream()
{
	guard(UNOpenALAudioSubsystem::PumpFxStream)

	{
		FScopedLock Lock( FxMutex );
		if( GalaxyParmsPending )
		{
			GalaxyParmsPending = false;
			if( !GalaxyReverbSet( GalaxyReverb, PendingGalaxyParms ) && PendingGalaxyParms.MasterGain )
				debugf( NAME_Log, "[KHG] Reverb: Galaxy refuses these values (CutoffHz above 22050), no echo" );
		}
	}

	MUSIC_AL_SCOPE;
	ALint Processed = 0, Queued = 0, State = AL_STOPPED;
	alGetSourcei( FxOutSource, AL_BUFFERS_PROCESSED, &Processed );
	alGetSourcei( FxOutSource, AL_BUFFERS_QUEUED, &Queued );
	alGetSourcei( FxOutSource, AL_SOURCE_STATE, &State );
	if( Processed > 0 )
	{
		const INT N = Min<INT>( Processed, ARRAY_COUNT( FxOutBuffers ) - NumFreeFxOutBuffers );
		alSourceUnqueueBuffers( FxOutSource, N, &FreeFxOutBuffers[NumFreeFxOutBuffers] );
		NumFreeFxOutBuffers += N;
		Queued -= N;
	}

	static short Mix[FX_FRAMES * 8];
	while( NumFreeFxOutBuffers > 0 )
	{
		alcRenderSamplesSOFT( FxDevice, Mix, FX_FRAMES );
		GalaxyReverbProcess( GalaxyReverb, Mix, FX_FRAMES, FxChannels );
		const ALuint Buf = FreeFxOutBuffers[--NumFreeFxOutBuffers];
		alBufferData( Buf, FxOutFormat, Mix, FX_FRAMES * FxChannels * sizeof( short ), OutputRate );
		alSourceQueueBuffers( FxOutSource, 1, &Buf );
		++Queued;
	}

	// Start, or restart after an underrun.
	if( Queued > 0 && State != AL_PLAYING )
		alSourcePlay( FxOutSource );

	unguard;
}

#ifdef PLATFORM_WIN32
DWORD __stdcall UNOpenALAudioSubsystem::FxThreadProc( void* Audio )
#else
void* UNOpenALAudioSubsystem::FxThreadProc( void* Audio )
#endif
{
	UNOpenALAudioSubsystem* This = (UNOpenALAudioSubsystem*)Audio;
	while( This->FxThreadRunning )
	{
		This->PumpFxStream();
		appSleep( 0.001f );
	}
	return (THREAD_RET)0;
}
#endif
