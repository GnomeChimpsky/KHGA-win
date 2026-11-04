// [KHG] The retail Galaxy.dll (build 219) software reverb, rebuilt sample for sample.
//
// Galaxy's UGalaxyAudioSubsystem::UpdateReverb fills this from the listener's zone when
// UseReverb is on and the zone has bReverbZone, else with zeros; glxSetSampleReverb2 then
// (re)starts the echo with an empty delay line whenever the values change, and turns it off
// when MasterGain is 0. The echo is mono: a 32768-sample ring fed with (L + R + echo) * MasterGain,
// read by six taps (Delay[i] * 2 ms, Gain[i] / 255), summed, low-passed at CutoffHz, and added
// to both front channels. Music is mixed after it, so it never echoes. See docs/Audio.md.
#pragma once

#include <stdint.h>

struct FGalaxyReverbParms
{
	int32_t MasterGain = 0;  // ZoneInfo.MasterGain, 0..255; 0 = no echo
	int32_t CutoffHz = 0;    // ZoneInfo.CutoffHz
	uint8_t Delay[6] = {};   // ZoneInfo.Delay[i], in 2 ms steps
	uint8_t Gain[6] = {};    // ZoneInfo.Gain[i], 0..255

	bool operator==( const FGalaxyReverbParms& O ) const;
	bool operator!=( const FGalaxyReverbParms& O ) const { return !( *this == O ); }
};

struct FGalaxyReverb;

// Rate is the mix rate of the buffers given to GalaxyReverbProcess.
FGalaxyReverb* GalaxyReverbCreate( int Rate );
void GalaxyReverbDestroy( FGalaxyReverb* R );

// Like glxSetSampleReverb2: restarts the echo with an empty delay line, or turns it off.
// Returns false (echo off) for values Galaxy refuses: MasterGain 0, or CutoffHz above Galaxy's
// mix rate (22050 Hz in retail).
bool GalaxyReverbSet( FGalaxyReverb* R, const FGalaxyReverbParms& P );
bool GalaxyReverbActive( const FGalaxyReverb* R );

// Adds the echo to an interleaved 16-bit buffer in place. Channels 0 and 1 are front left/right.
void GalaxyReverbProcess( FGalaxyReverb* R, int16_t* Samples, int Frames, int Channels );
