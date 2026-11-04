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

// Picks the folder for KHGWin.ini, KHGWin.log, saves and screenshots, and passes it to appSetUserDir().
// Call before appInit(), with the System directory (trailing separator) and the command line:
//   -portable, or a portable.txt next to the exe: the System folder itself (saves in ..\Save, like retail)
//   -userdir=DIR: DIR (relative to the System folder unless absolute); no migration
//   otherwise: <Saved Games>\Klingon Honor Guard\, set up on first run from the install (see below)
// Returns the folder, with a trailing separator.
std::string KHGDesktopChooseUserDir( const char* BaseDir, const char* CmdLine );

// Builds <UserDir><Package>.ini if it doesn't exist yet. Call after KHGDesktopChooseUserDir() and before
// appInit(). In the Saved Games folder the first run migrates the install: an existing System\KHGWin.ini is
// copied (else the ini is built from the retail defaults), the save files from the retail Save folder
// (and its Windows VirtualStore copy) are copied to <UserDir>Save, and the save slot names come along.
// Nothing in the install is changed.
void KHGDesktopPrepareConfig( const char* BaseDir, const char* Package );

// Call right after appInit(): when the user files live outside the System folder and the ini keeps the
// retail SavePath=..\Save / CachePath=..\Cache, points them into the user folder (not written to the ini).
void KHGDesktopApplyUserPaths();

// Launcher "Reset settings": renames IniPath to IniPath.bak and builds a fresh one from the retail defaults
// (Default.ini, else Khg.ini) + <Package>Defaults.ini + the port's input defaults, keeping the save slot names.
// Saves are not touched. Returns false (and leaves the old file in place) if it can't.
bool KHGDesktopResetConfig( const char* BaseDir, const char* Package, const char* IniPath );

// Writes what KHGDesktopPrepareConfig did to the engine log. Call after appInit().
void KHGDesktopLogConfigStatus();

// Merges an ini overlay into a base ini (exposed for tests): every key in Overlay replaces all
// lines of that key in the same section of Base (or is appended to it), sections missing from
// Base are appended, and keys of [WinDrv.WindowsClient] seed [NSDLDrv.NSDLClient] when absent.
std::string KHGMergeIni( const std::string& Base, const std::string& Overlay );
