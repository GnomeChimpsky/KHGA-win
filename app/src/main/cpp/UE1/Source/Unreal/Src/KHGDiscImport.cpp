/*=============================================================================
	KHGDiscImport.cpp: copies the movies and the CD music from the game CD (desktop build).
	See KHGDiscImport.h.

	Sources:
	- a CD/DVD drive (a real disc 2 or a mounted .iso): the AVI folder through the file system,
	  the audio tracks with IOCTL_CDROM_READ_TOC + IOCTL_CDROM_RAW_READ (CDDA sectors);
	- a .cue/.bin image: the data track is read as ISO 9660 straight from the .bin, the audio
	  tracks are the raw 2352-byte sectors of the .bin (16-bit little-endian stereo, 44100 Hz);
	- an .iso image (data only, no music).
	A source counts as the game's disc 2 when its data track has AVI\Brief0102.avi. Disc 1
	only has INTRO.AVI (in its root), which is copied too when the install lacks it.
	Audio tracks are written as Track02.wav and so on (what NOpenALDrv/CDMusic.cpp looks for).
=============================================================================*/

#include <windows.h>
#include <commctrl.h>
#include <commdlg.h>
#include <shlobj.h>
#include <winioctl.h>
#include <ntddcdrm.h>

#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

#include "KHGDiscImport.h"

extern "C" {
#include "libunshield.h"   // thirdparty/unshield: the installer's InstallShield 5 cabinet
}

namespace
{

const int RawSector = 2352;
const char* const KeyMovie = "brief0102.avi";   // the first briefing; its presence means "the movies are there"

std::string Lower( std::string S )
{
	for( char& C : S )
		C = (char)tolower( (unsigned char)C );
	return S;
}

bool FileExists( const std::string& P )
{
	const DWORD A = GetFileAttributesA( P.c_str() );
	return A != INVALID_FILE_ATTRIBUTES && !( A & FILE_ATTRIBUTE_DIRECTORY );
}

bool DirExists( const std::string& P )
{
	const DWORD A = GetFileAttributesA( P.c_str() );
	return A != INVALID_FILE_ATTRIBUTES && ( A & FILE_ATTRIBUTE_DIRECTORY );
}

uint64_t FileSize( const std::string& P )
{
	WIN32_FILE_ATTRIBUTE_DATA D;
	if( !GetFileAttributesExA( P.c_str(), GetFileExInfoStandard, &D ) )
		return 0;
	return ( (uint64_t)D.nFileSizeHigh << 32 ) | D.nFileSizeLow;
}

std::string Slash( std::string P )
{
	if( !P.empty() && P.back() != '\\' && P.back() != '/' )
		P += '\\';
	return P;
}

std::string FullPath( const std::string& P )
{
	char Buf[MAX_PATH * 2];
	const DWORD N = GetFullPathNameA( P.c_str(), sizeof(Buf), Buf, NULL );
	return ( N > 0 && N < sizeof(Buf) ) ? std::string( Buf ) : P;
}

std::string IniGet( const char* IniPath, const char* Section, const char* Key, const char* Default )
{
	char Buf[1024];
	GetPrivateProfileStringA( Section, Key, Default, Buf, sizeof(Buf), IniPath ? IniPath : "" );
	return Buf;
}

bool IniBool( const char* IniPath, const char* Section, const char* Key, bool Default )
{
	const std::string V = Lower( IniGet( IniPath, Section, Key, "" ) );
	if( V.empty() )
		return Default;
	return !( V == "false" || V == "0" || V == "no" || V == "off" );
}

// The folder the game takes the ripped tracks from ([NOpenALDrv.NOpenALAudioSubsystem] CDMusicPath,
// relative to System unless absolute), as NOpenALDrv resolves it.
std::string MusicDir( const std::string& BaseDir, const char* IniPath )
{
	std::string P = IniGet( IniPath, "NOpenALDrv.NOpenALAudioSubsystem", "CDMusicPath", "..\\Music" );
	if( P.empty() )
		P = "..\\Music";
	const bool Absolute = ( P.size() > 1 && P[1] == ':' ) || P[0] == '\\' || P[0] == '/';
	return Slash( FullPath( Absolute ? P : BaseDir + P ) );
}

bool IsAudioExt( const std::string& E )
{
	const std::string L = Lower( E );
	return L == ".ogg" || L == ".flac" || L == ".mp3" || L == ".wav" || L == ".opus";
}

// True when Dir has a file for CD track Track under one of the exact names CDMusic.cpp tries first.
bool HasTrack( const std::string& Dir, int Track )
{
	char Want[4][16];
	snprintf( Want[0], sizeof(Want[0]), "track%02d", Track );
	snprintf( Want[1], sizeof(Want[1]), "track %02d", Track );
	snprintf( Want[2], sizeof(Want[2]), "track%d", Track );
	snprintf( Want[3], sizeof(Want[3]), "%02d", Track );
	WIN32_FIND_DATAA F;
	HANDLE H = FindFirstFileA( ( Slash( Dir ) + "*" ).c_str(), &F );
	if( H == INVALID_HANDLE_VALUE )
		return false;
	bool Found = false;
	do
	{
		if( F.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
			continue;
		std::string Name = F.cFileName;
		const size_t Dot = Name.rfind( '.' );
		if( Dot == std::string::npos || !IsAudioExt( Name.substr( Dot ) ) )
			continue;
		const std::string Stem = Lower( Name.substr( 0, Dot ) );
		for( auto& W : Want )
			Found |= ( Stem == W );
	} while( !Found && FindNextFileA( H, &F ) );
	FindClose( H );
	return Found;
}

// Can files be created in Dir (made if missing)?
bool Writable( const std::string& Dir )
{
	CreateDirectoryA( Dir.c_str(), NULL );
	const std::string T = Slash( Dir ) + "KHGWrite.tmp";
	HANDLE H = CreateFileA( T.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_FLAG_DELETE_ON_CLOSE, NULL );
	if( H == INVALID_HANDLE_VALUE )
		return false;
	CloseHandle( H );
	return true;
}

std::string MB( uint64_t Bytes )
{
	char B[32];
	if( Bytes < 1024 * 1024 )
		snprintf( B, sizeof(B), "%u KB", (unsigned)( ( Bytes + 512 ) / 1024 ) );
	else
		snprintf( B, sizeof(B), "%u MB", (unsigned)( ( Bytes + 512 * 1024 ) / ( 1024 * 1024 ) ) );
	return B;
}

/*-----------------------------------------------------------------------------
	Sources.
-----------------------------------------------------------------------------*/

struct FMovie
{
	std::string Name;      // as on the disc, e.g. Brief0102.avi
	uint64_t Size = 0;
	std::string Path;      // drive: full path
	uint32_t Lba = 0;      // image: first sector of the file in the data track
};

struct FTrack
{
	int Number = 0;
	uint32_t Lba = 0;      // drive: first sector
	uint32_t Sectors = 0;
	std::string Bin;       // image: file holding the track
	uint64_t Offset = 0;   // image: byte offset of the first sector in Bin
};

struct FSource
{
	std::string Label;     // "drive E:" or the image's file name
	char Drive = 0;
	// Image data track.
	std::string DataBin;
	uint64_t DataOffset = 0;
	int SectorSize = 2048, UserOffset = 0;
	std::string Error;     // why an image couldn't be read
	std::vector<FMovie> Movies;   // the AVI folder (disc 2)
	FMovie Intro;                 // root INTRO.AVI (disc 1)
	FMovie Cab;                   // root DATA1.CAB, the installer's data (disc 1)
	FMovie Art;                   // AUTOPLAY\AUTOBACKDROP.BMP, the autorun menu's backdrop (disc 1)
	std::vector<FTrack> Tracks;   // audio tracks (disc 2 only)

	bool IsDisc2() const
	{
		for( const FMovie& M : Movies )
			if( Lower( M.Name ) == KeyMovie )
				return true;
		return false;
	}
	bool Useful() const { return IsDisc2() || !Intro.Name.empty(); }
};

// ISO 9660 over a data track in an image file.
struct FIsoReader
{
	FILE* F = nullptr;
	const FSource* S = nullptr;
	bool Read( uint32_t Lba, unsigned char* Out )
	{
		const uint64_t Pos = S->DataOffset + (uint64_t)Lba * S->SectorSize + S->UserOffset;
		return _fseeki64( F, (int64_t)Pos, SEEK_SET ) == 0 && fread( Out, 1, 2048, F ) == 2048;
	}
	bool Joliet = false;   // names are UCS-2 big-endian (the Joliet tree keeps their case, e.g. Brief0102.avi)
	struct FEntry { std::string Name; uint32_t Lba; uint32_t Size; bool Dir; };
	bool List( uint32_t Lba, uint32_t Size, std::vector<FEntry>& Out )
	{
		unsigned char Sec[2048];
		for( uint32_t Done = 0; Done < Size; Done += 2048 )
		{
			if( !Read( Lba + Done / 2048, Sec ) )
				return false;
			for( unsigned P = 0; P < 2048; )
			{
				const unsigned Len = Sec[P];
				if( Len == 0 || P + Len > 2048 )
					break;   // rest of the sector is padding
				const unsigned NameLen = Sec[P + 32];
				FEntry E;
				memcpy( &E.Lba, Sec + P + 2, 4 );
				memcpy( &E.Size, Sec + P + 10, 4 );
				E.Dir = ( Sec[P + 25] & 2 ) != 0;
				const unsigned N = std::min<unsigned>( NameLen, Len - 33 );
				if( Joliet && N >= 2 )
					for( unsigned k = 0; k + 1 < N; k += 2 )
						E.Name += ( Sec[P + 33 + k] == 0 && Sec[P + 34 + k] >= 0x20 ) ? (char)Sec[P + 34 + k] : '_';
				else
					E.Name.assign( (const char*)Sec + P + 33, N );
				const size_t Semi = E.Name.find( ';' );
				if( Semi != std::string::npos )
					E.Name.resize( Semi );
				if( !E.Name.empty() && E.Name.back() == '.' )
					E.Name.pop_back();
				if( NameLen == 1 && ( E.Name.empty() || E.Name[0] == 0 || E.Name[0] == 1 ) )
					E.Name.clear();   // "." and ".."
				if( !E.Name.empty() )
					Out.push_back( E );
				P += Len;
			}
		}
		return true;
	}
};

// Fills Movies/Intro of an image source from its data track.
void ReadImageData( FSource& S )
{
	FILE* F = fopen( S.DataBin.c_str(), "rb" );
	if( !F )
	{
		S.Error = "can't open " + S.DataBin;
		return;
	}
	FIsoReader R;
	R.F = F;
	R.S = &S;
	unsigned char Pvd[2048];
	if( !R.Read( 16, Pvd ) || Pvd[0] != 1 || memcmp( Pvd + 1, "CD001", 5 ) != 0 )
	{
		S.Error = "no ISO 9660 data track found";
		fclose( F );
		return;
	}
	uint32_t RootLba, RootSize;
	memcpy( &RootLba, Pvd + 156 + 2, 4 );
	memcpy( &RootSize, Pvd + 156 + 10, 4 );
	// A Joliet supplementary descriptor (type 2, escape %/@ %/C or %/E) has the long, mixed-case names.
	unsigned char Vd[2048];
	for( uint32_t L = 17; L < 32 && R.Read( L, Vd ) && Vd[0] != 255; ++L )
		if( Vd[0] == 2 && memcmp( Vd + 1, "CD001", 5 ) == 0 && Vd[88] == '%' && Vd[89] == '/'
		 && ( Vd[90] == '@' || Vd[90] == 'C' || Vd[90] == 'E' ) )
		{
			memcpy( &RootLba, Vd + 156 + 2, 4 );
			memcpy( &RootSize, Vd + 156 + 10, 4 );
			R.Joliet = true;
			break;
		}
	std::vector<FIsoReader::FEntry> Root;
	R.List( RootLba, RootSize, Root );
	for( const auto& E : Root )
	{
		if( !E.Dir && Lower( E.Name ) == "data1.cab" )
		{
			S.Cab.Name = "DATA1.CAB";
			S.Cab.Size = E.Size;
			S.Cab.Lba = E.Lba;
		}
		if( !E.Dir && Lower( E.Name ) == "intro.avi" )
		{
			S.Intro.Name = "INTRO.AVI";
			S.Intro.Size = E.Size;
			S.Intro.Lba = E.Lba;
		}
		if( E.Dir && Lower( E.Name ) == "autoplay" )
		{
			std::vector<FIsoReader::FEntry> Auto;
			R.List( E.Lba, E.Size, Auto );
			for( const auto& A : Auto )
				if( !A.Dir && Lower( A.Name ) == "autobackdrop.bmp" )
				{
					S.Art.Name = "autobackdrop.bmp";
					S.Art.Size = A.Size;
					S.Art.Lba = A.Lba;
				}
		}
		if( E.Dir && Lower( E.Name ) == "avi" )
		{
			std::vector<FIsoReader::FEntry> Avi;
			R.List( E.Lba, E.Size, Avi );
			for( const auto& A : Avi )
				if( !A.Dir && Lower( A.Name ).size() > 4 && Lower( A.Name ).substr( A.Name.size() - 4 ) == ".avi" )
				{
					FMovie M;
					M.Name = A.Name;
					M.Size = A.Size;
					M.Lba = A.Lba;
					S.Movies.push_back( M );
				}
		}
	}
	fclose( F );
}

int ParseMsf( const std::string& T )
{
	int M = 0, Sec = 0, Fr = 0;
	if( sscanf( T.c_str(), "%d:%d:%d", &M, &Sec, &Fr ) != 3 )
		return -1;
	return ( M * 60 + Sec ) * 75 + Fr;
}

// A .cue sheet: its data track (ISO 9660) and its audio tracks.
FSource OpenCue( const std::string& CuePath )
{
	FSource S;
	S.Label = CuePath.substr( CuePath.find_last_of( "\\/" ) + 1 );
	FILE* F = fopen( CuePath.c_str(), "rb" );
	if( !F )
	{
		S.Error = "can't open the file";
		return S;
	}
	const std::string Dir = CuePath.substr( 0, CuePath.find_last_of( "\\/" ) + 1 );
	struct FCueTrack { int Number; std::string Type; std::string File; int Index0 = -1, Index1 = -1; };
	std::vector<FCueTrack> Tracks;
	std::string File;
	char Line[1024];
	while( fgets( Line, sizeof(Line), F ) )
	{
		std::string L = Line;
		while( !L.empty() && ( L.back() == '\r' || L.back() == '\n' ) )
			L.pop_back();
		size_t P = L.find_first_not_of( " \t" );
		if( P == std::string::npos )
			continue;
		L = L.substr( P );
		const std::string U = Lower( L );
		if( U.compare( 0, 5, "file " ) == 0 )
		{
			const size_t Q1 = L.find( '"' ), Q2 = L.rfind( '"' );
			std::string Name = ( Q1 != std::string::npos && Q2 > Q1 ) ? L.substr( Q1 + 1, Q2 - Q1 - 1 ) : L.substr( 5, L.find( ' ', 5 ) - 5 );
			const bool Absolute = ( Name.size() > 1 && Name[1] == ':' ) || Name[0] == '\\';
			File = Absolute ? Name : Dir + Name;
		}
		else if( U.compare( 0, 6, "track " ) == 0 )
		{
			FCueTrack T;
			char Type[64] = "";
			sscanf( L.c_str() + 6, "%d %63s", &T.Number, Type );
			T.Type = Lower( Type );
			T.File = File;
			Tracks.push_back( T );
		}
		else if( U.compare( 0, 6, "index " ) == 0 && !Tracks.empty() )
		{
			int N = 0;
			char Msf[32] = "";
			sscanf( L.c_str() + 6, "%d %31s", &N, Msf );
			if( N == 0 ) Tracks.back().Index0 = ParseMsf( Msf );
			if( N == 1 ) Tracks.back().Index1 = ParseMsf( Msf );
		}
	}
	fclose( F );
	if( Tracks.empty() )
	{
		S.Error = "no tracks in the cue sheet";
		return S;
	}
	auto SectorSizeOf = []( const std::string& Type ) { return Type == "mode1/2048" ? 2048 : ( Type == "mode2/2336" ? 2336 : RawSector ); };
	for( size_t i = 0; i < Tracks.size(); ++i )
	{
		const FCueTrack& T = Tracks[i];
		if( T.Index1 < 0 )
			continue;
		const int Size = SectorSizeOf( T.Type );
		if( T.Type != "audio" )
		{
			if( S.DataBin.empty() )
			{
				S.DataBin = T.File;
				S.SectorSize = Size;
				S.UserOffset = T.Type == "mode1/2352" ? 16 : ( T.Type.compare( 0, 5, "mode2" ) == 0 ? ( Size == RawSector ? 24 : 8 ) : 0 );
				S.DataOffset = (uint64_t)T.Index1 * Size;
			}
			continue;
		}
		// The track runs to the next track's start (its pregap, INDEX 00, if it has one) in the same file,
		// or to the end of the file.
		uint64_t EndSector;
		if( i + 1 < Tracks.size() && Tracks[i + 1].File == T.File )
			EndSector = (uint64_t)( Tracks[i + 1].Index0 >= 0 ? Tracks[i + 1].Index0 : Tracks[i + 1].Index1 );
		else
			EndSector = FileSize( T.File ) / Size;
		if( EndSector <= (uint64_t)T.Index1 )
			continue;
		FTrack A;
		A.Number = T.Number;
		A.Sectors = (uint32_t)( EndSector - T.Index1 );
		A.Bin = T.File;
		A.Offset = (uint64_t)T.Index1 * Size;
		S.Tracks.push_back( A );
	}
	if( S.DataBin.empty() )
		S.Error = "the image has no data track";
	else
		ReadImageData( S );
	return S;
}

FSource OpenImage( const std::string& Path )
{
	const std::string L = Lower( Path );
	const std::string Ext = L.size() > 4 ? L.substr( L.size() - 4 ) : "";
	if( Ext == ".cue" )
		return OpenCue( Path );
	if( Ext == ".bin" || Ext == ".img" )
	{
		const std::string Cue = Path.substr( 0, Path.size() - 4 ) + ".cue";
		if( FileExists( Cue ) )
			return OpenCue( Cue );
	}
	FSource S;
	S.Label = Path.substr( Path.find_last_of( "\\/" ) + 1 );
	S.DataBin = Path;
	if( Ext == ".bin" || Ext == ".img" )
	{
		// A .bin without a cue sheet: one raw MODE1/2352 data track.
		S.SectorSize = RawSector;
		S.UserOffset = 16;
	}
	ReadImageData( S );
	return S;
}

uint32_t MsfToLba( const UCHAR Address[4] )
{
	return ( Address[1] * 60 + Address[2] ) * 75 + Address[3] - 150;
}

// The audio tracks of the disc in Drive, from its table of contents.
void ReadDriveTracks( FSource& S )
{
	char Dev[] = "\\\\.\\?:";
	Dev[4] = S.Drive;
	HANDLE H = CreateFileA( Dev, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL );
	if( H == INVALID_HANDLE_VALUE )
		return;
	CDROM_TOC Toc;
	DWORD Got = 0;
	memset( &Toc, 0, sizeof(Toc) );
	if( DeviceIoControl( H, IOCTL_CDROM_READ_TOC, NULL, 0, &Toc, sizeof(Toc), &Got, NULL ) )
	{
		const int Count = Toc.LastTrack - Toc.FirstTrack + 1;
		for( int i = 0; i < Count && i + 1 < MAXIMUM_NUMBER_TRACKS; ++i )
		{
			const TRACK_DATA& T = Toc.TrackData[i];
			const TRACK_DATA& N = Toc.TrackData[i + 1];
			if( T.Control & 4 )
				continue;   // data track
			uint32_t Start = MsfToLba( T.Address ), End = MsfToLba( N.Address );
			if( N.TrackNumber != 0xAA && ( N.Control & 4 ) )
				End -= 11400;   // a data session after the audio (enhanced CD)
			if( End <= Start )
				continue;
			FTrack A;
			A.Number = T.TrackNumber;
			A.Lba = Start;
			A.Sectors = End - Start;
			S.Tracks.push_back( A );
		}
	}
	CloseHandle( H );
}

void ListAvi( const std::string& Dir, std::vector<FMovie>& Out )
{
	WIN32_FIND_DATAA F;
	HANDLE H = FindFirstFileA( ( Dir + "*.avi" ).c_str(), &F );
	if( H == INVALID_HANDLE_VALUE )
		return;
	do
	{
		if( F.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
			continue;
		FMovie M;
		M.Name = F.cFileName;
		M.Size = ( (uint64_t)F.nFileSizeHigh << 32 ) | F.nFileSizeLow;
		M.Path = Dir + F.cFileName;
		Out.push_back( M );
	} while( FindNextFileA( H, &F ) );
	FindClose( H );
}

// Every CD/DVD drive (mounted .iso images are drives too) that holds disc 1 or disc 2.
std::vector<FSource> ScanDrives( bool WithTracks )
{
	std::vector<FSource> Out;
	const DWORD Mask = GetLogicalDrives();
	const UINT OldMode = SetErrorMode( SEM_FAILCRITICALERRORS );   // no "insert a disc" boxes for empty drives
	for( int i = 0; i < 26; ++i )
	{
		if( !( Mask & ( 1u << i ) ) )
			continue;
		char Root[] = "?:\\";
		Root[0] = (char)( 'A' + i );
		if( GetDriveTypeA( Root ) != DRIVE_CDROM )
			continue;
		if( !GetVolumeInformationA( Root, NULL, 0, NULL, NULL, NULL, NULL, 0 ) )
			continue;   // no disc
		FSource S;
		S.Drive = Root[0];
		S.Label = std::string( "drive " ) + Root[0] + ":";
		ListAvi( std::string( Root ) + "AVI\\", S.Movies );
		if( FileExists( std::string( Root ) + "INTRO.AVI" ) )
		{
			S.Intro.Name = "INTRO.AVI";
			S.Intro.Path = std::string( Root ) + "INTRO.AVI";
			S.Intro.Size = FileSize( S.Intro.Path );
		}
		if( FileExists( std::string( Root ) + "DATA1.CAB" ) )
		{
			S.Cab.Name = "DATA1.CAB";
			S.Cab.Path = std::string( Root ) + "DATA1.CAB";
			S.Cab.Size = FileSize( S.Cab.Path );
		}
		if( FileExists( std::string( Root ) + "AUTOPLAY\\AUTOBACKDROP.BMP" ) )
		{
			S.Art.Name = "autobackdrop.bmp";
			S.Art.Path = std::string( Root ) + "AUTOPLAY\\AUTOBACKDROP.BMP";
			S.Art.Size = FileSize( S.Art.Path );
		}
		if( !S.Useful() && S.Cab.Name.empty() )
			continue;
		if( WithTracks && S.IsDisc2() )
			ReadDriveTracks( S );
		Out.push_back( S );
	}
	SetErrorMode( OldMode );
	return Out;
}

/*-----------------------------------------------------------------------------
	The copy.
-----------------------------------------------------------------------------*/

struct FPlan
{
	std::string MovieDir, MusicDir;      // where the files go
	bool MovieFallback = false, MusicFallback = false;   // the user folder, as the install can't be written to
	std::vector<std::pair<const FSource*, FMovie>> Movies;
	std::vector<std::pair<const FSource*, FTrack>> Tracks;
	uint64_t Bytes = 0;
};

struct FJob
{
	std::string BaseDir, UserDir, IniPath;
	std::vector<FSource> Sources;
	FPlan Plan;
	// Progress, shared with the window (read under the lock).
	CRITICAL_SECTION Lock;
	std::string Current;
	uint64_t Done = 0;
	volatile LONG Cancel = 0;
	volatile LONG Finished = 0;
	std::vector<std::string> Log;     // what was done (also the engine log and the summary)
	int Copied = 0, Ripped = 0, Failed = 0;
	FJob() { InitializeCriticalSection( &Lock ); }
	~FJob() { DeleteCriticalSection( &Lock ); }
	void SetCurrent( const std::string& S ) { EnterCriticalSection( &Lock ); Current = S; LeaveCriticalSection( &Lock ); }
	void Add( const std::string& S ) { EnterCriticalSection( &Lock ); Log.push_back( S ); LeaveCriticalSection( &Lock ); }
	void Progress( uint64_t N ) { EnterCriticalSection( &Lock ); Done += N; LeaveCriticalSection( &Lock ); }
};

std::vector<std::string> GStatus;   // for KHGDiscImportLogStatus

bool MoviePresent( const std::string& BaseDir, const std::string& UserDir, const FMovie& M )
{
	for( const std::string& D : { BaseDir, UserDir + "Movies\\" } )
	{
		const std::string P = D + M.Name;
		if( FileExists( P ) && ( M.Size == 0 || FileSize( P ) == M.Size ) )
			return true;
	}
	return false;
}

bool TrackPresent( const std::string& MusicFolder, const std::string& UserDir, int Track )
{
	return HasTrack( MusicFolder, Track ) || HasTrack( UserDir + "Music", Track );
}

// What to copy from the sources found, and where to.
FPlan MakePlan( const std::string& BaseDir, const std::string& UserDir, const char* IniPath, const std::vector<FSource>& Sources )
{
	FPlan P;
	const std::string MusicFolder = MusicDir( BaseDir, IniPath );
	std::vector<std::string> Names;
	for( const FSource& S : Sources )
	{
		auto AddMovie = [&]( const FMovie& M )
		{
			if( M.Name.empty() || std::find( Names.begin(), Names.end(), Lower( M.Name ) ) != Names.end() )
				return;
			Names.push_back( Lower( M.Name ) );
			if( !MoviePresent( BaseDir, UserDir, M ) )
			{
				P.Movies.push_back( { &S, M } );
				P.Bytes += M.Size;
			}
		};
		for( const FMovie& M : S.Movies )
			AddMovie( M );
		AddMovie( S.Intro );
		if( S.IsDisc2() )
			for( const FTrack& T : S.Tracks )
			{
				bool Dup = false;
				for( auto& Q : P.Tracks )
					Dup |= ( Q.second.Number == T.Number );
				if( !Dup && !TrackPresent( MusicFolder, UserDir, T.Number ) )
				{
					P.Tracks.push_back( { &S, T } );
					P.Bytes += (uint64_t)T.Sectors * RawSector + 44;
				}
			}
	}
	P.MovieDir = BaseDir;
	if( !P.Movies.empty() && !Writable( BaseDir ) )
	{
		P.MovieDir = UserDir + "Movies\\";
		P.MovieFallback = true;
	}
	P.MusicDir = MusicFolder;
	if( !P.Tracks.empty() && !Writable( MusicFolder ) )
	{
		P.MusicDir = UserDir + "Music\\";
		P.MusicFallback = true;
	}
	return P;
}

bool WriteAll( HANDLE H, const void* Data, DWORD Size )
{
	DWORD W = 0;
	return WriteFile( H, Data, Size, &W, NULL ) && W == Size;
}

// Copies one movie to Dest (via a .part file). Returns an error, or "" when done.
std::string CopyMovie( FJob& J, const FSource& S, const FMovie& M, const std::string& Dest )
{
	const std::string Part = Dest + ".part";
	HANDLE Out = CreateFileA( Part.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL );
	if( Out == INVALID_HANDLE_VALUE )
		return "can't write " + Part;
	std::vector<unsigned char> Buf( 1 << 20 );
	std::string Err;
	if( S.Drive )
	{
		HANDLE In = CreateFileA( M.Path.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL );
		if( In == INVALID_HANDLE_VALUE )
			Err = "can't read " + M.Path;
		for( DWORD Got = 0; Err.empty(); )
		{
			if( J.Cancel ) { Err = "cancelled"; break; }
			if( !ReadFile( In, Buf.data(), (DWORD)Buf.size(), &Got, NULL ) ) { Err = "read error in " + M.Path; break; }
			if( Got == 0 ) break;
			if( !WriteAll( Out, Buf.data(), Got ) ) { Err = "can't write " + Part + " (disk full?)"; break; }
			J.Progress( Got );
		}
		if( In != INVALID_HANDLE_VALUE )
			CloseHandle( In );
	}
	else
	{
		FILE* In = fopen( S.DataBin.c_str(), "rb" );
		if( !In )
			Err = "can't read " + S.DataBin;
		unsigned char Sec[RawSector];
		uint64_t Left = M.Size;
		for( uint32_t Lba = M.Lba; Err.empty() && Left > 0; ++Lba )
		{
			if( J.Cancel ) { Err = "cancelled"; break; }
			const uint64_t Pos = S.DataOffset + (uint64_t)Lba * S.SectorSize + S.UserOffset;
			if( _fseeki64( In, (int64_t)Pos, SEEK_SET ) != 0 || fread( Sec, 1, 2048, In ) != 2048 ) { Err = "read error in " + S.DataBin; break; }
			const DWORD N = (DWORD)std::min<uint64_t>( Left, 2048 );
			if( !WriteAll( Out, Sec, N ) ) { Err = "can't write " + Part + " (disk full?)"; break; }
			Left -= N;
			J.Progress( N );
		}
		if( In )
			fclose( In );
	}
	CloseHandle( Out );
	if( Err.empty() && !MoveFileExA( Part.c_str(), Dest.c_str(), MOVEFILE_REPLACE_EXISTING ) )
		Err = "can't rename " + Part;
	if( !Err.empty() )
		DeleteFileA( Part.c_str() );
	return Err;
}

void WavHeader( unsigned char H[44], uint32_t DataBytes )
{
	auto U32 = [&]( int At, uint32_t V ) { memcpy( H + At, &V, 4 ); };
	auto U16 = [&]( int At, uint16_t V ) { memcpy( H + At, &V, 2 ); };
	memcpy( H, "RIFF", 4 ); U32( 4, 36 + DataBytes ); memcpy( H + 8, "WAVEfmt ", 8 );
	U32( 16, 16 ); U16( 20, 1 ); U16( 22, 2 ); U32( 24, 44100 ); U32( 28, 44100 * 4 ); U16( 32, 4 ); U16( 34, 16 );
	memcpy( H + 36, "data", 4 ); U32( 40, DataBytes );
}

// Rips one audio track to Dest as a WAV file.
std::string RipTrack( FJob& J, const FSource& S, const FTrack& T, const std::string& Dest )
{
	const std::string Part = Dest + ".part";
	HANDLE Out = CreateFileA( Part.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL );
	if( Out == INVALID_HANDLE_VALUE )
		return "can't write " + Part;
	unsigned char Head[44];
	WavHeader( Head, T.Sectors * RawSector );
	std::string Err;
	if( !WriteAll( Out, Head, 44 ) )
		Err = "can't write " + Part;
	const uint32_t Chunk = 24;   // sectors per read; small enough for every drive
	std::vector<unsigned char> Buf( Chunk * RawSector );
	if( S.Drive && Err.empty() )
	{
		char Dev[] = "\\\\.\\?:";
		Dev[4] = S.Drive;
		HANDLE In = CreateFileA( Dev, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL );
		if( In == INVALID_HANDLE_VALUE )
			In = CreateFileA( Dev, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL );
		if( In == INVALID_HANDLE_VALUE )
			Err = std::string( "can't open drive " ) + S.Drive + ":";
		for( uint32_t At = 0; Err.empty() && At < T.Sectors; )
		{
			if( J.Cancel ) { Err = "cancelled"; break; }
			const uint32_t N = std::min( Chunk, T.Sectors - At );
			RAW_READ_INFO Info;
			memset( &Info, 0, sizeof(Info) );
			Info.DiskOffset.QuadPart = (LONGLONG)( T.Lba + At ) * 2048;
			Info.SectorCount = N;
			Info.TrackMode = CDDA;
			DWORD Got = 0;
			bool Ok = false;
			for( int Try = 0; Try < 4 && !Ok; ++Try )
				Ok = DeviceIoControl( In, IOCTL_CDROM_RAW_READ, &Info, sizeof(Info), Buf.data(), N * RawSector, &Got, NULL ) && Got == N * RawSector;
			if( !Ok )
			{
				char E[96];
				snprintf( E, sizeof(E), "the drive couldn't read sector %u of track %d", (unsigned)( T.Lba + At ), T.Number );
				Err = E;
				break;
			}
			if( !WriteAll( Out, Buf.data(), N * RawSector ) ) { Err = "can't write " + Part + " (disk full?)"; break; }
			J.Progress( N * RawSector );
			At += N;
		}
		if( In != INVALID_HANDLE_VALUE )
			CloseHandle( In );
	}
	else if( Err.empty() )
	{
		FILE* In = fopen( T.Bin.c_str(), "rb" );
		if( !In || _fseeki64( In, (int64_t)T.Offset, SEEK_SET ) != 0 )
			Err = "can't read " + T.Bin;
		for( uint32_t At = 0; Err.empty() && At < T.Sectors; )
		{
			if( J.Cancel ) { Err = "cancelled"; break; }
			const uint32_t N = std::min( Chunk, T.Sectors - At );
			if( fread( Buf.data(), 1, N * RawSector, In ) != N * RawSector ) { Err = "read error in " + T.Bin; break; }
			if( !WriteAll( Out, Buf.data(), N * RawSector ) ) { Err = "can't write " + Part + " (disk full?)"; break; }
			J.Progress( N * RawSector );
			At += N;
		}
		if( In )
			fclose( In );
	}
	CloseHandle( Out );
	if( Err.empty() && !MoveFileExA( Part.c_str(), Dest.c_str(), MOVEFILE_REPLACE_EXISTING ) )
		Err = "can't rename " + Part;
	if( !Err.empty() )
		DeleteFileA( Part.c_str() );
	return Err;
}

void RunJob( FJob& J )
{
	FPlan& P = J.Plan;
	if( !P.Movies.empty() )
		CreateDirectoryA( P.MovieDir.c_str(), NULL );
	if( !P.Tracks.empty() )
		CreateDirectoryA( P.MusicDir.c_str(), NULL );
	if( P.MovieFallback )
		J.Add( "the install's System folder can't be written to; movies go to " + P.MovieDir );
	if( P.MusicFallback )
		J.Add( "the music folder can't be written to; tracks go to " + P.MusicDir );
	for( auto& M : P.Movies )
	{
		if( J.Cancel ) break;
		J.SetCurrent( "Copying " + M.second.Name + " from " + M.first->Label );
		const std::string Err = CopyMovie( J, *M.first, M.second, P.MovieDir + M.second.Name );
		if( Err.empty() ) { J.Copied++; J.Add( "copied " + M.second.Name + " (" + MB( M.second.Size ) + ") from " + M.first->Label + " to " + P.MovieDir ); }
		else if( Err != "cancelled" ) { J.Failed++; J.Add( "FAILED " + M.second.Name + ": " + Err ); }
	}
	for( auto& T : P.Tracks )
	{
		if( J.Cancel ) break;
		char Name[32];
		snprintf( Name, sizeof(Name), "Track%02d.wav", T.second.Number );
		char Cur[128];
		snprintf( Cur, sizeof(Cur), "Copying music track %d of the CD (%s)", T.second.Number, MB( (uint64_t)T.second.Sectors * RawSector ).c_str() );
		J.SetCurrent( std::string( Cur ) + " from " + T.first->Label );
		const std::string Err = RipTrack( J, *T.first, T.second, P.MusicDir + Name );
		if( Err.empty() ) { J.Ripped++; J.Add( std::string( "ripped CD track to " ) + P.MusicDir + Name + " from " + T.first->Label ); }
		else if( Err != "cancelled" ) { J.Failed++; J.Add( std::string( "FAILED " ) + Name + ": " + Err ); }
	}
	if( J.Cancel )
		J.Add( "cancelled" );
	EnterCriticalSection( &J.Lock );
	for( const std::string& L : J.Log )
		GStatus.push_back( L );
	LeaveCriticalSection( &J.Lock );
	InterlockedExchange( &J.Finished, 1 );
}

DWORD WINAPI JobThread( LPVOID P )
{
	RunJob( *(FJob*)P );
	return 0;
}

std::string Summary( const FJob& J )
{
	std::string S;
	char B[256];
	if( J.Copied )
	{
		snprintf( B, sizeof(B), "%d movie%s copied to %s\n", J.Copied, J.Copied == 1 ? "" : "s", J.Plan.MovieDir.c_str() );
		S += B;
	}
	if( J.Ripped )
	{
		snprintf( B, sizeof(B), "%d music track%s copied to %s\n", J.Ripped, J.Ripped == 1 ? "" : "s", J.Plan.MusicDir.c_str() );
		S += B;
	}
	if( J.Failed )
	{
		snprintf( B, sizeof(B), "\n%d file%s could not be copied:\n", J.Failed, J.Failed == 1 ? "" : "s" );
		S += B;
		for( const std::string& L : J.Log )
			if( L.compare( 0, 6, "FAILED" ) == 0 )
				S += "  " + L.substr( 7 ) + "\n";
	}
	if( J.Cancel )
		S += "\nStopped before the end. Files already copied are kept; the rest is copied next time.\n";
	if( S.empty() )
		S = "Nothing needed copying.";
	return S;
}

/*-----------------------------------------------------------------------------
	Window.
-----------------------------------------------------------------------------*/

enum { ID_COPY = IDOK, ID_CLOSE = IDCANCEL, ID_IMAGE = 300, ID_NOASK = 301, TIMER_SCAN = 1, TIMER_PROGRESS = 2 };

struct FVisualStyles
{
	HANDLE Ctx = INVALID_HANDLE_VALUE;
	ULONG_PTR Cookie = 0;
	FVisualStyles()
	{
		char Exe[MAX_PATH];
		GetModuleFileNameA( NULL, Exe, MAX_PATH );
		ACTCTXA AC;
		memset( &AC, 0, sizeof(AC) );
		AC.cbSize = sizeof(AC);
		AC.dwFlags = ACTCTX_FLAG_RESOURCE_NAME_VALID | ACTCTX_FLAG_HMODULE_VALID;
		AC.lpSource = Exe;
		AC.hModule = GetModuleHandleA( NULL );
		AC.lpResourceName = MAKEINTRESOURCEA( 2 );   // desktop/res/KHGLauncher.manifest
		Ctx = CreateActCtxA( &AC );
		if( Ctx != INVALID_HANDLE_VALUE && !ActivateActCtx( Ctx, &Cookie ) )
		{
			ReleaseActCtx( Ctx );
			Ctx = INVALID_HANDLE_VALUE;
		}
	}
	~FVisualStyles()
	{
		if( Ctx != INVALID_HANDLE_VALUE )
		{
			DeactivateActCtx( 0, Cookie );
			ReleaseActCtx( Ctx );
		}
	}
};

struct FWindow
{
	HWND Wnd = NULL, Owner = NULL;
	HWND Intro = NULL, SourceText = NULL, Bar = NULL, BarText = NULL, NoAsk = NULL, CopyBtn = NULL, ImageBtn = NULL, CloseBtn = NULL;
	HFONT Font = NULL, Bold = NULL;
	int Dpi = 96;
	bool AtStartup = false, Closed = false;
	std::string BaseDir, UserDir, IniPath;
	std::vector<FSource> Images;   // picked by the user
	std::vector<FSource> Drives;
	FPlan Plan;
	FJob* Job = nullptr;
	HANDLE Thread = NULL;

	int S( int V ) const { return MulDiv( V, Dpi, 96 ); }

	std::vector<FSource> Sources() const
	{
		std::vector<FSource> All = Images;
		All.insert( All.end(), Drives.begin(), Drives.end() );
		return All;
	}

	void Rescan()
	{
		Drives = ScanDrives( true );
		Refresh();
	}

	void Refresh()
	{
		const FKHGDiscNeeds Need = KHGDiscCheck( BaseDir.c_str(), UserDir.c_str(), IniPath.c_str() );
		std::vector<FSource> All = Sources();
		// The plan keeps pointers into the source list, so it is rebuilt from a list that lives in the window.
		static std::vector<FSource> Keep;
		Keep = All;
		Plan = MakePlan( BaseDir, UserDir, IniPath.c_str(), Keep );

		std::string T;
		const FSource* Disc2 = nullptr;
		for( const FSource& Src : Keep )
			if( Src.IsDisc2() && !Disc2 )
				Disc2 = &Src;
		for( const FSource& Src : Images )
			if( !Src.Error.empty() )
				T += "Can't use " + Src.Label + ": " + Src.Error + ".\n";
			else if( !Src.Useful() )
				T += Src.Label + " is not a Klingon Honor Guard disc image (no AVI folder).\n";
		if( Disc2 )
		{
			char B[160];
			snprintf( B, sizeof(B), "Found disc 2 (%s): %d movies, %d music tracks.", Disc2->Label.c_str(),
				(int)Disc2->Movies.size(), (int)Disc2->Tracks.size() );
			T += B;
			if( Disc2->Tracks.empty() && Need.Music )
				T += " No CD music found on it (a mounted .iso has none); use the disc itself or a .cue/.bin image for the music.";
		}
		else if( !Keep.empty() && Keep[0].Useful() )
			T += "Found disc 1 (" + Keep[0].Label + "); it only has the intro movie. Insert disc 2 for the rest.";
		else
			T += "Looking for disc 2 in your CD/DVD drives...";
		if( Plan.Movies.empty() && Plan.Tracks.empty() )
			T += ( Disc2 || !Keep.empty() ) ? "\nEverything on it is already in your game folder." : "";
		else
		{
			T += "\nTo copy: ";
			if( !Plan.Movies.empty() )
				T += std::to_string( Plan.Movies.size() ) + " movie" + ( Plan.Movies.size() == 1 ? "" : "s" ) + ( Plan.Tracks.empty() ? "" : " and " );
			if( !Plan.Tracks.empty() )
				T += std::to_string( Plan.Tracks.size() ) + " music track" + ( Plan.Tracks.size() == 1 ? "" : "s" );
			T += ", " + MB( Plan.Bytes ) + ".";
			if( !Plan.Tracks.empty() && Disc2 && Disc2->Drive )
				T += " Reading music from a disc takes a few minutes.";
		}
		SetWindowTextA( SourceText, T.c_str() );
		EnableWindow( CopyBtn, !Plan.Movies.empty() || !Plan.Tracks.empty() );
	}

	std::string IntroText() const
	{
		const FKHGDiscNeeds Need = KHGDiscCheck( BaseDir.c_str(), UserDir.c_str(), IniPath.c_str() );
		std::string T = "Klingon Honor Guard plays its movies and music from game disc 2, and the installer copies neither. "
			"KHGWin can copy them into your game folder once, so the game has them without the disc.\n\n";
		if( Need.Movies && Need.Music )
			T += "Missing now: the movies (about 450 MB) and the CD music (about 320 MB).";
		else if( Need.Movies )
			T += "Missing now: the movies (about 450 MB). The music is there.";
		else if( Need.Music )
			T += "Missing now: the CD music (about 320 MB). The movies are there.";
		else
			T += "Your game folder already has the movies and the music.";
		T += " Insert disc 2 or mount its image, or choose a disc image file.";
		return T;
	}

	HWND Child( const char* Class, const char* Text, DWORD Style, int X, int Y, int W, int H, int Id, HFONT F )
	{
		HWND C = CreateWindowExA( 0, Class, Text, WS_CHILD | WS_VISIBLE | Style, S( X ), S( Y ), S( W ), S( H ), Wnd,
			(HMENU)(INT_PTR)Id, GetModuleHandleA( NULL ), NULL );
		SendMessageA( C, WM_SETFONT, (WPARAM)F, TRUE );
		return C;
	}

	void Create()
	{
		const int CW = 540, CH = 284;
		RECT R = { 0, 0, S( CW ), S( CH ) };
		const DWORD Style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;
		AdjustWindowRectEx( &R, Style, FALSE, 0 );
		const int WW = R.right - R.left, WH = R.bottom - R.top;
		RECT Work;
		SystemParametersInfoA( SPI_GETWORKAREA, 0, &Work, 0 );
		Wnd = CreateWindowExA( WS_EX_CONTROLPARENT | ( Owner ? 0 : WS_EX_APPWINDOW ), "KHGDiscImport",
			"Copy movies and music from the game CD", Style,
			( Work.left + Work.right - WW ) / 2, ( Work.top + Work.bottom - WH ) / 2, WW, WH,
			Owner, NULL, GetModuleHandleA( NULL ), this );
		Intro = Child( "STATIC", IntroText().c_str(), SS_LEFT, 16, 14, CW - 32, 100, 0, Font );
		SourceText = Child( "STATIC", "", SS_LEFT, 16, 120, CW - 32, 58, 0, Bold );
		Bar = Child( PROGRESS_CLASSA, "", 0, 16, 184, CW - 32, 18, 0, Font );
		SendMessageA( Bar, PBM_SETRANGE32, 0, 1000 );
		ShowWindow( Bar, SW_HIDE );
		BarText = Child( "STATIC", "", SS_LEFT | SS_ENDELLIPSIS, 16, 206, CW - 32, 20, 0, Font );
		const int BY = CH - 42;
		if( AtStartup )
			NoAsk = Child( "BUTTON", "Don't ask again at startup", WS_TABSTOP | BS_AUTOCHECKBOX, 16, BY + 3, 200, 22, ID_NOASK, Font );
		ImageBtn = Child( "BUTTON", "Disc image...", WS_TABSTOP | BS_PUSHBUTTON, CW - 296, BY, 104, 28, ID_IMAGE, Font );
		CopyBtn = Child( "BUTTON", "Copy", WS_TABSTOP | BS_DEFPUSHBUTTON, CW - 184, BY, 80, 28, ID_COPY, Font );
		CloseBtn = Child( "BUTTON", AtStartup ? "Skip" : "Close", WS_TABSTOP | BS_PUSHBUTTON, CW - 96, BY, 80, 28, ID_CLOSE, Font );
		Rescan();
		SetTimer( Wnd, TIMER_SCAN, 2000, NULL );
		ShowWindow( Wnd, SW_SHOWNORMAL );
		SetForegroundWindow( Wnd );
		SetFocus( IsWindowEnabled( CopyBtn ) ? CopyBtn : ImageBtn );
	}

	void PickImage()
	{
		char File[MAX_PATH * 2] = "";
		OPENFILENAMEA O;
		memset( &O, 0, sizeof(O) );
		O.lStructSize = sizeof(O);
		O.hwndOwner = Wnd;
		O.lpstrFilter = "Disc images (*.cue, *.iso, *.bin)\0*.cue;*.iso;*.bin;*.img\0All files\0*.*\0";
		O.lpstrFile = File;
		O.nMaxFile = sizeof(File);
		O.lpstrTitle = "Klingon Honor Guard disc 2 image";
		O.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR;
		if( !GetOpenFileNameA( &O ) )
			return;
		FSource Src = OpenImage( File );
		for( auto It = Images.begin(); It != Images.end(); )
			It = ( It->Label == Src.Label ) ? Images.erase( It ) : It + 1;
		Images.insert( Images.begin(), Src );
		Refresh();
	}

	void StartCopy()
	{
		if( Plan.Movies.empty() && Plan.Tracks.empty() )
			return;
		KillTimer( Wnd, TIMER_SCAN );
		Job = new FJob;
		Job->BaseDir = BaseDir; Job->UserDir = UserDir; Job->IniPath = IniPath;
		Job->Sources = Sources();
		Job->Plan = MakePlan( BaseDir, UserDir, IniPath.c_str(), Job->Sources );
		EnableWindow( CopyBtn, FALSE );
		EnableWindow( ImageBtn, FALSE );
		if( NoAsk ) EnableWindow( NoAsk, FALSE );
		SetWindowTextA( CloseBtn, "Cancel" );
		ShowWindow( Bar, SW_SHOW );
		Thread = CreateThread( NULL, 0, JobThread, Job, 0, NULL );
		SetTimer( Wnd, TIMER_PROGRESS, 100, NULL );
	}

	void UpdateProgress()
	{
		if( !Job )
			return;
		EnterCriticalSection( &Job->Lock );
		const uint64_t Done = Job->Done, Total = std::max<uint64_t>( 1, Job->Plan.Bytes );
		const std::string Cur = Job->Current;
		LeaveCriticalSection( &Job->Lock );
		SendMessageA( Bar, PBM_SETPOS, (WPARAM)( Done * 1000 / Total ), 0 );
		const std::string T = Cur + "   " + MB( Done ) + " of " + MB( Total );
		SetWindowTextA( BarText, T.c_str() );
		if( Job->Finished )
		{
			KillTimer( Wnd, TIMER_PROGRESS );
			WaitForSingleObject( Thread, INFINITE );
			CloseHandle( Thread );
			Thread = NULL;
			const bool Ok = !Job->Failed && !Job->Cancel;
			MessageBoxA( Wnd, Summary( *Job ).c_str(), "Copy from the game CD", MB_OK | ( Ok ? MB_ICONINFORMATION : MB_ICONWARNING ) );
			delete Job;
			Job = nullptr;
			if( Ok )
				DestroyWindow( Wnd );
			else
			{
				// Let them try again (another disc, a cleaned disc).
				EnableWindow( ImageBtn, TRUE );
				if( NoAsk ) EnableWindow( NoAsk, TRUE );
				SetWindowTextA( CloseBtn, AtStartup ? "Skip" : "Close" );
				ShowWindow( Bar, SW_HIDE );
				SetWindowTextA( BarText, "" );
				SetWindowTextA( Intro, IntroText().c_str() );
				Rescan();
				SetTimer( Wnd, TIMER_SCAN, 2000, NULL );
			}
		}
	}

	void Close()
	{
		if( Job )
		{
			InterlockedExchange( &Job->Cancel, 1 );
			SetWindowTextA( BarText, "Stopping..." );
			return;
		}
		if( NoAsk && SendMessageA( NoAsk, BM_GETCHECK, 0, 0 ) == BST_CHECKED )
			WritePrivateProfileStringA( "KHGWin", "DiscPrompt", "False", IniPath.c_str() );
		DestroyWindow( Wnd );
	}

	static LRESULT CALLBACK StaticProc( HWND H, UINT Msg, WPARAM W, LPARAM L )
	{
		if( Msg == WM_NCCREATE )
			SetWindowLongPtrA( H, GWLP_USERDATA, (LONG_PTR)( (CREATESTRUCTA*)L )->lpCreateParams );
		FWindow* Me = (FWindow*)GetWindowLongPtrA( H, GWLP_USERDATA );
		if( Me )
		{
			Me->Wnd = H;
			return Me->Proc( Msg, W, L );
		}
		return DefWindowProcA( H, Msg, W, L );
	}

	LRESULT Proc( UINT Msg, WPARAM W, LPARAM L )
	{
		switch( Msg )
		{
		case WM_CTLCOLORSTATIC:
			SetBkMode( (HDC)W, TRANSPARENT );
			return (LRESULT)GetSysColorBrush( COLOR_WINDOW );
		case WM_ERASEBKGND:
		{
			RECT R;
			GetClientRect( Wnd, &R );
			FillRect( (HDC)W, &R, GetSysColorBrush( COLOR_WINDOW ) );
			return 1;
		}
		case WM_DEVICECHANGE:
			if( !Job ) Rescan();
			return TRUE;
		case WM_TIMER:
			if( W == TIMER_SCAN && !Job ) Rescan();
			if( W == TIMER_PROGRESS ) UpdateProgress();
			return 0;
		case WM_COMMAND:
			switch( LOWORD( W ) )
			{
			case ID_COPY: StartCopy(); return 0;
			case ID_IMAGE: PickImage(); return 0;
			case ID_CLOSE: Close(); return 0;
			}
			break;
		case WM_CLOSE:
			Close();
			return 0;
		case WM_DESTROY:
			Closed = true;
			return 0;
		}
		return DefWindowProcA( Wnd, Msg, W, L );
	}
};

int SystemDpi()
{
	HMODULE User = GetModuleHandleA( "user32.dll" );
	typedef UINT (WINAPI *FGetDpi)();
	FGetDpi GetDpi = User ? (FGetDpi)(void*)GetProcAddress( User, "GetDpiForSystem" ) : nullptr;
	if( GetDpi )
		return (int)GetDpi();
	HDC DC = GetDC( NULL );
	const int D = GetDeviceCaps( DC, LOGPIXELSY );
	ReleaseDC( NULL, DC );
	return D;
}

// Test and tool runs (-khgmaptest, -khgshots, ...) and -nodiscprompt, as the launcher decides it.
bool IsTestRun( const char* CmdLine )
{
	for( const char* P = CmdLine ? CmdLine : ""; *P; )
	{
		while( *P == ' ' || *P == '\t' )
			P++;
		const char* B = P;
		while( *P && *P != ' ' && *P != '\t' )
			P++;
		if( P > B + 1 && ( *B == '-' || *B == '/' ) )
		{
			const std::string A = Lower( std::string( B + 1, P ) );
			if( A.compare( 0, 3, "khg" ) == 0 || A == "nodiscprompt" )
				return true;
		}
	}
	return false;
}

void SetDpiAware()
{
	HMODULE User = GetModuleHandleA( "user32.dll" );
	typedef BOOL (WINAPI *FSetCtx)( HANDLE );
	FSetCtx SetCtx = User ? (FSetCtx)(void*)GetProcAddress( User, "SetProcessDpiAwarenessContext" ) : nullptr;
	if( !SetCtx || !SetCtx( (HANDLE)-4 /*DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2*/ ) )
		SetProcessDPIAware();
}

} // namespace

FKHGDiscNeeds KHGDiscCheck( const char* InBaseDir, const char* InUserDir, const char* IniPath )
{
	const std::string BaseDir = Slash( InBaseDir ? InBaseDir : "" ), UserDir = Slash( InUserDir ? InUserDir : "" );
	FKHGDiscNeeds N;
	N.Movies = !FileExists( BaseDir + KeyMovie ) && !FileExists( UserDir + "Movies\\" + KeyMovie );
	// Music: a ripped track 2 (every disc 2 has it; the menu and most missions use 2 to 8), or the fan
	// music pack that turns the CD tracks into Music\track2.umx and so on.
	const std::string Folder = MusicDir( BaseDir, IniPath );
	N.Music = !TrackPresent( Folder, UserDir, 2 ) && !FileExists( FullPath( BaseDir + "..\\Music\\track2.umx" ) );
	return N;
}

bool KHGDiscPromptWanted( const char* CmdLine, const char* BaseDir, const char* UserDir, const char* IniPath )
{
	if( IsTestRun( CmdLine ) || !IniBool( IniPath, "KHGWin", "DiscPrompt", true ) )
		return false;
	return KHGDiscCheck( BaseDir, UserDir, IniPath ).Any();
}

void KHGRunDiscImport( void* InOwner, const char* BaseDir, const char* UserDir, const char* IniPath, bool AtStartup )
{
	SetDpiAware();
	FVisualStyles Styles;
	HMODULE ComCtl = LoadLibraryA( "comctl32.dll" );
	typedef BOOL (WINAPI *FInit)( const INITCOMMONCONTROLSEX* );
	FInit Init = ComCtl ? (FInit)(void*)GetProcAddress( ComCtl, "InitCommonControlsEx" ) : nullptr;
	if( Init )
	{
		INITCOMMONCONTROLSEX ICC = { sizeof(ICC), ICC_PROGRESS_CLASS | ICC_STANDARD_CLASSES };
		Init( &ICC );
	}
	WNDCLASSEXA WC;
	memset( &WC, 0, sizeof(WC) );
	WC.cbSize = sizeof(WC);
	WC.lpfnWndProc = FWindow::StaticProc;
	WC.hInstance = GetModuleHandleA( NULL );
	WC.hIcon = LoadIconA( WC.hInstance, MAKEINTRESOURCEA( 128 ) );
	WC.hIconSm = WC.hIcon;
	WC.hCursor = LoadCursorA( NULL, (LPCSTR)IDC_ARROW );
	WC.lpszClassName = "KHGDiscImport";
	RegisterClassExA( &WC );

	FWindow W;
	W.Owner = (HWND)InOwner;
	W.AtStartup = AtStartup;
	W.BaseDir = Slash( BaseDir ? BaseDir : "" );
	W.UserDir = Slash( UserDir ? UserDir : "" );
	W.IniPath = IniPath ? IniPath : "";
	W.Dpi = SystemDpi();
	W.Font = CreateFontA( -MulDiv( 9, W.Dpi, 72 ), 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH, "Segoe UI" );
	W.Bold = CreateFontA( -MulDiv( 9, W.Dpi, 72 ), 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH, "Segoe UI" );
	if( W.Owner )
		EnableWindow( W.Owner, FALSE );
	W.Create();
	MSG Msg;
	while( !W.Closed && GetMessageA( &Msg, NULL, 0, 0 ) > 0 )
	{
		if( IsWindow( W.Wnd ) && IsDialogMessageA( W.Wnd, &Msg ) )
			continue;
		TranslateMessage( &Msg );
		DispatchMessageA( &Msg );
	}
	if( W.Owner )
	{
		EnableWindow( W.Owner, TRUE );
		SetForegroundWindow( W.Owner );
	}
	UnregisterClassA( "KHGDiscImport", GetModuleHandleA( NULL ) );
	DeleteObject( W.Font );
	DeleteObject( W.Bold );
}

bool KHGDiscImportCommandLine( const char* CmdLine, const char* BaseDir, const char* InUserDir, const char* IniPath, int* ExitCode )
{
	const std::string C = CmdLine ? CmdLine : "";
	const size_t P = Lower( C ).find( "-khgdiscimport=" );
	if( P == std::string::npos )
		return false;
	std::string Arg;
	size_t I = P + 15;
	if( I < C.size() && C[I] == '"' )
	{
		const size_t E = C.find( '"', I + 1 );
		Arg = C.substr( I + 1, E == std::string::npos ? std::string::npos : E - I - 1 );
	}
	else
		Arg = C.substr( I, C.find( ' ', I ) == std::string::npos ? std::string::npos : C.find( ' ', I ) - I );
	const std::string UserDir = Slash( InUserDir ? InUserDir : "" );
	FJob J;
	J.BaseDir = Slash( BaseDir ? BaseDir : "" );
	J.UserDir = UserDir;
	J.IniPath = IniPath ? IniPath : "";
	std::vector<std::string> Head;
	if( Lower( Arg ) == "auto" || ( Arg.size() == 2 && Arg[1] == ':' ) )
	{
		for( FSource& S : ScanDrives( true ) )
			if( Lower( Arg ) == "auto" || toupper( (unsigned char)S.Drive ) == toupper( (unsigned char)Arg[0] ) )
				J.Sources.push_back( S );
	}
	else
		J.Sources.push_back( OpenImage( Arg ) );
	for( const FSource& S : J.Sources )
	{
		char B[512];
		snprintf( B, sizeof(B), "source %s: %d movies%s, %d audio tracks%s%s", S.Label.c_str(), (int)S.Movies.size(),
			S.Intro.Name.empty() ? "" : " + INTRO.AVI", (int)S.Tracks.size(), S.Error.empty() ? "" : ", error: ", S.Error.c_str() );
		Head.push_back( B );
	}
	if( J.Sources.empty() )
		Head.push_back( "no source found for " + Arg );
	J.Plan = MakePlan( J.BaseDir, UserDir, J.IniPath.c_str(), J.Sources );
	RunJob( J );
	FILE* F = fopen( ( UserDir + "KHGDiscImport.log" ).c_str(), "wb" );
	if( F )
	{
		for( const std::string& L : Head )
			fprintf( F, "%s\r\n", L.c_str() );
		for( const std::string& L : J.Log )
			fprintf( F, "%s\r\n", L.c_str() );
		fprintf( F, "copied %d movies, %d tracks, %d failed\r\n", J.Copied, J.Ripped, J.Failed );
		fclose( F );
	}
	*ExitCode = ( J.Failed || J.Sources.empty() ) ? 1 : 0;
	return true;
}


/*-----------------------------------------------------------------------------
	Installing the game from disc 1.

	The retail installer (InstallShield 5, a 16-bit setup.exe) doesn't run on 64-bit Windows. Its
	data is DATA1.CAB in the root of disc 1; each file group goes to one folder of the install:
	Maps, Textures and Sounds to their folders, the program, script and ini files to System. The
	"Editor Shared DLLs" group (old copies of Windows system DLLs for UnrealEd) is left out, and
	INTRO.AVI from the disc's root is copied to System, as the installer's intro option did.
-----------------------------------------------------------------------------*/

namespace
{

// The install folder for a file group of DATA1.CAB, or "" to leave it out.
std::string GroupFolder( const std::string& Group )
{
	const std::string G = Lower( Group );
	if( G.find( "shared dll" ) != std::string::npos )
		return "";
	if( G.find( "map" ) != std::string::npos ) return "Maps";
	if( G.find( "texture" ) != std::string::npos ) return "Textures";
	if( G.find( "sound" ) != std::string::npos ) return "Sounds";
	if( G.find( "music" ) != std::string::npos ) return "Music";
	return "System";
}

// Creates a folder and any missing parents (C:\Games\Klingon Honor Guard\ on a fresh PC).
void MakeDirs( const std::string& Dir )
{
	for( size_t i = 3; i <= Dir.size(); i++ )
		if( i == Dir.size() || Dir[i] == '\\' || Dir[i] == '/' )
			CreateDirectoryA( Dir.substr( 0, i ).c_str(), NULL );
}

struct FInstallFile { int Index; std::string Dest; uint64_t Size; };

// Unpacks disc 1 into Target (a folder; made if missing). Fills J.Log; returns an error or "".
std::string RunInstall( FJob& J, const FSource& Src, const std::string& InTarget )
{
	const std::string Target = Slash( FullPath( InTarget ) );
	MakeDirs( Target );
	if( !DirExists( Target ) || !Writable( Target ) )
		return "can't write to " + Target;
	J.SetCurrent( "Reading the installer data on " + Src.Label );

	// From an image, DATA1.CAB is copied out first (unshield reads a file).
	std::string Cab = Src.Cab.Path, TempDir;
	if( !Src.Drive )
	{
		TempDir = Target + "KHGInstall.tmp\\";
		CreateDirectoryA( TempDir.c_str(), NULL );
		Cab = TempDir + "DATA1.CAB";
		J.SetCurrent( "Reading DATA1.CAB from " + Src.Label );
		const std::string Err = CopyMovie( J, Src, Src.Cab, Cab );
		if( !Err.empty() )
		{
			RemoveDirectoryA( TempDir.c_str() );
			return Err == "cancelled" ? Err : "can't read DATA1.CAB from " + Src.Label + ": " + Err;
		}
	}
	auto Cleanup = [&]()
	{
		if( !TempDir.empty() )
		{
			DeleteFileA( Cab.c_str() );
			RemoveDirectoryA( TempDir.c_str() );
		}
	};
	unshield_set_log_level( UNSHIELD_LOG_LEVEL_LOWEST );
	Unshield* U = unshield_open( Cab.c_str() );
	if( !U )
	{
		Cleanup();
		return "DATA1.CAB on " + Src.Label + " can't be opened (not the Klingon Honor Guard disc 1, or a damaged copy)";
	}
	std::vector<FInstallFile> Files;
	bool HasGame = false;
	uint64_t Bytes = 0;
	for( int g = 0; g < unshield_file_group_count( U ); ++g )
	{
		UnshieldFileGroup* G = unshield_file_group_get( U, g );
		if( !G || !G->name )
			continue;
		const std::string Folder = GroupFolder( G->name );
		if( Folder.empty() )
		{
			J.Add( std::string( "left out the file group \"" ) + G->name + "\" (UnrealEd's copies of Windows DLLs)" );
			continue;
		}
		for( unsigned i = G->first_file; i <= G->last_file; ++i )
		{
			if( !unshield_file_is_valid( U, (int)i ) || !unshield_file_name( U, (int)i ) )
				continue;
			FInstallFile F;
			F.Index = (int)i;
			F.Dest = Target + Folder + "\\" + unshield_file_name( U, (int)i );
			F.Size = unshield_file_size( U, (int)i );
			HasGame |= ( Lower( unshield_file_name( U, (int)i ) ) == "klingons.u" );
			Files.push_back( F );
			Bytes += F.Size;
		}
	}
	if( !HasGame )
	{
		unshield_close( U );
		Cleanup();
		return "the installer data on " + Src.Label + " has no Klingons.u; it is not Klingon Honor Guard disc 1";
	}
	EnterCriticalSection( &J.Lock );
	J.Plan.Bytes = J.Done + Bytes + ( Src.Intro.Name.empty() ? 0 : Src.Intro.Size ) + ( Src.Art.Name.empty() ? 0 : Src.Art.Size );
	LeaveCriticalSection( &J.Lock );
	for( const char* Sub : { "System", "Maps", "Textures", "Sounds", "Save" } )
		CreateDirectoryA( ( Target + Sub ).c_str(), NULL );
	std::string Err;
	int Count = 0;
	for( const FInstallFile& F : Files )
	{
		if( J.Cancel ) { Err = "cancelled"; break; }
		CreateDirectoryA( F.Dest.substr( 0, F.Dest.find_last_of( '\\' ) ).c_str(), NULL );
		J.SetCurrent( "Installing " + F.Dest.substr( Target.size() ) );
		if( !unshield_file_save( U, F.Index, F.Dest.c_str() ) )
		{
			Err = "can't unpack " + F.Dest.substr( Target.size() ) + " (disk full, or a read error on the disc)";
			break;
		}
		J.Progress( F.Size );
		Count++;
	}
	unshield_close( U );
	Cleanup();
	if( Err.empty() )
	{
		char B[160];
		snprintf( B, sizeof(B), "installed %d files (%s) from %s to ", Count, MB( Bytes ).c_str(), Src.Label.c_str() );
		J.Add( B + Target );
		if( !Src.Intro.Name.empty() && !J.Cancel )
		{
			J.SetCurrent( "Copying INTRO.AVI" );
			const std::string E = CopyMovie( J, Src, Src.Intro, Target + "System\\INTRO.AVI" );
			if( E.empty() ) J.Add( "copied INTRO.AVI to " + Target + "System\\" );
			else if( E != "cancelled" ) J.Add( "FAILED INTRO.AVI: " + E );
		}
		// [KHG] The launcher's picture: version 1.0 has no MicroProse splash in its exe (patch 1.1 adds it),
		// so the autorun menu's backdrop stands in (KHGLauncher.cpp). Not needed by the game.
		if( !Src.Art.Name.empty() && !J.Cancel
		 && CopyMovie( J, Src, Src.Art, Target + "System\\KHGLauncher.bmp" ).empty() )
			J.Add( "copied the disc's autorun backdrop to " + Target + "System\\KHGLauncher.bmp (launcher picture)" );
	}
	return J.Cancel ? "cancelled" : Err;
}

std::string DefaultInstallDir()
{
	char Drive[MAX_PATH] = "C:";
	GetEnvironmentVariableA( "SystemDrive", Drive, sizeof(Drive) );
	return std::string( Drive ) + "\\Games\\Klingon Honor Guard";
}

// The first disc 1 source: a picked image, else a drive.
const FSource* Disc1Of( const std::vector<FSource>& Images, const std::vector<FSource>& Drives )
{
	for( const auto* L : { &Images, &Drives } )
		for( const FSource& S : *L )
			if( !S.Cab.Name.empty() && S.Error.empty() )
				return &S;
	return nullptr;
}

enum { ID_INSTALL = IDOK, ID_IQUIT = IDCANCEL, ID_IIMAGE = 400, ID_ICHANGE = 401, ID_ILOCATE = 402, ID_IDIR = 403 };

struct FInstallWindow
{
	HWND Wnd = NULL, Intro = NULL, SourceText = NULL, DirEdit = NULL, Bar = NULL, BarText = NULL;
	HWND InstallBtn = NULL, ImageBtn = NULL, ChangeBtn = NULL, LocateBtn = NULL, QuitBtn = NULL;
	HFONT Font = NULL, Bold = NULL;
	int Dpi = 96;
	bool Closed = false;
	int Result = 0;            // 0 quit, 1 installed, 2 locate
	std::string Installed;     // the new install's System folder
	std::vector<FSource> Images, Drives;
	FJob* Job = nullptr;
	HANDLE Thread = NULL;
	FSource JobSource;
	std::string JobTarget, JobError;

	int S( int V ) const { return MulDiv( V, Dpi, 96 ); }

	HWND Child( const char* Class, const char* Text, DWORD Style, int X, int Y, int W, int H, int Id, HFONT F, DWORD Ex = 0 )
	{
		HWND C = CreateWindowExA( Ex, Class, Text, WS_CHILD | WS_VISIBLE | Style, S( X ), S( Y ), S( W ), S( H ), Wnd,
			(HMENU)(INT_PTR)Id, GetModuleHandleA( NULL ), NULL );
		SendMessageA( C, WM_SETFONT, (WPARAM)F, TRUE );
		return C;
	}

	void Refresh()
	{
		std::string T;
		for( const FSource& Src : Images )
			if( !Src.Error.empty() )
				T += "Can't use " + Src.Label + ": " + Src.Error + ".\n";
			else if( Src.Cab.Name.empty() )
				T += Src.Label + " is not Klingon Honor Guard disc 1 (no DATA1.CAB).\n";
		const FSource* D = Disc1Of( Images, Drives );
		if( D )
			T += "Found disc 1 (" + D->Label + "). The game takes about 300 MB.";
		else
			T += "Looking for disc 1 in your CD/DVD drives...";
		SetWindowTextA( SourceText, T.c_str() );
		EnableWindow( InstallBtn, D != nullptr );
	}

	void Create()
	{
		const int CW = 560, CH = 300;
		RECT R = { 0, 0, S( CW ), S( CH ) };
		const DWORD Style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
		AdjustWindowRectEx( &R, Style, FALSE, 0 );
		const int WW = R.right - R.left, WH = R.bottom - R.top;
		RECT Work;
		SystemParametersInfoA( SPI_GETWORKAREA, 0, &Work, 0 );
		Wnd = CreateWindowExA( WS_EX_CONTROLPARENT | WS_EX_APPWINDOW, "KHGInstall", "Install Klingon Honor Guard", Style,
			( Work.left + Work.right - WW ) / 2, ( Work.top + Work.bottom - WH ) / 2, WW, WH, NULL, NULL, GetModuleHandleA( NULL ), this );
		Intro = Child( "STATIC",
			"KHGWin runs the game from your own Klingon Honor Guard CD, and it found no installed copy.\n\n"
			"The game's 1998 installer doesn't run on 64-bit Windows, so KHGWin can install the game itself: "
			"insert disc 1 (or mount its image) or choose a disc image file, pick a folder and click Install. "
			"If the game is already installed, click \"Installed game...\" and choose its folder.",
			SS_LEFT, 16, 14, CW - 32, 92, 0, Font );
		SourceText = Child( "STATIC", "", SS_LEFT, 16, 112, CW - 32, 40, 0, Bold );
		Child( "STATIC", "Install to:", SS_LEFT, 16, 160, 70, 20, 0, Font );
		DirEdit = Child( "EDIT", DefaultInstallDir().c_str(), WS_TABSTOP | ES_AUTOHSCROLL, 88, 157, CW - 88 - 112, 24, ID_IDIR, Font, WS_EX_CLIENTEDGE );
		ChangeBtn = Child( "BUTTON", "Change...", WS_TABSTOP | BS_PUSHBUTTON, CW - 104, 156, 88, 26, ID_ICHANGE, Font );
		Bar = Child( PROGRESS_CLASSA, "", 0, 16, 196, CW - 32, 18, 0, Font );
		SendMessageA( Bar, PBM_SETRANGE32, 0, 1000 );
		ShowWindow( Bar, SW_HIDE );
		BarText = Child( "STATIC", "", SS_LEFT | SS_ENDELLIPSIS, 16, 218, CW - 32, 20, 0, Font );
		const int BY = CH - 42;
		LocateBtn = Child( "BUTTON", "Installed game...", WS_TABSTOP | BS_PUSHBUTTON, 16, BY, 124, 28, ID_ILOCATE, Font );
		ImageBtn = Child( "BUTTON", "Disc image...", WS_TABSTOP | BS_PUSHBUTTON, CW - 296, BY, 104, 28, ID_IIMAGE, Font );
		InstallBtn = Child( "BUTTON", "Install", WS_TABSTOP | BS_DEFPUSHBUTTON, CW - 184, BY, 80, 28, ID_INSTALL, Font );
		QuitBtn = Child( "BUTTON", "Quit", WS_TABSTOP | BS_PUSHBUTTON, CW - 96, BY, 80, 28, ID_IQUIT, Font );
		Drives = ScanDrives( false );
		Refresh();
		SetTimer( Wnd, TIMER_SCAN, 2000, NULL );
		ShowWindow( Wnd, SW_SHOWNORMAL );
		SetForegroundWindow( Wnd );
		SetFocus( IsWindowEnabled( InstallBtn ) ? InstallBtn : ImageBtn );
	}

	void PickImage()
	{
		char File[MAX_PATH * 2] = "";
		OPENFILENAMEA O;
		memset( &O, 0, sizeof(O) );
		O.lStructSize = sizeof(O);
		O.hwndOwner = Wnd;
		O.lpstrFilter = "Disc images (*.cue, *.iso, *.bin)\0*.cue;*.iso;*.bin;*.img\0All files\0*.*\0";
		O.lpstrFile = File;
		O.nMaxFile = sizeof(File);
		O.lpstrTitle = "Klingon Honor Guard disc 1 image";
		O.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR;
		if( !GetOpenFileNameA( &O ) )
			return;
		Images.clear();
		Images.push_back( OpenImage( File ) );
		Refresh();
	}

	void ChangeDir()
	{
		BROWSEINFOA BI;
		memset( &BI, 0, sizeof(BI) );
		BI.hwndOwner = Wnd;
		BI.lpszTitle = "Folder to install Klingon Honor Guard in (a new or empty folder)";
		BI.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
		LPITEMIDLIST Pidl = SHBrowseForFolderA( &BI );
		if( !Pidl )
			return;
		char Path[MAX_PATH] = "";
		if( SHGetPathFromIDListA( Pidl, Path ) )
			SetWindowTextA( DirEdit, Path );
		CoTaskMemFree( Pidl );
	}

	static DWORD WINAPI InstallThread( LPVOID P )
	{
		FInstallWindow* W = (FInstallWindow*)P;
		W->JobError = RunInstall( *W->Job, W->JobSource, W->JobTarget );
		InterlockedExchange( &W->Job->Finished, 1 );
		return 0;
	}

	void StartInstall()
	{
		const FSource* D = Disc1Of( Images, Drives );
		if( !D )
			return;
		char Dir[MAX_PATH * 2] = "";
		GetWindowTextA( DirEdit, Dir, sizeof(Dir) );
		JobTarget = FullPath( Dir );
		if( FileExists( Slash( JobTarget ) + "System\\Engine.u" ) )
		{
			Installed = Slash( JobTarget ) + "System\\";
			Result = 1;
			MessageBoxA( Wnd, ( JobTarget + "\n\nThe game is already installed there. KHGWin uses that copy." ).c_str(),
				"Install Klingon Honor Guard", MB_OK | MB_ICONINFORMATION );
			DestroyWindow( Wnd );
			return;
		}
		KillTimer( Wnd, TIMER_SCAN );
		JobSource = *D;
		Job = new FJob;
		for( HWND H : { InstallBtn, ImageBtn, ChangeBtn, LocateBtn, DirEdit } )
			EnableWindow( H, FALSE );
		SetWindowTextA( QuitBtn, "Cancel" );
		ShowWindow( Bar, SW_SHOW );
		Thread = CreateThread( NULL, 0, InstallThread, this, 0, NULL );
		SetTimer( Wnd, TIMER_PROGRESS, 100, NULL );
	}

	void UpdateProgress()
	{
		if( !Job )
			return;
		EnterCriticalSection( &Job->Lock );
		const uint64_t Done = Job->Done, Total = std::max<uint64_t>( 1, Job->Plan.Bytes ? Job->Plan.Bytes : JobSource.Cab.Size );
		const std::string Cur = Job->Current;
		LeaveCriticalSection( &Job->Lock );
		SendMessageA( Bar, PBM_SETPOS, (WPARAM)std::min<uint64_t>( 1000, Done * 1000 / Total ), 0 );
		SetWindowTextA( BarText, Cur.c_str() );
		if( !Job->Finished )
			return;
		KillTimer( Wnd, TIMER_PROGRESS );
		WaitForSingleObject( Thread, INFINITE );
		CloseHandle( Thread );
		Thread = NULL;
		for( const std::string& L : Job->Log )
			GStatus.push_back( L );
		const bool Cancelled = Job->Cancel != 0;
		delete Job;
		Job = nullptr;
		if( JobError.empty() )
		{
			Installed = Slash( JobTarget ) + "System\\";
			Result = 1;
			GStatus.push_back( "game installed from disc 1 in " + JobTarget );
			DestroyWindow( Wnd );
			return;
		}
		if( !Cancelled )
			MessageBoxA( Wnd, ( "The game could not be installed:\n\n" + JobError ).c_str(), "Install Klingon Honor Guard", MB_OK | MB_ICONWARNING );
		for( HWND H : { InstallBtn, ImageBtn, ChangeBtn, LocateBtn, DirEdit } )
			EnableWindow( H, TRUE );
		SetWindowTextA( QuitBtn, "Quit" );
		ShowWindow( Bar, SW_HIDE );
		SetWindowTextA( BarText, Cancelled ? "Stopped. The files unpacked so far are still in the folder." : "" );
		Refresh();
		SetTimer( Wnd, TIMER_SCAN, 2000, NULL );
	}

	static LRESULT CALLBACK StaticProc( HWND H, UINT Msg, WPARAM W, LPARAM L )
	{
		if( Msg == WM_NCCREATE )
			SetWindowLongPtrA( H, GWLP_USERDATA, (LONG_PTR)( (CREATESTRUCTA*)L )->lpCreateParams );
		FInstallWindow* Me = (FInstallWindow*)GetWindowLongPtrA( H, GWLP_USERDATA );
		if( Me )
		{
			Me->Wnd = H;
			return Me->Proc( Msg, W, L );
		}
		return DefWindowProcA( H, Msg, W, L );
	}

	LRESULT Proc( UINT Msg, WPARAM W, LPARAM L )
	{
		switch( Msg )
		{
		case WM_CTLCOLORSTATIC:
			SetBkMode( (HDC)W, TRANSPARENT );
			return (LRESULT)GetSysColorBrush( COLOR_WINDOW );
		case WM_ERASEBKGND:
		{
			RECT R;
			GetClientRect( Wnd, &R );
			FillRect( (HDC)W, &R, GetSysColorBrush( COLOR_WINDOW ) );
			return 1;
		}
		case WM_DEVICECHANGE:
			if( !Job ) { Drives = ScanDrives( false ); Refresh(); }
			return TRUE;
		case WM_TIMER:
			if( W == TIMER_SCAN && !Job ) { Drives = ScanDrives( false ); Refresh(); }
			if( W == TIMER_PROGRESS ) UpdateProgress();
			return 0;
		case WM_COMMAND:
			switch( LOWORD( W ) )
			{
			case ID_INSTALL: StartInstall(); return 0;
			case ID_IIMAGE: PickImage(); return 0;
			case ID_ICHANGE: ChangeDir(); return 0;
			case ID_ILOCATE: Result = 2; DestroyWindow( Wnd ); return 0;
			case ID_IQUIT:
				if( Job ) { InterlockedExchange( &Job->Cancel, 1 ); SetWindowTextA( BarText, "Stopping..." ); }
				else DestroyWindow( Wnd );
				return 0;
			}
			break;
		case WM_CLOSE:
			if( Job ) InterlockedExchange( &Job->Cancel, 1 );
			else DestroyWindow( Wnd );
			return 0;
		case WM_DESTROY:
			Closed = true;
			return 0;
		}
		return DefWindowProcA( Wnd, Msg, W, L );
	}
};

} // namespace

int KHGRunGameInstall( std::string* SystemDir )
{
	SetDpiAware();
	FVisualStyles Styles;
	HMODULE ComCtl = LoadLibraryA( "comctl32.dll" );
	typedef BOOL (WINAPI *FInit)( const INITCOMMONCONTROLSEX* );
	FInit Init = ComCtl ? (FInit)(void*)GetProcAddress( ComCtl, "InitCommonControlsEx" ) : nullptr;
	if( Init )
	{
		INITCOMMONCONTROLSEX ICC = { sizeof(ICC), ICC_PROGRESS_CLASS | ICC_STANDARD_CLASSES };
		Init( &ICC );
	}
	CoInitializeEx( NULL, COINIT_APARTMENTTHREADED );   // the folder dialog
	WNDCLASSEXA WC;
	memset( &WC, 0, sizeof(WC) );
	WC.cbSize = sizeof(WC);
	WC.lpfnWndProc = FInstallWindow::StaticProc;
	WC.hInstance = GetModuleHandleA( NULL );
	WC.hIcon = LoadIconA( WC.hInstance, MAKEINTRESOURCEA( 128 ) );
	WC.hIconSm = WC.hIcon;
	WC.hCursor = LoadCursorA( NULL, (LPCSTR)IDC_ARROW );
	WC.lpszClassName = "KHGInstall";
	RegisterClassExA( &WC );
	FInstallWindow W;
	W.Dpi = SystemDpi();
	W.Font = CreateFontA( -MulDiv( 9, W.Dpi, 72 ), 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH, "Segoe UI" );
	W.Bold = CreateFontA( -MulDiv( 9, W.Dpi, 72 ), 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH, "Segoe UI" );
	W.Create();
	MSG Msg;
	while( !W.Closed && GetMessageA( &Msg, NULL, 0, 0 ) > 0 )
	{
		if( IsWindow( W.Wnd ) && IsDialogMessageA( W.Wnd, &Msg ) )
			continue;
		TranslateMessage( &Msg );
		DispatchMessageA( &Msg );
	}
	UnregisterClassA( "KHGInstall", GetModuleHandleA( NULL ) );
	DeleteObject( W.Font );
	DeleteObject( W.Bold );
	if( SystemDir )
		*SystemDir = W.Installed;
	return W.Result;
}

bool KHGInstallCommandLine( const char* CmdLine, int* ExitCode )
{
	const std::string C = CmdLine ? CmdLine : "";
	auto Arg = [&]( const char* Key ) -> std::string
	{
		const size_t P = Lower( C ).find( Key );
		if( P == std::string::npos )
			return "";
		size_t I = P + strlen( Key );
		if( I < C.size() && C[I] == '"' )
		{
			const size_t E = C.find( '"', I + 1 );
			return C.substr( I + 1, E == std::string::npos ? std::string::npos : E - I - 1 );
		}
		const size_t E = C.find( ' ', I );
		return C.substr( I, E == std::string::npos ? std::string::npos : E - I );
	};
	const std::string From = Arg( "-khginstall=" );
	if( From.empty() )
		return false;
	std::string To = Arg( "-khginstalldir=" );
	if( To.empty() )
		To = DefaultInstallDir();
	std::vector<FSource> Images, Drives;
	if( From.size() == 2 && From[1] == ':' )
	{
		for( FSource& S : ScanDrives( false ) )
			if( toupper( (unsigned char)S.Drive ) == toupper( (unsigned char)From[0] ) )
				Drives.push_back( S );
	}
	else
		Images.push_back( OpenImage( From ) );
	const FSource* D = Disc1Of( Images, Drives );
	FJob J;
	const std::string Err = D ? RunInstall( J, *D, To ) : "no disc 1 found in " + From;
	MakeDirs( Slash( FullPath( To ) ) );
	FILE* F = fopen( ( Slash( To ) + "KHGInstall.log" ).c_str(), "wb" );
	if( F )
	{
		for( const std::string& L : J.Log )
			fprintf( F, "%s\r\n", L.c_str() );
		fprintf( F, "%s\r\n", Err.empty() ? "done" : ( "ERROR: " + Err ).c_str() );
		fclose( F );
	}
	*ExitCode = Err.empty() ? 0 : 1;
	return true;
}

// The engine log functions live in Core; declared here to keep this file free of engine headers.
void KHGDiscImportLogLine( const char* Line );

void KHGDiscImportLogStatus()
{
	for( const std::string& L : GStatus )
		KHGDiscImportLogLine( L.c_str() );
	GStatus.clear();
}
