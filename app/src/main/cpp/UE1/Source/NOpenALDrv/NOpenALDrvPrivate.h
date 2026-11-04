/*------------------------------------------------------------------------------------
	Dependencies.
------------------------------------------------------------------------------------*/

#include "AL/al.h"
#include "AL/alc.h"
#include "xmp.h"
#include "Engine.h"
#ifdef KHG_GALAXYREVERB
#include "GalaxyReverb.h"
#endif

/*------------------------------------------------------------------------------------
	OpenAL audio subsystem private definitions.
------------------------------------------------------------------------------------*/

#define MAX_SOURCES 64

#define NUM_MUSIC_BUFFERS 4

#define INVALID_SOURCE ((ALuint)-1)
#define INVALID_BUFFER ((ALuint)-1)

#define SOUND_SLOT_IS( Id, Slot ) ( ( (Id) & 14 ) == (Slot) * 2 )
#define AMBIENT_SOUND_ID( ActorIndex ) ( (ActorIndex) * 16 + SLOT_Ambient * 2 )

#define DEFAULT_OUTPUT_RATE 44100

#define STREAM_BUFSIZE 32768

// World scale related constants, same as in ALAudio 2.4.7.
#define DISTANCE_SCALE 0.023255814f
#define ROLLOFF_FACTOR 1.1f
#define DESPATIALIZE_FACTOR 0.1f

class DLL_EXPORT UNOpenALAudioSubsystem : public UAudioSubsystem
{
	DECLARE_CLASS_WITHOUT_CONSTRUCT(UNOpenALAudioSubsystem, UAudioSubsystem, CLASS_Config)

	// Options
	char DeviceName[256];
	INT OutputRate;
	BYTE MasterVolume;
	BYTE SoundVolume;
	BYTE MusicVolume;
	BYTE MusicInterpolation;
	FLOAT AmbientFactor;
	FLOAT DopplerFactor;
	UBOOL UseReverb;
	UBOOL UseHRTF;
#ifdef KHG_CDMUSIC
	UBOOL UseCDMusic;
	char CDMusicPath[256];
#endif
	UBOOL UseDigitalMusic; // [KHG] Galaxy's switch for module (UMX) music

	// Constructors.
	static void InternalClassInitializer( UClass* Class );
	UNOpenALAudioSubsystem();

	// UObject interface.
	virtual void Destroy() override;
	virtual void PostEditChange() override;
	virtual void ShutdownAfterError() override;

	// UAudioSubsystem interface.
	virtual UBOOL Init() override;
	virtual void SetViewport( UViewport* Viewport ) override;
	virtual UBOOL Exec( const char* Cmd, FOutputDevice* Out = GSystem ) override;
	virtual void Update( FPointRegion Region, FCoords& Listener ) override;
	virtual void RegisterMusic( UMusic* Music ) override;
	virtual void RegisterSound( USound* Music ) override;
	virtual void UnregisterSound( USound* Sound ) override;
	virtual void UnregisterMusic( UMusic* Music ) override;
	virtual UBOOL PlaySound( AActor* Actor, INT Id, USound* Sound, FVector Location, FLOAT Volume, FLOAT Radius, FLOAT Pitch ) override;
	virtual void NoteDestroy( AActor* Actor );
	virtual UBOOL GetLowQualitySetting() override { return false; };
#if defined(PLATFORM_ANDROID) || defined(UNREAL_ANDROID) || defined(__ANDROID__)
	void AndroidSetSuspended( UBOOL bSuspended );
#endif

	// Internals.
private:
	UViewport* Viewport;
	ALCdevice* Device;
	ALCcontext* Ctx;
	ALuint Sources[MAX_SOURCES];
	TArray<ALuint> Buffers;
	INT NextId;
	FCoords ListenerCoords;

	ALuint ReverbEffect;
	ALuint ReverbSlot;
	UBOOL ReverbOn;
	UBOOL EfxReverb = false; // EFX reverb (Android); the desktop build uses Galaxy's echo instead
	AZoneInfo* ReverbZone;

#ifdef KHG_GALAXYREVERB
	// [KHG] Sound effects are mixed on an OpenAL Soft loopback device, Galaxy's zone echo is added
	// to that mix (GalaxyReverb.cpp), and the result is streamed to the real device. Music and the
	// CD track stay on the real device, as Galaxy mixes them after its reverb.
	ALCdevice* FxDevice = nullptr;
	ALCcontext* FxCtx = nullptr;        // loopback context: effect sources and buffers
	ALuint FxOutSource = INVALID_SOURCE; // real context: streams the effects mix
	ALuint FxOutBuffers[8];
	ALuint FreeFxOutBuffers[8];
	INT NumFreeFxOutBuffers = 0;
	INT FxChannels = 2;
	ALenum FxOutFormat = 0;
	struct FGalaxyReverb* GalaxyReverb = nullptr;
	FGalaxyReverbParms GalaxyParms;     // what the echo was last set to
	FGalaxyReverbParms PendingGalaxyParms;
	UBOOL GalaxyParmsPending = false;
	FMutex FxMutex { "FxMutex" };
	volatile UBOOL FxThreadRunning = false;
	UTHREAD FxThread = nullptr;
	UBOOL InitFxLoopback();
	void ShutdownFxLoopback();
	void UpdateGalaxyReverb();
	void PumpFxStream();
	#ifdef PLATFORM_WIN32
	static DWORD __stdcall FxThreadProc( void* Audio );
	#else
	static void* FxThreadProc( void* Audio );
	#endif
#endif
	// Context of the music and CD sources (the real device), see FMusicCtxScope.
	ALCcontext* MusicALCtx() const { return Ctx; }

	xmp_context MusicCtx;
	UMusic* Music;
	FLOAT MusicFade;
	DOUBLE MusicTime;
	BYTE MusicSection;
	ALuint MusicSource;
	UBOOL MusicIsPlaying = false;
	UBOOL MusicIsLoaded = false;
#if defined(PLATFORM_ANDROID) || defined(UNREAL_ANDROID) || defined(__ANDROID__)
	volatile UBOOL AndroidSuspended = false;
#endif

	BYTE MusicBufferData[STREAM_BUFSIZE];
	ALuint MusicBuffers[NUM_MUSIC_BUFFERS];
	ALuint FreeMusicBuffers[NUM_MUSIC_BUFFERS];
	INT NumFreeMusicBuffers;

#ifdef KHG_CDMUSIC
	// [KHG] Ripped CD tracks: PlayerPawn.CdTrack played from a file, like Galaxy plays it from the CD.
	BYTE CDTrack = 255;                       // track Galaxy would be playing, 255 = none
	struct FCDMusicStream* CDStream = nullptr; // decoder, null when the track ended or has no file
	ALuint CDSource = INVALID_SOURCE;
	ALuint CDBuffers[NUM_MUSIC_BUFFERS];
	ALuint FreeCDBuffers[NUM_MUSIC_BUFFERS];
	INT NumFreeCDBuffers = 0;
	short CDBufferData[STREAM_BUFSIZE / sizeof(short)];
	void StartCDTrack( BYTE Track );
	void StopCDTrack();
	void UpdateCDBuffers();
#endif

	volatile UBOOL MusicThreadRunning;
	FMutex MusicMutex { "MusicMutex" };
	UTHREAD MusicThread;

	enum ENVoiceOp
	{
		NVOP_None,
		NVOP_Play,
		NVOP_Stop,
		NVOP_Pause,
	};

	struct FNVoice
	{
		ALuint Buffer = INVALID_BUFFER;
		AActor* Actor;
		INT Id;
		USound* Sound;
		FVector Location;
		FVector Velocity;
		FLOAT Volume;
		FLOAT Radius;
		FLOAT Pitch;
		FLOAT Priority;
		UBOOL Looping;
		UBOOL BufferChanged = false;
	} Voices[MAX_SOURCES];

	void InitReverbEffect();
	void UpdateReverb( FPointRegion& Region );
	void UpdateVoice( INT Num, const ENVoiceOp Op = NVOP_None );
	void StopVoice( INT Num );
	void PlayMusic();
	void StopMusic();

	void UpdateMusicBuffers();
	void ClearMusicBuffers();

	void StartMusicThread();
	void StopMusicThread();

	inline FLOAT GetVoicePriority( const FVector& Location, FLOAT Volume, FLOAT Radius )
	{
		if( Radius && Viewport->Actor )
			return Volume * ( 1.f - (Location - Viewport->Actor->Location).Size() / Radius );
		else
			return Volume;
	}

	#ifdef PLATFORM_WIN32
	static DWORD __stdcall MusicThreadProc( void* Audio );
	#else
	static void* MusicThreadProc( void* Audio );
	#endif
};
