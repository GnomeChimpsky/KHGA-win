/*=============================================================================
	KHGDesktopConfig.cpp: first-run ini for the desktop build. See KHGDesktopConfig.h.
=============================================================================*/

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

// WASD movement with mouse look (Galvatron, 2026-11-01). Applied when KHGWin.ini is created, and
// once to an existing KHGWin.ini that predates it (marked by [KHGWin] InputDefaults). Only these
// keys change; all other bindings stay as in the retail ini. Retail Default.ini binds S to
// "Axis aUp" (swim/fly up); Space (Jump) and C (Duck) already do that.
static const int kInputDefaultsVersion = 1;
static const char* const kInputDefaults =
	"[Engine.Input]\r\n"
	"W=MoveForward\r\n"
	"S=MoveBackward\r\n"
	"A=StrafeLeft\r\n"
	"D=StrafeRight\r\n"
	"[Engine.PlayerPawn]\r\n"
	"bAlwaysMouseLook=True\r\n"
	"[KHGWin]\r\n"
	"InputDefaults=1\r\n";

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

void KHGDesktopPrepareConfig( const char* BaseDir, const char* Package )
{
	const std::string Dir = BaseDir ? BaseDir : "";
	const std::string Target = Dir + Package + ".ini";
	std::string Existing;
	if( ReadText( Target, Existing ) && !Existing.empty() )
	{
		GConfigStatus = "using existing " + Target;
		if( InputDefaultsIn( Existing ) < kInputDefaultsVersion )
		{
			if( WriteText( Target, KHGMergeIni( Existing, kInputDefaults ) ) )
				GConfigStatus += " (applied WASD movement and mouse look once)";
			else
				GConfigStatus += " (WARNING: could not apply WASD defaults)";
		}
		return;
	}

	std::string Overlay;
	const std::string OverlayPath = Dir + Package + "Defaults.ini";
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

	if( !WriteText( Target, KHGMergeIni( KHGMergeIni( Base, Overlay ), kInputDefaults ) ) )
	{
		GConfigStatus = "WARNING: could not write " + Target;
		return;
	}
	GConfigStatus = BaseName.empty()
		? "WARNING: created " + Target + " from " + OverlayPath + " only; no retail Default.ini or Khg.ini found in " + Dir
		: "created " + Target + " from retail " + BaseName + " + " + OverlayPath;
}

void KHGDesktopLogConfigStatus()
{
	if( !GConfigStatus.empty() )
		debugf( NAME_Init, "[KHG] Desktop config: %s", GConfigStatus.c_str() );
}
