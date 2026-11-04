/*=============================================================================
	KHGDesktopConfig.cpp: first-run ini for the desktop build. See KHGDesktopConfig.h.
=============================================================================*/

#include <windows.h>   // before Core.h, like SDLLaunch.cpp
#include <shlobj.h>
#include <shobjidl.h>  // IFileOpenDialog (game folder picker)
#include "KHGDesktopConfig.h"
#include <stdlib.h>

#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <vector>

#include "Core.h"

namespace
{

struct FIniLine
{
	std::string Text;    // the line without its line ending
};

struct FIniSection
{
	std::string Name;    // without brackets; empty for lines before the first section
	std::vector<FIniLine> Lines;
};

static std::string Trim( const std::string& S )
{
	size_t B = 0, E = S.size();
	while( B < E && ( S[B] == ' ' || S[B] == '\t' ) ) B++;
	while( E > B && ( S[E-1] == ' ' || S[E-1] == '\t' || S[E-1] == '\r' ) ) E--;
	return S.substr( B, E - B );
}

static bool SameName( const std::string& A, const std::string& B )
{
	return strcasecmp( A.c_str(), B.c_str() ) == 0;
}

// Key of a "Key=Value" line, or empty for comments/blank lines.
static std::string KeyOf( const std::string& Line )
{
	const std::string T = Trim( Line );
	if( T.empty() || T[0] == ';' || T[0] == '[' )
		return std::string();
	const size_t Eq = T.find( '=' );
	return Eq == std::string::npos ? std::string() : Trim( T.substr( 0, Eq ) );
}

static std::vector<FIniSection> Parse( const std::string& Text )
{
	std::vector<FIniSection> Sections( 1 );
	size_t Pos = 0;
	while( Pos < Text.size() )
	{
		size_t End = Text.find( '\n', Pos );
		if( End == std::string::npos )
			End = Text.size();
		std::string Line = Text.substr( Pos, End - Pos );
		if( !Line.empty() && Line.back() == '\r' )
			Line.pop_back();
		Pos = End + 1;

		const std::string T = Trim( Line );
		if( T.size() >= 2 && T[0] == '[' && T.back() == ']' )
		{
			Sections.push_back( FIniSection() );
			Sections.back().Name = T.substr( 1, T.size() - 2 );
		}
		else
		{
			Sections.back().Lines.push_back( { Line } );
		}
	}
	return Sections;
}

static FIniSection* Find( std::vector<FIniSection>& Sections, const std::string& Name )
{
	for( FIniSection& S : Sections )
		if( !S.Name.empty() && SameName( S.Name, Name ) )
			return &S;
	return nullptr;
}

static bool HasKey( const FIniSection& S, const std::string& Key )
{
	for( const FIniLine& L : S.Lines )
		if( SameName( KeyOf( L.Text ), Key ) )
			return true;
	return false;
}

// Inserts Line after the last non-blank line of the section.
static void Append( FIniSection& S, const std::string& Line )
{
	size_t At = S.Lines.size();
	while( At > 0 && Trim( S.Lines[At-1].Text ).empty() )
		At--;
	S.Lines.insert( S.Lines.begin() + At, { Line } );
}

static std::string Write( const std::vector<FIniSection>& Sections )
{
	std::string Out;
	for( const FIniSection& S : Sections )
	{
		if( !S.Name.empty() )
		{
			// Keep a blank line between sections.
			if( !Out.empty() && ( Out.size() < 4 || Out.compare( Out.size() - 4, 4, "\r\n\r\n" ) != 0 ) )
				Out += "\r\n";
			Out += "[" + S.Name + "]\r\n";
		}
		for( const FIniLine& L : S.Lines )
			Out += L.Text + "\r\n";
	}
	return Out;
}

static bool ReadText( const std::string& Path, std::string& Out )
{
	FILE* F = fopen( Path.c_str(), "rb" );
	if( !F )
		return false;
	char Buf[16384];
	size_t N;
	Out.clear();
	while( ( N = fread( Buf, 1, sizeof(Buf), F ) ) > 0 )
		Out.append( Buf, N );
	fclose( F );
	return true;
}

static bool WriteText( const std::string& Path, const std::string& Text )
{
	FILE* F = fopen( Path.c_str(), "wb" );
	if( !F )
		return false;
	const bool Ok = fwrite( Text.data(), 1, Text.size(), F ) == Text.size();
	return fclose( F ) == 0 && Ok;
}

static std::string GConfigStatus;

// Input defaults that differ from the retail ini, applied when KHGWin.ini is created and once to an
// existing KHGWin.ini whose [KHGWin] InputDefaults is lower than the step's version. Only the listed
// keys change; all other bindings stay as in the retail ini.
//
// 1: WASD movement with mouse look (Galvatron, 2026-11-01). Retail Default.ini binds S to
//    "Axis aUp" (swim/fly up); Space (Jump) and C (Duck) already do that.
// 2: twin-stick gamepad layout (M5, Galvatron's choice 2026-11-01). Retail ships UseJoystick=False and
//    Joy bindings made for 1998 pads (A fire, triggers on inventory, no menu button). SDL buttons:
//    A Joy1, B Joy2, X Joy3, Y Joy4, Back Joy5, Guide Joy6, Start Joy7, stick clicks Joy8/Joy9,
//    LB Joy10, RB Joy11, LT Joy12, RT Joy13, d-pad JoyPov*; left stick JoyX/JoyY, right stick JoyU/JoyV.
//    Right stick full tilt turns at 2x the keyboard turn rate (aBaseX 6000 at ScaleRUV=100) and looks
//    up/down at 1.5x the keyboard look rate.
struct FKHGInputDefaults { int Version; const char* Ini; };
static const FKHGInputDefaults kInputDefaults[] =
{
	{ 1,
	"[Engine.Input]\r\n"
	"W=MoveForward\r\n"
	"S=MoveBackward\r\n"
	"A=StrafeLeft\r\n"
	"D=StrafeRight\r\n"
	"[Engine.PlayerPawn]\r\n"
	"bAlwaysMouseLook=True\r\n" },
	{ 2,
	"[Engine.Input]\r\n"
	"Joy1=Jump\r\n"
	"Joy2=Duck\r\n"
	"Joy3=Grab\r\n"
	"Joy4=ReloadWeapon\r\n"
	"Joy5=ChangeHud\r\n"
	"Joy6=\r\n"
	"Joy7=ShowMenu\r\n"
	"Joy8=Walking\r\n"
	"Joy9=CenterView\r\n"
	"Joy10=PrevWeapon\r\n"
	"Joy11=NextWeapon\r\n"
	"Joy12=AltFire\r\n"
	"Joy13=Fire\r\n"
	"JoyX=Axis aStrafe Speed=2\r\n"
	"JoyY=Axis aBaseY Speed=2\r\n"
	"JoyU=Axis aBaseX Speed=5\r\n"
	"JoyV=Axis aLookUp Speed=-2.5\r\n"
	"JoyPovUp=InventoryActivate\r\n"
	"JoyPovDown=\r\n"
	"JoyPovLeft=InventoryPrevious\r\n"
	"JoyPovRight=InventoryNext\r\n"
	"[NSDLDrv.NSDLClient]\r\n"
	"UseJoystick=True\r\n"
	"ScaleXYZ=1000.0\r\n"
	"ScaleRUV=100.0\r\n"
	"DeadZoneXYZ=0.2\r\n"
	"DeadZoneRUV=0.15\r\n" },
};
static const int kInputDefaultsVersion = 2;

// Merges every step newer than FromVersion into Ini and records the new version.
static std::string ApplyInputDefaults( std::string Ini, int FromVersion );

static int InputDefaultsIn( const std::string& Ini )
{
	for( const FIniSection& S : Parse( Ini ) )
		if( SameName( S.Name, "KHGWin" ) )
			for( const FIniLine& L : S.Lines )
				if( SameName( KeyOf( L.Text ), "InputDefaults" ) )
				{
					const size_t Eq = L.Text.find( '=' );
					return Eq == std::string::npos ? 0 : atoi( L.Text.c_str() + Eq + 1 );
				}
	return 0;
}

} // namespace

std::string KHGMergeIni( const std::string& Base, const std::string& Overlay );

namespace
{
static std::string ApplyInputDefaults( std::string Ini, int FromVersion )
{
	for( const FKHGInputDefaults& D : kInputDefaults )
		if( D.Version > FromVersion )
			Ini = KHGMergeIni( Ini, D.Ini );
	char Mark[64];
	snprintf( Mark, sizeof(Mark), "[KHGWin]\r\nInputDefaults=%d\r\n", kInputDefaultsVersion );
	return KHGMergeIni( Ini, Mark );
}
} // namespace

std::string KHGMergeIni( const std::string& Base, const std::string& Overlay )
{
	std::vector<FIniSection> Out = Parse( Base );

	// Window settings: carry the retail WinDrv client settings over to the SDL client, so window
	// size, fullscreen and brightness start as the user had them. Keys NSDLClient doesn't know are
	// ignored by the engine.
	if( FIniSection* Win = Find( Out, "WinDrv.WindowsClient" ) )
	{
		const std::vector<FIniLine> WinLines = Win->Lines;
		FIniSection* Sdl = Find( Out, "NSDLDrv.NSDLClient" );
		if( !Sdl )
		{
			Out.push_back( FIniSection() );
			Out.back().Name = "NSDLDrv.NSDLClient";
			Sdl = &Out.back();
		}
		for( const FIniLine& L : WinLines )
		{
			const std::string Key = KeyOf( L.Text );
			if( !Key.empty() && !HasKey( *Sdl, Key ) )
				Append( *Sdl, Trim( L.Text ) );
		}
	}

	for( const FIniSection& O : Parse( Overlay ) )
	{
		if( O.Name.empty() )
			continue;
		FIniSection* Target = Find( Out, O.Name );
		if( !Target )
		{
			Out.push_back( FIniSection() );
			Out.back().Name = O.Name;
			Target = &Out.back();
		}
		// Drop every existing line of the overlay's keys first, so array-style keys are replaced as a whole.
		for( const FIniLine& L : O.Lines )
		{
			const std::string Key = KeyOf( L.Text );
			if( Key.empty() )
				continue;
			for( size_t i = 0; i < Target->Lines.size(); )
			{
				if( SameName( KeyOf( Target->Lines[i].Text ), Key ) )
					Target->Lines.erase( Target->Lines.begin() + i );
				else
					i++;
			}
		}
		for( const FIniLine& L : O.Lines )
			if( !KeyOf( L.Text ).empty() )
				Append( *Target, Trim( L.Text ) );
	}

	return Write( Out );
}

namespace
{

// UDM_ExeFolder: portable, but KHGWin.exe runs from its own folder (not the install's System folder), so the
// user files go next to the exe and, like the Saved Games folder, are set up from the install on first run.
enum EUserDirMode { UDM_Portable, UDM_SavedGames, UDM_CommandLine, UDM_ExeFolder };
static EUserDirMode GUserDirMode = UDM_Portable;
static std::string GGameDirStatus, GGameDirToSave;

// First run in a folder of its own (Saved Games, or next to an exe outside the install): bring the install's
// ini and saves along.
static bool MigratesFromInstall()
{
	return GUserDirMode == UDM_SavedGames || GUserDirMode == UDM_ExeFolder;
}
static std::string GUserDirStatus;
static std::vector<std::string> GMigrationLog;

static bool FileExists( const std::string& Path )
{
	const DWORD A = GetFileAttributesA( Path.c_str() );
	return A != INVALID_FILE_ATTRIBUTES && !( A & FILE_ATTRIBUTE_DIRECTORY );
}

static bool DirExists( const std::string& Path )
{
	const DWORD A = GetFileAttributesA( Path.c_str() );
	return A != INVALID_FILE_ATTRIBUTES && ( A & FILE_ATTRIBUTE_DIRECTORY );
}

static bool IsAbsolute( const std::string& P )
{
	return ( P.size() >= 2 && P[1] == ':' ) || ( !P.empty() && ( P[0] == '\\' || P[0] == '/' ) );
}

static std::string WithSlash( std::string P )
{
	if( !P.empty() && P.back() != '\\' && P.back() != '/' )
		P += '\\';
	return P;
}

// Full path with ".." resolved; Rel is relative to Base unless absolute.
static std::string FullPath( const std::string& Base, const std::string& Rel )
{
	const std::string In = IsAbsolute( Rel ) ? Rel : Base + Rel;
	char Out[MAX_PATH * 2];
	const DWORD N = GetFullPathNameA( In.c_str(), sizeof(Out), Out, NULL );
	return ( N > 0 && N < sizeof(Out) ) ? std::string( Out ) : In;
}

// Wide path -> ANSI path the engine's char file functions can open; falls back to the 8.3 short name
// (the folder must exist) when the user name has characters outside the ANSI code page.
static bool ToAnsiPath( const std::wstring& W, std::string& Out )
{
	for( int Pass = 0; Pass < 2; Pass++ )
	{
		std::wstring Path = W;
		if( Pass == 1 )
		{
			wchar_t Short[MAX_PATH * 2];
			const DWORD N = GetShortPathNameW( W.c_str(), Short, MAX_PATH * 2 );
			if( N == 0 || N >= MAX_PATH * 2 )
				return false;
			Path = Short;
		}
		BOOL Lossy = FALSE;
		char Buf[MAX_PATH * 2];
		const int N = WideCharToMultiByte( CP_ACP, WC_NO_BEST_FIT_CHARS, Path.c_str(), -1, Buf, sizeof(Buf), NULL, &Lossy );
		if( N > 0 && !Lossy )
		{
			Out = Buf;
			return true;
		}
	}
	return false;
}

// <Saved Games>\Klingon Honor Guard\, created if needed.
static bool SavedGamesDir( std::string& Out )
{
	static const GUID SavedGames = { 0x4c5c32ff, 0xbb9d, 0x43b0, { 0xb5, 0xb4, 0x2d, 0x72, 0xe5, 0x4e, 0xaa, 0xa4 } };
	PWSTR Known = NULL;
	std::wstring Dir;
	if( SUCCEEDED( SHGetKnownFolderPath( SavedGames, KF_FLAG_CREATE, NULL, &Known ) ) && Known )
		Dir = Known;
	CoTaskMemFree( Known );
	if( Dir.empty() )
	{
		const wchar_t* Profile = _wgetenv( L"USERPROFILE" );
		if( !Profile )
			return false;
		Dir = std::wstring( Profile ) + L"\\Saved Games";
		CreateDirectoryW( Dir.c_str(), NULL );
	}
	Dir += L"\\Klingon Honor Guard";
	CreateDirectoryW( Dir.c_str(), NULL );
	if( !ToAnsiPath( Dir, Out ) || !DirExists( Out ) )
		return false;
	Out = WithSlash( Out );
	return true;
}

// Where Windows redirects writes a non-elevated program makes under Program Files:
// %LOCALAPPDATA%\VirtualStore\<path without the drive>. Empty if not available.
static std::string VirtualStorePath( const std::string& Full )
{
	const char* Local = getenv( "LOCALAPPDATA" );
	if( !Local || Full.size() < 3 || Full[1] != ':' )
		return std::string();
	return WithSlash( Local ) + "VirtualStore\\" + Full.substr( 3 );
}

// Value of Section/Key in an ini text, or empty.
static std::string IniValue( const std::string& Ini, const char* Section, const char* Key )
{
	for( const FIniSection& S : Parse( Ini ) )
		if( SameName( S.Name, Section ) )
			for( const FIniLine& L : S.Lines )
				if( SameName( KeyOf( L.Text ), Key ) )
				{
					const size_t Eq = L.Text.find( '=' );
					return Trim( L.Text.substr( Eq + 1 ) );
				}
	return std::string();
}

static bool IsRetailDefaultPath( const std::string& P, const char* Name )
{
	std::string N = Trim( P );
	for( char& C : N )
		if( C == '/' )
			C = '\\';
	while( !N.empty() && N.back() == '\\' )
		N.pop_back();
	return SameName( N, std::string( "..\\" ) + Name );
}

// The [Klingons.KlingonMenuSlot] section (the save slot names the load/save menus show) as an ini
// overlay, or empty when Ini has none.
static std::string SlotSection( const std::string& Ini )
{
	for( const FIniSection& S : Parse( Ini ) )
		if( SameName( S.Name, "Klingons.KlingonMenuSlot" ) )
		{
			std::string Out = "[Klingons.KlingonMenuSlot]\r\n";
			bool Any = false;
			for( const FIniLine& L : S.Lines )
				if( !KeyOf( L.Text ).empty() )
				{
					Out += Trim( L.Text ) + "\r\n";
					Any = true;
				}
			return Any ? Out : std::string();
		}
	return std::string();
}

// The retail Khg.ini as the retail game sees it: its VirtualStore copy wins if there is one.
static bool ReadRetailIni( const std::string& BaseDir, std::string& Out, std::string& Where )
{
	static const char* Names[] = { "Khg.ini", "KHG.INI", "khg.ini" };
	for( const char* Name : Names )
	{
		const std::string Full = FullPath( BaseDir, Name );
		const std::string Virtual = VirtualStorePath( Full );
		if( !Virtual.empty() && ReadText( Virtual, Out ) )
		{
			Where = Virtual;
			return true;
		}
		if( ReadText( Full, Out ) )
		{
			Where = Full;
			return true;
		}
	}
	return false;
}

// Copies every *.usa in From to To that To doesn't have yet. Returns how many were copied.
static int CopySaves( const std::string& From, const std::string& To )
{
	int Copied = 0;
	WIN32_FIND_DATAA Found;
	HANDLE H = FindFirstFileA( ( WithSlash( From ) + "*.usa" ).c_str(), &Found );
	if( H == INVALID_HANDLE_VALUE )
		return 0;
	do
	{
		if( Found.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
			continue;
		if( CopyFileA( ( WithSlash( From ) + Found.cFileName ).c_str(), ( WithSlash( To ) + Found.cFileName ).c_str(), TRUE ) )
			Copied++;
	} while( FindNextFileA( H, &Found ) );
	FindClose( H );
	return Copied;
}

// First run in the Saved Games folder: brings the saves of the install along. Returns the ini with the
// slot names added when it had none.
static std::string MigrateSaves( const std::string& BaseDir, const std::string& UserDir, std::string Ini, const std::string& OldPortIni )
{
	std::string RetailIni, RetailWhere;
	const bool HaveRetail = ReadRetailIni( BaseDir, RetailIni, RetailWhere );

	// Save folders the install used: the retail one (and its VirtualStore copy) and the port's old one.
	std::vector<std::string> From;
	auto AddFrom = [&]( const std::string& SavePath )
	{
		if( SavePath.empty() )
			return;
		const std::string Full = FullPath( BaseDir, SavePath );
		for( const std::string& Dir : { VirtualStorePath( Full ), Full } )
		{
			bool Known = Dir.empty();
			for( const std::string& F : From )
				Known = Known || SameName( F, Dir );
			if( !Known && DirExists( Dir ) )
				From.push_back( Dir );
		}
	};
	AddFrom( HaveRetail ? ( IniValue( RetailIni, "Core.System", "SavePath" ).empty() ? std::string( "..\\Save" ) : IniValue( RetailIni, "Core.System", "SavePath" ) ) : std::string( "..\\Save" ) );
	AddFrom( IniValue( OldPortIni, "Core.System", "SavePath" ) );

	const std::string Dest = UserDir + "Save";
	CreateDirectoryA( Dest.c_str(), NULL );
	int Total = 0;
	for( const std::string& Dir : From )
	{
		const int N = CopySaves( Dir, Dest );
		Total += N;
		GMigrationLog.push_back( "copied " + std::to_string( N ) + " save file(s) from " + Dir + " to " + Dest );
	}
	if( From.empty() )
		GMigrationLog.push_back( "no Save folder found in the install, nothing to copy" );

	// Slot names live in the ini. The port's own old ini wins (its saves shared the retail folder).
	if( Total > 0 && SlotSection( Ini ).empty() )
	{
		std::string Slots = SlotSection( OldPortIni ), Src = "the old System\\KHGWin.ini";
		if( Slots.empty() && HaveRetail )
		{
			Slots = SlotSection( RetailIni );
			Src = RetailWhere;
		}
		if( !Slots.empty() )
		{
			Ini = KHGMergeIni( Ini, Slots );
			GMigrationLog.push_back( "save slot names taken from " + Src );
		}
	}
	return Ini;
}

} // namespace

std::string KHGDesktopChooseUserDir( const char* InExeDir, const char* CmdLine )
{
	const std::string BaseDir = InExeDir ? InExeDir : "";
	std::string Dir;
	char Arg[1024] = "";
	if( ParseParam( CmdLine, "PORTABLE" ) || FileExists( BaseDir + "portable.txt" ) )
	{
		GUserDirMode = UDM_Portable;
		GUserDirStatus = ParseParam( CmdLine, "PORTABLE" ) ? "portable (-portable): ini, log and saves next to KHGWin.exe"
		                                                 : "portable (portable.txt): ini, log and saves next to KHGWin.exe";
	}
	else if( Parse( CmdLine, "USERDIR=", Arg, ARRAY_COUNT(Arg) ) && Arg[0] )
	{
		Dir = WithSlash( FullPath( BaseDir, Arg ) );
		CreateDirectoryA( Dir.c_str(), NULL );
		if( DirExists( Dir ) )
		{
			GUserDirMode = UDM_CommandLine;
			GUserDirStatus = "user folder " + Dir + " (-userdir)";
		}
		else
		{
			GUserDirMode = UDM_Portable;
			GUserDirStatus = "WARNING: could not create -userdir folder " + Dir + "; running portable";
			Dir.clear();
		}
	}
	else if( SavedGamesDir( Dir ) )
	{
		GUserDirMode = UDM_SavedGames;
		GUserDirStatus = "user folder " + Dir;
	}
	else
	{
		GUserDirMode = UDM_Portable;
		GUserDirStatus = "WARNING: no usable Saved Games folder; running portable";
		Dir.clear();
	}
	if( GUserDirMode == UDM_Portable )
		Dir = BaseDir;
	appSetUserDir( GUserDirMode == UDM_Portable ? "" : Dir.c_str() );
	return Dir;
}

namespace
{

// A System folder of the retail game: has the engine and the game's own script package.
static bool IsGameSystemDir( const std::string& Dir )
{
	return FileExists( Dir + "Engine.u" ) && FileExists( Dir + "Klingons.u" );
}

// Dir is the System folder or the folder above it (the install folder). Returns the System folder
// (full path, trailing separator) or empty.
static std::string GameSystemDirOf( const std::string& In )
{
	if( In.empty() )
		return std::string();
	const std::string Dir = WithSlash( FullPath( "", In ) );
	if( IsGameSystemDir( Dir ) )
		return Dir;
	if( IsGameSystemDir( Dir + "System\\" ) )
		return Dir + "System\\";
	return std::string();
}

// Looks for the install near the exe: in each of the exe folder's parents (up to four levels), the
// parent itself and every folder directly inside it. Finds e.g. <game>\KHGWin\KHGWin.exe next to
// <game>\System, or builds\<build>\KHGWin.exe next to Installed\System.
static std::string FindNearExe( const std::string& ExeDir )
{
	std::string Dir = WithSlash( FullPath( "", ExeDir ) );
	for( int Level = 0; Level < 5 && Dir.size() > 3; Level++ )
	{
		std::string Found = GameSystemDirOf( Dir );
		if( !Found.empty() )
			return Found;
		WIN32_FIND_DATAA F;
		HANDLE H = FindFirstFileA( ( Dir + "*" ).c_str(), &F );
		if( H != INVALID_HANDLE_VALUE )
		{
			do
			{
				if( ( F.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) && strcmp( F.cFileName, "." ) && strcmp( F.cFileName, ".." ) )
					Found = GameSystemDirOf( Dir + F.cFileName );
			} while( Found.empty() && FindNextFileA( H, &F ) );
			FindClose( H );
		}
		if( !Found.empty() )
			return Found;
		// Up one level.
		const size_t Slash = Dir.find_last_of( "\\/", Dir.size() - 2 );
		if( Slash == std::string::npos )
			break;
		Dir = Dir.substr( 0, Slash + 1 );
	}
	return std::string();
}

// Folder dialog. Returns the picked folder, or empty if cancelled.
static std::string PickFolder( const std::string& Start )
{
	std::string Out;
	const HRESULT Co = CoInitializeEx( NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE );
	IFileOpenDialog* Dlg = NULL;
	if( SUCCEEDED( CoCreateInstance( CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_IFileOpenDialog, (void**)&Dlg ) ) && Dlg )
	{
		DWORD Opt = 0;
		Dlg->GetOptions( &Opt );
		Dlg->SetOptions( Opt | FOS_PICKFOLDERS | FOS_FORCEFILESYSTEM | FOS_PATHMUSTEXIST );
		Dlg->SetTitle( L"Choose your Klingon Honor Guard folder" );
		if( !Start.empty() )
		{
			wchar_t W[MAX_PATH * 2];
			IShellItem* Item = NULL;
			if( MultiByteToWideChar( CP_ACP, 0, Start.c_str(), -1, W, MAX_PATH * 2 ) > 0
			 && SUCCEEDED( SHCreateItemFromParsingName( W, NULL, IID_IShellItem, (void**)&Item ) ) && Item )
			{
				Dlg->SetFolder( Item );
				Item->Release();
			}
		}
		IShellItem* Result = NULL;
		if( SUCCEEDED( Dlg->Show( NULL ) ) && SUCCEEDED( Dlg->GetResult( &Result ) ) && Result )
		{
			PWSTR Path = NULL;
			if( SUCCEEDED( Result->GetDisplayName( SIGDN_FILESYSPATH, &Path ) ) && Path )
			{
				if( !ToAnsiPath( Path, Out ) )
				{
					MessageBoxW( NULL, L"This folder's path has characters KHGWin can't use. Move or rename the game folder and try again.",
						L"Klingon Honor Guard", MB_OK | MB_ICONWARNING | MB_TOPMOST );
					Out = "?";
				}
				CoTaskMemFree( Path );
			}
			Result->Release();
		}
		Dlg->Release();
	}
	if( SUCCEEDED( Co ) )
		CoUninitialize();
	return Out;
}

} // namespace

bool KHGDesktopChooseGameDir( const char* CmdLine )
{
	const std::string ExeDir = appExeDir();
	const std::string Ini = std::string( appUserDir() ) + appPackage() + ".ini";
	char Arg[1024] = "";
	std::string Dir, How;

	// 1. -gamedir=<folder> for this run (the install folder or its System folder).
	if( Parse( CmdLine, "GAMEDIR=", Arg, ARRAY_COUNT(Arg) ) && Arg[0] )
	{
		Dir = GameSystemDirOf( FullPath( ExeDir, Arg ) );
		if( Dir.empty() )
			GGameDirStatus = std::string( "WARNING: -gamedir=" ) + Arg + " is not a Klingon Honor Guard folder; ignored. ";
		else
			How = "-gamedir";
	}
	// 2. The exe is in the install's System folder (the way earlier builds were installed).
	if( Dir.empty() && !ParseParam( CmdLine, "PICKGAMEDIR" ) && IsGameSystemDir( ExeDir ) )
		Dir = ExeDir, How = "KHGWin.exe is in it";
	// 3. The folder chosen before.
	char Saved[1024] = "";
	GetPrivateProfileStringA( "KHGWin", "GameDir", "", Saved, sizeof(Saved), Ini.c_str() );
	if( Dir.empty() && !ParseParam( CmdLine, "PICKGAMEDIR" ) && Saved[0] )
	{
		Dir = GameSystemDirOf( FullPath( ExeDir, Saved ) );
		if( Dir.empty() )
			GGameDirStatus += std::string( "WARNING: GameDir=" ) + Saved + " in " + Ini + " is not a Klingon Honor Guard folder (any more). ";
		else
			How = "GameDir in " + Ini;
	}
	// 4. Next to the exe.
	if( Dir.empty() && !ParseParam( CmdLine, "PICKGAMEDIR" ) )
	{
		Dir = FindNearExe( ExeDir );
		if( !Dir.empty() )
			How = "found near KHGWin.exe";
	}
	// 5. Ask.
	if( Dir.empty() )
	{
		if( ParseParam( CmdLine, "NOGAMEDIRDIALOG" ) )
		{
			GGameDirStatus += "no game folder found (-nogamedirdialog)";
			return false;
		}
		MessageBoxA( NULL,
			"KHGWin runs the game from your own copy of Star Trek: Klingon Honor Guard, and it could not find it.\n\n"
			"Choose the folder the game is installed in (the one that has the System, Maps and Textures folders) in the next window. "
			"KHGWin remembers it; nothing in that folder is changed.",
			"Klingon Honor Guard", MB_OK | MB_ICONINFORMATION | MB_TOPMOST );
		std::string Start = Saved[0] ? FullPath( ExeDir, Saved ) : ExeDir;
		for( ;; )
		{
			const std::string Picked = PickFolder( Start );
			if( Picked.empty() )
			{
				GGameDirStatus += "no game folder chosen";
				return false;
			}
			Dir = Picked == "?" ? std::string() : GameSystemDirOf( Picked );
			if( !Dir.empty() )
			{
				How = "chosen in the folder dialog";
				break;
			}
			if( Picked != "?" && MessageBoxA( NULL, ( Picked + "\n\nThis folder has no Klingon Honor Guard game files (System\\Engine.u and System\\Klingons.u). "
				"Choose the folder the game is installed in." ).c_str(), "Klingon Honor Guard", MB_RETRYCANCEL | MB_ICONWARNING | MB_TOPMOST ) != IDRETRY )
			{
				GGameDirStatus += "no game folder chosen";
				return false;
			}
			if( Picked != "?" )
				Start = Picked;
		}
	}

	GGameDirStatus += Dir + " (" + How + ")";
	// Remember it, unless it is the exe's own folder or was only given for this run. Written after
	// KHGDesktopPrepareConfig(), which creates the ini on the first run.
	if( How != "KHGWin.exe is in it" && How != "-gamedir" && !SameName( WithSlash( FullPath( ExeDir, Saved ) ), Dir ) )
		GGameDirToSave = Dir;
	if( !SameName( Dir, ExeDir ) )
	{
		appSetBaseDir( Dir.c_str() );
		// Portable with the exe in a folder of its own: the user files stay next to the exe.
		if( GUserDirMode == UDM_Portable )
		{
			GUserDirMode = UDM_ExeFolder;
			appSetUserDir( ExeDir.c_str() );
		}
	}
	return true;
}

static void PrepareConfig( const char* BaseDir, const char* Package )
{
	const std::string Dir = BaseDir ? BaseDir : "";
	const std::string UserDir = appUserDir();
	const std::string Target = UserDir + Package + ".ini";
	std::string Existing;
	if( ReadText( Target, Existing ) && !Existing.empty() )
	{
		// Portable: the saves are the retail Save folder's, so show the retail slot names until the port
		// writes its own.
		if( GUserDirMode == UDM_Portable && SlotSection( Existing ).empty() )
		{
			std::string RetailIni, Where;
			if( ReadRetailIni( Dir, RetailIni, Where ) && !SlotSection( RetailIni ).empty()
			 && IsRetailDefaultPath( IniValue( Existing, "Core.System", "SavePath" ), "Save" )
			 && WriteText( Target, KHGMergeIni( Existing, SlotSection( RetailIni ) ) ) )
			{
				ReadText( Target, Existing );
				GMigrationLog.push_back( "save slot names taken from " + Where + " (shared retail Save folder)" );
			}
		}
		GConfigStatus = "using existing " + Target;
		const int Have = InputDefaultsIn( Existing );
		if( Have < kInputDefaultsVersion )
		{
			if( WriteText( Target, ApplyInputDefaults( Existing, Have ) ) )
				GConfigStatus += Have < 1 ? " (applied WASD movement, mouse look and the gamepad layout once)"
				                          : " (applied the gamepad layout once)";
			else
				GConfigStatus += " (WARNING: could not apply the input defaults)";
		}
		return;
	}

	// First run in the Saved Games folder: carry over the port's ini from the install, if any.
	std::string OldPortIni;
	if( MigratesFromInstall() && ReadText( Dir + Package + ".ini", OldPortIni ) && !OldPortIni.empty() )
	{
		const int Have = InputDefaultsIn( OldPortIni );
		std::string Ini = Have < kInputDefaultsVersion ? ApplyInputDefaults( OldPortIni, Have ) : OldPortIni;
		Ini = MigrateSaves( Dir, UserDir, Ini, OldPortIni );
		if( !WriteText( Target, Ini ) )
		{
			GConfigStatus = "WARNING: could not write " + Target;
			return;
		}
		GConfigStatus = "created " + Target + " from " + Dir + Package + ".ini (moved to the user folder; the old file is left as it was)";
		return;
	}

	std::string Overlay;
	const std::string OverlayPath = std::string( appExeDir() ) + Package + "Defaults.ini";   // ships next to the exe
	if( !ReadText( OverlayPath, Overlay ) )
	{
		GConfigStatus = "WARNING: " + OverlayPath + " is missing; the desktop driver settings were not applied";
		return;
	}

	// Retail defaults: Default.ini is the pristine copy; Khg.ini is the one the retail game writes.
	static const char* BaseNames[] = { "Default.ini", "DEFAULT.INI", "default.ini", "Khg.ini", "KHG.INI", "khg.ini" };
	std::string Base, BaseName;
	for( const char* Name : BaseNames )
	{
		if( ReadText( Dir + Name, Base ) )
		{
			BaseName = Name;
			break;
		}
	}
	if( BaseName.empty() )
		Base.clear();

	std::string Ini = ApplyInputDefaults( KHGMergeIni( Base, Overlay ), 0 );
	if( MigratesFromInstall() )
		Ini = MigrateSaves( Dir, UserDir, Ini, std::string() );
	else if( GUserDirMode == UDM_Portable && IsRetailDefaultPath( IniValue( Ini, "Core.System", "SavePath" ), "Save" ) )
	{
		std::string RetailIni, Where;
		if( ReadRetailIni( Dir, RetailIni, Where ) && !SlotSection( RetailIni ).empty() && SlotSection( Ini ).empty() )
		{
			Ini = KHGMergeIni( Ini, SlotSection( RetailIni ) );
			GMigrationLog.push_back( "save slot names taken from " + Where + " (shared retail Save folder)" );
		}
	}
	if( !WriteText( Target, Ini ) )
	{
		GConfigStatus = "WARNING: could not write " + Target;
		return;
	}
	GConfigStatus = BaseName.empty()
		? "WARNING: created " + Target + " from " + OverlayPath + " only; no retail Default.ini or Khg.ini found in " + Dir
		: "created " + Target + " from retail " + BaseName + " + " + OverlayPath;
}

void KHGDesktopPrepareConfig( const char* BaseDir, const char* Package )
{
	PrepareConfig( BaseDir, Package );
	if( !GGameDirToSave.empty() )
	{
		const std::string Ini = std::string( appUserDir() ) + Package + ".ini";
		if( WritePrivateProfileStringA( "KHGWin", "GameDir", GGameDirToSave.c_str(), Ini.c_str() ) )
			GGameDirStatus += ", saved as GameDir in " + Ini;
		GGameDirToSave.clear();
	}
}

void KHGDesktopLogConfigStatus()
{
	if( !GGameDirStatus.empty() )
		debugf( NAME_Init, "[KHG] Game folder: %s", GGameDirStatus.c_str() );
	if( !GUserDirStatus.empty() )
		debugf( NAME_Init, "[KHG] User files: %s%s", GUserDirStatus.c_str(), GUserDirMode == UDM_ExeFolder ? " (KHGWin.exe is outside the install)" : "" );
	if( !GConfigStatus.empty() )
		debugf( NAME_Init, "[KHG] Desktop config: %s", GConfigStatus.c_str() );
	for( const std::string& L : GMigrationLog )
		debugf( NAME_Init, "[KHG] Migration: %s", L.c_str() );
}

void KHGDesktopApplyUserPaths()
{
	if( GUserDirMode == UDM_Portable || !GSys )
		return;
	const std::string UserDir = appUserDir();
	struct { char* Path; const char* Name; } Paths[] = { { GSys->SavePath, "Save" }, { GSys->CachePath, "Cache" } };
	for( auto& P : Paths )
	{
		if( !IsRetailDefaultPath( P.Path, P.Name ) )
		{
			debugf( NAME_Init, "[KHG] %s folder: %s (set in the ini)", P.Name, P.Path );
			continue;
		}
		std::string Full = UserDir + P.Name;
		CreateDirectoryA( Full.c_str(), NULL );
		if( Full.size() >= sizeof(GSys->SavePath) )
		{
			// SavePath/CachePath hold 95 characters: try the 8.3 short name.
			char Short[MAX_PATH];
			const DWORD N = GetShortPathNameA( Full.c_str(), Short, sizeof(Short) );
			if( N > 0 && N < sizeof(GSys->SavePath) )
				Full = Short;
		}
		if( Full.size() < sizeof(GSys->SavePath) )
		{
			appStrcpy( P.Path, Full.c_str() );
			debugf( NAME_Init, "[KHG] %s folder: %s", P.Name, P.Path );
		}
		else
			debugf( NAME_Init, "[KHG] WARNING: %s folder path too long (%s); using %s", P.Name, Full.c_str(), P.Path );
	}
}

bool KHGDesktopResetConfig( const char* InBaseDir, const char* Package, const char* IniPath )
{
	const std::string Dir = InBaseDir ? InBaseDir : "";
	const std::string Target = IniPath ? IniPath : "";
	std::string Old, Overlay, Base;
	ReadText( Target, Old );
	if( !ReadText( std::string( appExeDir() ) + Package + "Defaults.ini", Overlay ) )
		return false;
	static const char* BaseNames[] = { "Default.ini", "DEFAULT.INI", "default.ini", "Khg.ini", "KHG.INI", "khg.ini" };
	bool Found = false;
	for( const char* Name : BaseNames )
		if( ( Found = ReadText( Dir + Name, Base ) ) )
			break;
	if( !Found )
		return false;
	std::string Ini = ApplyInputDefaults( KHGMergeIni( Base, Overlay ), 0 );
	if( !SlotSection( Old ).empty() )
		Ini = KHGMergeIni( Ini, SlotSection( Old ) );   // the load menu's names for the saves that are there
	const std::string Backup = Target + ".bak";
	DeleteFileA( Backup.c_str() );
	if( !Old.empty() && !MoveFileA( Target.c_str(), Backup.c_str() ) )
		return false;
	if( !WriteText( Target, Ini ) )
	{
		MoveFileA( Backup.c_str(), Target.c_str() );
		return false;
	}
	return true;
}
