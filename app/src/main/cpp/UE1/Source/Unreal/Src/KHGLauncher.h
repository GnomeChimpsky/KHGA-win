/*=============================================================================
	KHGLauncher.h: settings window shown when KHGWin.exe starts (desktop build).

	A plain Windows dialog with the retail MicroProse splash on the left and the
	settings the game's own menus can't change on the right. Play writes the
	changed keys to KHGWin.ini and starts the game; Quit exits without starting it.
=============================================================================*/

#pragma once

// True when the launcher should be shown for this run:
//   -nolauncher: never; -launcher or Shift held at startup: always;
//   test runs (any -khg... switch): never; else unless [KHGWin] ShowLauncher=False.
bool KHGLauncherWanted( const char* CmdLine, const char* IniPath );

// Shows the launcher (modal) before the engine starts. BaseDir = System folder (Khg.exe,
// OpenAL32.dll), UserDir = folder of KHGWin.ini and the saves. Returns true for Play.
bool KHGRunLauncher( const char* BaseDir, const char* UserDir, const char* IniPath );
