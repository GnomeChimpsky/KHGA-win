/*=============================================================================
	KHGTiming.h: M8 high frame rates. Frame pacing for the desktop main loop
	and the -khgtiming measurement mode. See KHGTiming.cpp.
=============================================================================*/

#pragma once

class UEngine;

// Frame pacing for the desktop main loop, set from KHGWin.ini [KHGWin]:
//   FrameRateLimit=N      cap the frame rate at N fps (0 = no cap; values 1-19 count as 20)
//   OriginalTiming=True   fixed game steps of 1/OriginalTimingRate s (default 60), one rendered frame
//                         per step, paced to real time
// Without either, the game ticks once per rendered frame like retail, but never with less than 1 ms of
// game time: the engine clamps each tick to at least 1 ms (ULevel::Tick), so faster frames would make
// the game run ahead of real time.
struct FKHGFramePacer
{
	DOUBLE Last;
	INT    Cap;        // FrameRateLimit, 0 = none
	INT    FixedRate;  // OriginalTiming rate, 0 = variable steps
	void   Init();
	void   Configure( INT InCap, INT InFixedRate );
	// Waits until the next frame is due and returns the game time it covers.
	DOUBLE BeginFrame();
};

// KHGWin.exe -khgtiming: run the same scripted input at several fixed tick rates and write a report.
void KHGRunTimingTest( UEngine* Engine );
