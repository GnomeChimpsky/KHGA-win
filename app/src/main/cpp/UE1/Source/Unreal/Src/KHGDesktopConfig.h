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
#include <vector>

// Picks the folder for KHGWin.ini, KHGWin.log, saves and screenshots, and passes it to appSetUserDir().
// Call before appInit(), with the exe's folder (trailing separator) and the command line:
//   -portable, or a portable.txt next to the exe: the exe's folder. When that is the install's System
//            folder, saves go to ..\Save like retail; when the exe runs from a folder of its own (see
//            KHGDesktopChooseGameDir), the ini, log and saves stay next to the exe, set up like Saved Games.
//   -userdir=DIR: DIR (relative to the exe's folder unless absolute); no migration
//   otherwise: <Saved Games>\Klingon Honor Guard\, set up on first run from the install (see below)
// Returns the folder, with a trailing separator.
std::string KHGDesktopChooseUserDir( const char* BaseDir, const char* CmdLine );

// Finds the retail install and, when it isn't the exe's own folder, passes its System folder to
// appSetBaseDir(). Call after KHGDesktopChooseUserDir() and before KHGDesktopPrepareConfig(). In order:
//   -gamedir=DIR for this run (the install folder or its System folder)
//   the exe's folder, if it is the install's System folder (unless -pickgamedir)
//   [KHGWin] GameDir in the ini, the folder found or chosen before (unless -pickgamedir)
//   a System folder near the exe: in any of its parent folders or the folders directly inside them
//   else a folder dialog (not with -nogamedirdialog)
// A folder that was found or chosen is saved as GameDir by KHGDesktopPrepareConfig(). Returns false when
// no install was found or chosen (the caller quits).
bool KHGDesktopChooseGameDir( const char* CmdLine );

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

// Mods (exposed for tests): returns PortIni with the mod keys of RetailIni (Khg.ini) brought over. A key
// (DefaultGame, DefaultServerGame, ServerPackages[], Paths[], the server menu's game types, the player model
// menus, any key of a non-retail package's section) is copied when its Khg.ini value differs from the one
// recorded in PortIni's [KHGWin.ModSync] (or from DefaultIni when there is no record yet), and removed when
// it left Khg.ini. The record is rewritten to what Khg.ini says now. Log gets one line per change.
// KHGDesktopPrepareConfig() and KHGDesktopResetConfig() run it on KHGWin.ini unless [KHGWin] ModSync=False.
std::string KHGSyncModKeys( const std::string& PortIni, const std::string& RetailIni, const std::string& DefaultIni,
	std::vector<std::string>* Log );
