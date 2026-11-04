/*=============================================================================
	KHGCrash.h: crash reports for the desktop build (M9).

	A crash (an access violation or other Windows exception nobody handles, abort(), or a C++
	terminate) and a critical engine error (appError, shown as "Critical Error") write a report to
	<user folder>Crashes\ (Saved Games\Klingon Honor Guard\Crashes unless portable):
	  KHGWin-<date>-<time>.txt   what happened: build, Windows version, exception, the call stack as
	                             module+offset (desktop/scripts/symbolize-crash.sh turns those into
	                             function names and lines with the build's debug KHGWin.exe), the
	                             engine's guard history and the end of the log
	  KHGWin-<date>-<time>.dmp   minidump, for a debugger
	  KHGWin-<date>-<time>.log   copy of KHGWin.log at the time of the crash
	and then tells the user where they are.
=============================================================================*/

#pragma once

// Version string of this build (git describe), e.g. "v0.9.0" or "v0.9.0-3-gabc1234".
const char* KHGBuildVersion();

// Installs the crash handlers. Call once, as early as possible (they write to appUserDir(), so after
// KHGDesktopChooseUserDir() for reports in the right folder).
void KHGCrashInstall();

// Writes a report for a critical engine error (HandleError). Reason = the error text and guard history.
// Returns the report's .txt path, or an empty string.
const char* KHGCrashReportError( const char* Reason );
