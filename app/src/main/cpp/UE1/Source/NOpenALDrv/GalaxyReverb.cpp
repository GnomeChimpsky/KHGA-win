// [KHG] Galaxy.dll 219 software reverb. Reverse-engineered from glxSetSampleReverb2 (setup at
// 0x10608ba0) and its MMX mixing loop (0x1060e49d); the integer steps below follow that code.
#include "GalaxyReverb.h"

#include <math.h>
#include <string.h>
#include <stdlib.h>

namespace
{
const int kRingSize = 32768;          // samples; Galaxy masks the position with 0x7fff
const int kRetailMixRate = 22050;     // Khg.ini OutputRate; Galaxy refuses a cutoff above its mix rate

inline int16_t Sat16( int32_t V )
{
	return (int16_t)( V > 32767 ? 32767 : V < -32768 ? -32768 : V );
}

// MSVC _ftol: truncate toward zero.
inline int32_t FtoL( double V )
{
	return (int32_t)V;
}
}

bool FGalaxyReverbParms::operator==( const FGalaxyReverbParms& O ) const
{
	return MasterGain == O.MasterGain && CutoffHz == O.CutoffHz
		&& !memcmp( Delay, O.Delay, sizeof(Delay) ) && !memcmp( Gain, O.Gain, sizeof(Gain) );
}

struct FGalaxyReverb
{
	int Rate = 44100;
	bool Active = false;
	int32_t Master = 0;        // Q15
	int32_t Offset[6] = {};    // negative sample offsets into the ring
	int16_t TapGain[6] = {};   // Q12
	int16_t LowState = 0;
	int16_t LowFeedback = 0;   // (1 - a) Q15
	int16_t LowInput = 0;      // a Q15
	int32_t Pos = 0;
	int16_t Ring[kRingSize] = {};
};

FGalaxyReverb* GalaxyReverbCreate( int Rate )
{
	FGalaxyReverb* R = new FGalaxyReverb;
	R->Rate = Rate > 0 ? Rate : 44100;
	return R;
}

void GalaxyReverbDestroy( FGalaxyReverb* R )
{
	delete R;
}

bool GalaxyReverbActive( const FGalaxyReverb* R )
{
	return R && R->Active;
}

bool GalaxyReverbSet( FGalaxyReverb* R, const FGalaxyReverbParms& P )
{
	if( !R )
		return false;
	R->Active = false;

	// UpdateReverb: MasterGain/255, CutoffHz clamped to 0..44100, Delay*0.002 s, Gain/255.
	const double Master = P.MasterGain / 255.0;
	const int Cutoff = P.CutoffHz < 0 ? 0 : P.CutoffHz > 44100 ? 44100 : P.CutoffHz;
	if( Master == 0.0 || Master <= -1.0 || Master > 1.0 || (double)kRetailMixRate < (double)Cutoff )
		return false;

	memset( R->Ring, 0, sizeof(R->Ring) );
	R->Pos = 0;
	R->LowState = 0;
	R->Master = FtoL( Master * 32767.0 );
	for( int i = 0; i < 6; i++ )
	{
		const double Delay = (float)( P.Delay[i] * 0.002 );
		const double Gain = (float)( P.Gain[i] / 255.0 );
		R->Offset[i] = FtoL( -( (double)R->Rate * Delay ) );
		R->TapGain[i] = (int16_t)FtoL( Gain * 4095.0 );
	}
	// One-pole low-pass: x = 1 - cos(w), a = sqrt(x * (x + 2)) - x.
	const double W = (double)Cutoff * (double)6.282f / (double)R->Rate;
	const double X = 1.0 - cos( W );
	const double A = (float)( sqrt( X * ( X + 2.0 ) ) - X );
	R->LowInput = (int16_t)FtoL( A * 32767.0 );
	R->LowFeedback = (int16_t)FtoL( ( 1.0 - A ) * 32767.0 );
	R->Active = true;
	return true;
}

void GalaxyReverbProcess( FGalaxyReverb* R, int16_t* S, int Frames, int Channels )
{
	if( !R || !R->Active || Channels < 2 )
		return;
	int32_t Pos = R->Pos;
	int16_t State = R->LowState;
	for( int f = 0; f < Frames; f++, S += Channels )
	{
		int32_t Sum = 0;
		for( int i = 0; i < 6; i++ )
			Sum += (int32_t)R->Ring[( Pos + R->Offset[i] ) & ( kRingSize - 1 )] * R->TapGain[i];
		const int16_t Taps = Sat16( Sum >> 12 );
		const int16_t Y = (int16_t)( ( (int32_t)State * R->LowFeedback + (int32_t)Taps * R->LowInput ) >> 15 );
		State = Y;

		const int16_t L = S[0], Rt = S[1];
		S[0] = Sat16( (int32_t)L + Y );
		S[1] = Sat16( (int32_t)Rt + Y );
		const int16_t In = Sat16( (int32_t)Sat16( (int32_t)Y + L ) + Rt );
		R->Ring[Pos] = (int16_t)( ( (int32_t)In * R->Master ) >> 15 );
		Pos = ( Pos + 1 ) & ( kRingSize - 1 );
	}
	R->Pos = Pos;
	R->LowState = State;
}
