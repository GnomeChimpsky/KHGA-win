/*=============================================================================
	KHGDesktopConfig.h: first-run ini for the desktop build.

	The desktop launcher (KHGWin.exe) keeps its own KHGWin.ini so it never touches the
	retail Khg.ini. On first run that file is built from the user's retail defaults
	(Default.ini, or Khg.ini if there is no Default.ini) with only the driver settings
	replaced by the ones in KHGWinDefaults.ini, which ships next to the exe. Gameplay,
	input and menu settings therefore start out exactly as the retail install has them.
=============================================================================*/

#pragma once

#include <string>

// Builds <BaseDir><Package>.ini if it doesn't exist yet. Call before appInit(), with the
// System directory (trailing separator) and the launcher's package name.
void KHGDesktopPrepareConfig( const char* BaseDir, const char* Package );

// Writes what KHGDesktopPrepareConfig did to the engine log. Call after appInit().
void KHGDesktopLogConfigStatus();

// Merges an ini overlay into a base ini (exposed for tests): every key in Overlay replaces all
// lines of that key in the same section of Base (or is appended to it), sections missing from
// Base are appended, and keys of [WinDrv.WindowsClient] seed [NSDLDrv.NSDLClient] when absent.
std::string KHGMergeIni( const std::string& Base, const std::string& Overlay );
