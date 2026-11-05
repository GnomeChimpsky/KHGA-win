/*=============================================================================
	KHGDiscImport.h: copies the movies and the CD music from the game CD (desktop build).

	The retail game streams its movies from disc 2's AVI folder and plays its music as CD
	audio tracks from the same disc; the installer copies neither. KHGWin plays the movies
	from the System folder and the music from ripped track files (Music\Track02.wav ...),
	so this copies the AVI folder into System and rips the audio tracks into the music
	folder, from a CD/DVD drive (or a mounted .iso) or from a .cue/.bin or .iso image.
	When the install can't be written to, the files go to the user folder (Movies\,
	Music\), where the game looks for them too.
=============================================================================*/

#pragma once

// What the install lacks that the game CD has.
struct FKHGDiscNeeds
{
	bool Movies = false;   // no briefing movies (Brief0102.avi) in System or <UserDir>Movies
	bool Music = false;    // no file for CD track 2 in the music folder or <UserDir>Music, and no fan music pack
	bool Any() const { return Movies || Music; }
};
FKHGDiscNeeds KHGDiscCheck( const char* BaseDir, const char* UserDir, const char* IniPath );

// True when KHGWin should offer the copy at startup: something is missing, this is not a test run
// (any -khg... switch) or -nodiscprompt, and [KHGWin] DiscPrompt isn't False.
bool KHGDiscPromptWanted( const char* CmdLine, const char* BaseDir, const char* UserDir, const char* IniPath );

// Shows the "Copy from the game CD" window (modal; Owner may be NULL). AtStartup adds
// "Don't ask again at startup" and words the close button "Skip".
void KHGRunDiscImport( void* Owner, const char* BaseDir, const char* UserDir, const char* IniPath, bool AtStartup );

// -khgdiscimport=<source>: copies without a window and returns true (the caller exits with *ExitCode).
// <source> = a drive (E:), a .cue or .iso file, or "auto" (every CD/DVD drive). The result is written
// to <UserDir>KHGDiscImport.log. Returns false when the switch isn't there.
bool KHGDiscImportCommandLine( const char* CmdLine, const char* BaseDir, const char* UserDir, const char* IniPath, int* ExitCode );

// Writes what the copy did to the engine log. Call after appInit().
void KHGDiscImportLogStatus();
