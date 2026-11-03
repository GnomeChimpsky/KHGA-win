/*=============================================================================
	KHGLauncher.cpp: settings window shown when KHGWin.exe starts (desktop build).

	Plain Win32 (no engine, no SDL): it runs after KHGWin.ini has been prepared
	and before appInit() reads it. Only the settings the game's own menus can't
	change are offered (see the retail KlingonMenuVideo / KlingonMenuOptions).
	Only keys whose control was changed are written, so untouched settings keep
	following the port's defaults.
=============================================================================*/

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <commctrl.h>
#include <shellapi.h>

#ifndef VK_NO_PROTOTYPES
#define VK_NO_PROTOTYPES
#endif
#include <vulkan/vulkan.h>

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

#include "KHGLauncher.h"
#include "KHGSplash.h"

namespace
{

/*-----------------------------------------------------------------------------
	Ini file: kept as lines so everything we don't touch stays byte for byte.
-----------------------------------------------------------------------------*/

std::string Trim( const std::string& S )
{
	size_t B = 0, E = S.size();
	while( B < E && ( S[B] == ' ' || S[B] == '\t' ) ) B++;
	while( E > B && ( S[E-1] == ' ' || S[E-1] == '\t' || S[E-1] == '\r' ) ) E--;
	return S.substr( B, E - B );
}

bool Same( const std::string& A, const char* B )
{
	return _stricmp( A.c_str(), B ) == 0;
}

struct FIniFile
{
	std::vector<std::string> Lines;
	bool Loaded = false;

	bool Load( const char* Path )
	{
		FILE* F = fopen( Path, "rb" );
		if( !F )
			return false;
		std::string Text;
		char Buf[16384];
		size_t N;
		while( ( N = fread( Buf, 1, sizeof(Buf), F ) ) > 0 )
			Text.append( Buf, N );
		fclose( F );
		size_t Pos = 0;
		while( Pos < Text.size() )
		{
			size_t End = Text.find( '\n', Pos );
			if( End == std::string::npos )
				End = Text.size();
			std::string Line = Text.substr( Pos, End - Pos );
			if( !Line.empty() && Line.back() == '\r' )
				Line.pop_back();
			Lines.push_back( Line );
			Pos = End + 1;
		}
		Loaded = true;
		return true;
	}

	bool Save( const char* Path ) const
	{
		std::string Text;
		for( const std::string& L : Lines )
			Text += L + "\r\n";
		FILE* F = fopen( Path, "wb" );
		if( !F )
			return false;
		const bool Ok = fwrite( Text.data(), 1, Text.size(), F ) == Text.size();
		return fclose( F ) == 0 && Ok;
	}

	static bool IsSection( const std::string& Line, std::string* Name )
	{
		const std::string T = Trim( Line );
		if( T.size() < 2 || T[0] != '[' || T.back() != ']' )
			return false;
		if( Name )
			*Name = T.substr( 1, T.size() - 2 );
		return true;
	}

	static bool KeyOf( const std::string& Line, std::string& Key, std::string& Value )
	{
		const std::string T = Trim( Line );
		if( T.empty() || T[0] == ';' || T[0] == '[' )
			return false;
		const size_t Eq = T.find( '=' );
		if( Eq == std::string::npos )
			return false;
		Key = Trim( T.substr( 0, Eq ) );
		Value = Trim( T.substr( Eq + 1 ) );
		return true;
	}

	// Line index of Section.Key, or -1. SectionEnd gets the index after the section's last
	// non-blank line (-1 when the section is missing).
	int Find( const char* Section, const char* Key, int* SectionEnd = nullptr ) const
	{
		bool In = false;
		int Found = -1, End = -1;
		for( int i = 0; i < (int)Lines.size(); ++i )
		{
			std::string Name;
			if( IsSection( Lines[i], &Name ) )
			{
				In = Same( Name, Section );
				if( In && End < 0 )
					End = i + 1;
				continue;
			}
			if( !In )
				continue;
			if( !Trim( Lines[i] ).empty() )
				End = i + 1;
			std::string K, V;
			if( Found < 0 && KeyOf( Lines[i], K, V ) && Same( K, Key ) )
				Found = i;
		}
		if( SectionEnd )
			*SectionEnd = End;
		return Found;
	}

	bool Get( const char* Section, const char* Key, std::string& Out ) const
	{
		const int i = Find( Section, Key );
		std::string K;
		return i >= 0 && KeyOf( Lines[i], K, Out );
	}

	void Set( const char* Section, const char* Key, const std::string& Value )
	{
		int End = -1;
		const int i = Find( Section, Key, &End );
		const std::string Line = std::string( Key ) + "=" + Value;
		if( i >= 0 )
			Lines[i] = Line;
		else if( End >= 0 )
			Lines.insert( Lines.begin() + End, Line );
		else
		{
			if( !Lines.empty() && !Trim( Lines.back() ).empty() )
				Lines.push_back( "" );
			Lines.push_back( std::string( "[" ) + Section + "]" );
			Lines.push_back( Line );
		}
	}
};

bool ParseBool( const std::string& V, bool Default )
{
	if( V.empty() )
		return Default;
	return _stricmp( V.c_str(), "true" ) == 0 || _stricmp( V.c_str(), "1" ) == 0
	    || _stricmp( V.c_str(), "yes" ) == 0 || _stricmp( V.c_str(), "on" ) == 0;
}

const char* const SecClient = "NSDLDrv.NSDLClient";
const char* const SecVulkan = "VulkanDrv.VulkanRenderDevice";
const char* const SecAudio  = "NOpenALDrv.NOpenALAudioSubsystem";
const char* const SecKHG    = "KHGWin";

/*-----------------------------------------------------------------------------
	What the system offers: display modes, Vulkan GPUs, OpenAL devices.
-----------------------------------------------------------------------------*/

struct FMode { int W, H; };

std::vector<FMode> DisplayModes( FMode& Desktop )
{
	std::vector<FMode> Modes;
	DEVMODEA DM;
	memset( &DM, 0, sizeof(DM) );
	DM.dmSize = sizeof(DM);
	for( DWORD i = 0; EnumDisplaySettingsA( NULL, i, &DM ); ++i )
	{
		if( DM.dmPelsWidth < 640 || DM.dmPelsHeight < 480 || DM.dmBitsPerPel < 16 )
			continue;
		FMode M = { (int)DM.dmPelsWidth, (int)DM.dmPelsHeight };
		bool Have = false;
		for( const FMode& O : Modes )
			Have |= O.W == M.W && O.H == M.H;
		if( !Have )
			Modes.push_back( M );
	}
	Desktop.W = GetSystemMetrics( SM_CXSCREEN );
	Desktop.H = GetSystemMetrics( SM_CYSCREEN );
	if( EnumDisplaySettingsA( NULL, ENUM_CURRENT_SETTINGS, &DM ) )
	{
		Desktop.W = DM.dmPelsWidth;
		Desktop.H = DM.dmPelsHeight;
	}
	std::sort( Modes.begin(), Modes.end(), []( const FMode& A, const FMode& B )
		{ return A.W != B.W ? A.W > B.W : A.H > B.H; } );
	return Modes;
}

// GPU names in the order VulkanDrv's VkDeviceIndex counts them (ZVulkan VulkanDeviceBuilder::FindDevices
// sorts discrete > integrated > virtual > CPU, then by pipeline cache UUID).
std::vector<std::string> VulkanDevices()
{
	std::vector<std::string> Names;
	HMODULE Lib = LoadLibraryA( "vulkan-1.dll" );
	if( !Lib )
		return Names;
	PFN_vkGetInstanceProcAddr GetProc = (PFN_vkGetInstanceProcAddr)(void*)GetProcAddress( Lib, "vkGetInstanceProcAddr" );
	PFN_vkCreateInstance CreateInstance = GetProc ? (PFN_vkCreateInstance)GetProc( VK_NULL_HANDLE, "vkCreateInstance" ) : nullptr;
	if( CreateInstance )
	{
		VkApplicationInfo App = {};
		App.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		App.pApplicationName = "KHGWin launcher";
		App.apiVersion = VK_API_VERSION_1_0;
		VkInstanceCreateInfo Info = {};
		Info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		Info.pApplicationInfo = &App;
		VkInstance Instance = VK_NULL_HANDLE;
		if( CreateInstance( &Info, nullptr, &Instance ) == VK_SUCCESS )
		{
			PFN_vkEnumeratePhysicalDevices Enum = (PFN_vkEnumeratePhysicalDevices)GetProc( Instance, "vkEnumeratePhysicalDevices" );
			PFN_vkGetPhysicalDeviceProperties Props = (PFN_vkGetPhysicalDeviceProperties)GetProc( Instance, "vkGetPhysicalDeviceProperties" );
			PFN_vkDestroyInstance Destroy = (PFN_vkDestroyInstance)GetProc( Instance, "vkDestroyInstance" );
			uint32_t Count = 0;
			if( Enum && Props && Enum( Instance, &Count, nullptr ) == VK_SUCCESS && Count )
			{
				std::vector<VkPhysicalDevice> Devices( Count );
				Enum( Instance, &Count, Devices.data() );
				std::vector<VkPhysicalDeviceProperties> P( Count );
				for( uint32_t i = 0; i < Count; ++i )
					Props( Devices[i], &P[i] );
				static const int TypeSort[] = { 4, 1, 0, 2, 3 };
				auto Rank = []( const VkPhysicalDeviceProperties& D )
					{ return D.deviceType < 5 ? TypeSort[D.deviceType] : (int)D.deviceType; };
				std::stable_sort( P.begin(), P.end(), [&]( const VkPhysicalDeviceProperties& A, const VkPhysicalDeviceProperties& B )
					{
						if( Rank( A ) != Rank( B ) )
							return Rank( A ) < Rank( B );
						return memcmp( A.pipelineCacheUUID, B.pipelineCacheUUID, VK_UUID_SIZE ) < 0;
					} );
				for( const VkPhysicalDeviceProperties& D : P )
					Names.push_back( D.deviceName );
			}
			if( Destroy )
				Destroy( Instance, nullptr );
		}
	}
	FreeLibrary( Lib );
	return Names;
}

// OpenAL output device names (UTF-8), as alcOpenDevice() takes them.
std::vector<std::string> AudioDevices( const std::string& BaseDir )
{
	std::vector<std::string> Names;
	typedef const char* (__cdecl *FGetString)( void*, int );
	typedef char        (__cdecl *FIsExt)( void*, const char* );
	HMODULE Lib = LoadLibraryA( ( BaseDir + "OpenAL32.dll" ).c_str() );
	if( !Lib )
		Lib = LoadLibraryA( "OpenAL32.dll" );
	if( !Lib )
		return Names;
	FGetString GetString = (FGetString)(void*)GetProcAddress( Lib, "alcGetString" );
	FIsExt IsExt = (FIsExt)(void*)GetProcAddress( Lib, "alcIsExtensionPresent" );
	if( GetString )
	{
		const int ALC_DEVICE_SPECIFIER = 0x1005, ALC_ALL_DEVICES_SPECIFIER = 0x1013;
		const bool All = IsExt && IsExt( nullptr, "ALC_ENUMERATE_ALL_EXT" );
		const char* List = GetString( nullptr, All ? ALC_ALL_DEVICES_SPECIFIER : ALC_DEVICE_SPECIFIER );
		for( const char* P = List; P && *P; P += strlen( P ) + 1 )
			Names.push_back( P );
	}
	FreeLibrary( Lib );
	return Names;
}

std::wstring Wide( const std::string& S, UINT CodePage = CP_UTF8 )
{
	if( S.empty() )
		return std::wstring();
	int N = MultiByteToWideChar( CodePage, 0, S.c_str(), (int)S.size(), nullptr, 0 );
	if( N <= 0 )
		return Wide( S, CP_ACP );
	std::wstring W( N, L'\0' );
	MultiByteToWideChar( CodePage, 0, S.c_str(), (int)S.size(), &W[0], N );
	return W;
}

/*-----------------------------------------------------------------------------
	The window.
-----------------------------------------------------------------------------*/

enum EKind { K_Check, K_Combo, K_Slider };
enum ESpecial { SP_None, SP_DisplayMode, SP_Resolution, SP_WindowSize };

struct FItem
{
	int Page = 0;
	EKind Kind = K_Check;
	ESpecial Special = SP_None;
	const char* Section = nullptr;
	const char* Key = nullptr;
	const char* Label = nullptr;
	const char* Tip = nullptr;
	HWND LabelWnd = NULL, Ctrl = NULL, ValueWnd = NULL;
	// Check
	bool InitialCheck = false;
	// Combo: what each entry writes
	std::vector<std::string> Values;
	int InitialIndex = 0;
	// Slider: ini value = Base + Pos * Scale (positions start at 0; some trackbars mishandle negative ranges)
	int Max = 100, InitialPos = 0;
	float Base = 0.f, Scale = 1.f;
	int Decimals = 0;   // ini formatting
	int Display = 0;    // 0 = value with Decimals, 1 = percent of Max, 2 = percent (value*100)
};

enum
{
	ID_TAB = 100, ID_PLAY = IDOK, ID_QUIT = IDCANCEL, ID_SHOWATSTART = 110, ID_SAVES = 111,
	ID_ITEM = 1000,
};

const char* const PageNames[] = { "Display", "Graphics", "Input", "Audio" };
const int NumPages = 4;

struct FLauncher
{
	std::string BaseDir, UserDir, IniPath;
	FIniFile Ini;
	std::vector<FItem> Items;
	HWND Wnd = NULL, Tab = NULL, Tips = NULL, ShowAtStart = NULL;
	HFONT Font = NULL;
	HBRUSH PageBrush = NULL, BarBrush = NULL;
	int Dpi = 96;
	unsigned char* Splash = nullptr;
	int SplashW = 0, SplashH = 0;
	bool InitialShowAtStart = true;
	bool Play = false;
	// Layout, in 96-dpi units.
	static const int ImageW = 512, ImageH = 372, PanelW = 430, BarH = 46;
	int RowY[NumPages] = {};

	int S( int V ) const { return MulDiv( V, Dpi, 96 ); }

	std::string Get( const char* Section, const char* Key, const char* Default ) const
	{
		std::string V;
		return Ini.Get( Section, Key, V ) && !V.empty() ? V : std::string( Default );
	}

	void SetFont( HWND W ) { SendMessageA( W, WM_SETFONT, (WPARAM)Font, FALSE ); }

	void AddTip( HWND W, const char* Tip )
	{
		if( !Tips || !W || !Tip )
			return;
		TOOLINFOA TI;
		memset( &TI, 0, sizeof(TI) );
		TI.cbSize = TTTOOLINFOA_V2_SIZE;
		TI.uFlags = TTF_IDISHWND | TTF_SUBCLASS;
		TI.hwnd = Wnd;
		TI.uId = (UINT_PTR)W;
		TI.lpszText = (LPSTR)Tip;
		SendMessageA( Tips, TTM_ADDTOOLA, 0, (LPARAM)&TI );
	}

	RECT PageRect() const
	{
		RECT R = { S( ImageW + 10 ), S( 10 ), S( ImageW + PanelW ), S( ImageH ) };
		RECT T = R;
		TabCtrl_AdjustRect( Tab, FALSE, &T );
		return T;
	}

	FItem& NewItem( int Page, EKind Kind, const char* Section, const char* Key, const char* Label, const char* Tip )
	{
		Items.push_back( FItem() );
		FItem& I = Items.back();
		I.Page = Page; I.Kind = Kind; I.Section = Section; I.Key = Key; I.Label = Label; I.Tip = Tip;
		return I;
	}

	// Creates the controls of the last item, on the next row of its page.
	void Place( FItem& I )
	{
		const RECT P = PageRect();
		const int Id = ID_ITEM + (int)( &I - &Items[0] );
		const int X = P.left + S( 12 ), W = P.right - P.left - S( 24 );
		const int LabelW = S( 150 );
		int& Y = RowY[I.Page];
		if( Y == 0 )
			Y = P.top + S( 14 );
		HINSTANCE Inst = GetModuleHandleA( NULL );
		if( I.Kind == K_Check )
		{
			I.Ctrl = CreateWindowExA( 0, "BUTTON", I.Label, WS_CHILD | WS_TABSTOP | BS_AUTOCHECKBOX,
				X, Y, W, S( 22 ), Wnd, (HMENU)(INT_PTR)Id, Inst, NULL );
			SendMessageA( I.Ctrl, BM_SETCHECK, I.InitialCheck ? BST_CHECKED : BST_UNCHECKED, 0 );
			Y += S( 28 );
		}
		else
		{
			I.LabelWnd = CreateWindowExA( 0, "STATIC", I.Label, WS_CHILD | SS_NOTIFY | SS_LEFT,
				X, Y + S( 3 ), LabelW - S( 6 ), S( 20 ), Wnd, NULL, Inst, NULL );
			SetFont( I.LabelWnd );
			AddTip( I.LabelWnd, I.Tip );
			const int CX = X + LabelW, CW = W - LabelW;
			if( I.Kind == K_Combo )
			{
				I.Ctrl = CreateWindowExW( 0, L"COMBOBOX", L"", WS_CHILD | WS_TABSTOP | WS_VSCROLL | CBS_DROPDOWNLIST,
					CX, Y, CW, S( 260 ), Wnd, (HMENU)(INT_PTR)Id, Inst, NULL );
			}
			else
			{
				const int VW = S( 52 );
				I.Ctrl = CreateWindowExA( 0, TRACKBAR_CLASSA, "", WS_CHILD | WS_TABSTOP | TBS_HORZ | TBS_NOTICKS,
					CX - S( 4 ), Y, CW - VW, S( 26 ), Wnd, (HMENU)(INT_PTR)Id, Inst, NULL );
				SendMessageA( I.Ctrl, TBM_SETRANGE, FALSE, MAKELPARAM( 0, I.Max ) );
				SendMessageA( I.Ctrl, TBM_SETPAGESIZE, 0, std::max( 1, I.Max / 10 ) );
				SendMessageA( I.Ctrl, TBM_SETPOS, TRUE, I.InitialPos );
				I.ValueWnd = CreateWindowExA( 0, "STATIC", "", WS_CHILD | SS_NOTIFY | SS_RIGHT,
					CX + CW - VW, Y + S( 4 ), VW, S( 20 ), Wnd, NULL, Inst, NULL );
				SetFont( I.ValueWnd );
				AddTip( I.ValueWnd, I.Tip );
			}
			Y += S( 34 );
		}
		SetFont( I.Ctrl );
		AddTip( I.Ctrl, I.Tip );
	}

	void AddCheck( int Page, const char* Section, const char* Key, bool Default, const char* Label, const char* Tip )
	{
		FItem& I = NewItem( Page, K_Check, Section, Key, Label, Tip );
		I.InitialCheck = ParseBool( Get( Section, Key, "" ), Default );
		Place( I );
	}

	// Labels are UTF-8. Initial = the ini value to select (added as an entry if no entry has it).
	FItem& AddCombo( int Page, const char* Section, const char* Key, const char* Label, const char* Tip,
		const std::vector<std::pair<std::string, std::string>>& Entries, const std::string& Initial )
	{
		FItem& I = NewItem( Page, K_Combo, Section, Key, Label, Tip );
		Place( I );
		I.InitialIndex = -1;
		for( const auto& E : Entries )
		{
			SendMessageW( I.Ctrl, CB_ADDSTRING, 0, (LPARAM)Wide( E.first ).c_str() );
			if( I.InitialIndex < 0 && _stricmp( E.second.c_str(), Initial.c_str() ) == 0 )
				I.InitialIndex = (int)I.Values.size();
			I.Values.push_back( E.second );
		}
		if( I.InitialIndex < 0 )
		{
			std::string Shown = Initial;
			const size_t X = Initial.find( 'x' );
			if( I.Key && ( strstr( I.Key, "ViewportX" ) ) && X != std::string::npos )
				Shown = Initial.substr( 0, X ) + " x " + Initial.substr( X + 1 );
			SendMessageW( I.Ctrl, CB_ADDSTRING, 0, (LPARAM)Wide( Shown + "  (current)" ).c_str() );
			I.InitialIndex = (int)I.Values.size();
			I.Values.push_back( Initial );
		}
		SendMessageW( I.Ctrl, CB_SETCURSEL, I.InitialIndex, 0 );
		return I;
	}

	void AddSlider( int Page, const char* Section, const char* Key, float Default, const char* Label, const char* Tip,
		int Min, int Max, float Scale, int Decimals, int Display )
	{
		FItem& I = NewItem( Page, K_Slider, Section, Key, Label, Tip );
		I.Base = Min * Scale; I.Max = Max - Min; I.Scale = Scale; I.Decimals = Decimals; I.Display = Display;
		const std::string Have = Get( Section, Key, "" );
		const float V = Have.empty() ? Default : (float)atof( Have.c_str() );
		I.InitialPos = std::min( I.Max, std::max( 0, (int)lroundf( ( V - I.Base ) / Scale ) ) );
		Place( I );
		UpdateValue( I );
	}

	void UpdateValue( FItem& I )
	{
		if( !I.ValueWnd )
			return;
		const int Pos = (int)SendMessageA( I.Ctrl, TBM_GETPOS, 0, 0 );
		const float V = I.Base + Pos * I.Scale;
		char Buf[32];
		if( I.Display == 1 )
			snprintf( Buf, sizeof(Buf), "%d%%", (int)lroundf( 100.f * Pos / I.Max ) );
		else if( I.Display == 2 )
			snprintf( Buf, sizeof(Buf), "%d%%", (int)lroundf( 100.f * V ) );
		else
			snprintf( Buf, sizeof(Buf), "%.*f", I.Decimals, V );
		SetWindowTextA( I.ValueWnd, Buf );
	}

	static std::string ModeLabel( int W, int H )
	{
		char Buf[64];
		snprintf( Buf, sizeof(Buf), "%d x %d", W, H );
		return Buf;
	}

	void BuildItems()
	{
		Items.reserve( 64 );
		FMode Desktop;
		const std::vector<FMode> Modes = DisplayModes( Desktop );

		// ---- Display ----
		{
			const bool Full = ParseBool( Get( SecClient, "StartupFullscreen", "" ), true );
			const bool Borderless = ParseBool( Get( SecClient, "BorderlessFullscreen", "" ), false );
			FItem& I = AddCombo( 0, SecClient, "StartupFullscreen", "Display mode",
				"How the game fills the screen. Fullscreen switches the monitor to the chosen resolution, like the retail game. "
				"Borderless covers the desktop without switching (the picture is scaled to fit, keeping its shape). "
				"Window runs in a resizable window. Alt+Enter switches in game.",
				{ { "Fullscreen", "full" }, { "Borderless fullscreen", "borderless" }, { "Window", "window" } },
				!Full ? "window" : Borderless ? "borderless" : "full" );
			I.Special = SP_DisplayMode;
		}
		{
			std::vector<std::pair<std::string, std::string>> E;
			for( const FMode& M : Modes )
				E.push_back( { ModeLabel( M.W, M.H ) + ( M.W == Desktop.W && M.H == Desktop.H ? "  (desktop)" : "" ),
				               std::to_string( M.W ) + "x" + std::to_string( M.H ) } );
			const std::string Cur = Get( SecClient, "ViewportX", "1024" ) + "x" + Get( SecClient, "ViewportY", "768" );
			FItem& I = AddCombo( 0, SecClient, "ViewportX", "Fullscreen resolution",
				"Resolution in fullscreen. Lists every mode your monitor offers, not only the 16 the in-game menu shows.",
				E, Cur );
			I.Special = SP_Resolution;
		}
		{
			std::vector<std::pair<std::string, std::string>> E;
			E.push_back( { "Automatic", "0x0" } );
			for( const FMode& M : Modes )
				if( M.W < Desktop.W && M.H < Desktop.H )
					E.push_back( { ModeLabel( M.W, M.H ), std::to_string( M.W ) + "x" + std::to_string( M.H ) } );
			const std::string Cur = Get( SecClient, "WindowedViewportX", "0" ) + "x" + Get( SecClient, "WindowedViewportY", "0" );
			FItem& I = AddCombo( 0, SecClient, "WindowedViewportX", "Window size",
				"Size of the game window. Automatic uses the fullscreen resolution, shrunk to fit the desktop. "
				"Dragging the window's edge in game also changes it.",
				E, Cur );
			I.Special = SP_WindowSize;
		}
		AddCheck( 0, SecVulkan, "UseVSync", true, "Vertical sync",
			"Waits for the monitor's refresh, like the retail 3dfx Glide driver. Off: no frame cap, the picture can tear." );
		AddCheck( 0, SecKHG, "HorPlusFOV", true, "Widescreen view (Hor+)",
			"On screens wider than 4:3, keeps the 4:3 view from top to bottom and shows more at the sides. "
			"Off: the retail view, cut off at the top and bottom. Console: HorPlus." );
		AddCheck( 0, SecKHG, "WidescreenHUD", true, "Widescreen HUD and menus",
			"Keeps the HUD panels 4:3 in the screen corners and shows the menus at 4:3 with black bars. "
			"Off: stretched across the screen like retail. Console: WideHUD." );
		AddCheck( 0, SecKHG, "PlayIntroAtStartup", false, "Play the intro movie at startup",
			"Plays the intro movie before the main menu every time. The retail game only plays it from the video log menu." );

		// ---- Graphics ----
		AddCombo( 1, SecVulkan, "LightMode", "World lighting",
			"Brightness of the levels' lighting. Retail matches the 1998 3dfx Glide look; brighter is the OpenGL / Android level.",
			{ { "Retail (3dfx Glide)", "3" }, { "Brighter (OpenGL / Android)", "0" }, { "Darker", "1" } },
			Get( SecVulkan, "LightMode", "3" ) );
		AddSlider( 1, SecVulkan, "MirrorReflection", 0.25f, "Floor reflections",
			"Strength of reflections on shiny see-through floors. 0% = none, 100% = the OpenGL / Android strength.",
			0, 100, 0.01f, 2, 2 );
		AddSlider( 1, SecVulkan, "LODBias", 0.f, "Texture sharpness",
			"Mipmap bias. Below 0 distant textures are sharper but can shimmer; above 0 they are softer. 0 = default.",
			-8, 8, 0.25f, 2, 0 );
		AddSlider( 1, SecVulkan, "Contrast", 128.f, "Contrast",
			"Picture contrast. 128 = unchanged.", 0, 255, 1.f, 0, 0 );
		AddSlider( 1, SecVulkan, "Saturation", 255.f, "Colour saturation",
			"Colour strength. 255 = unchanged; lower is greyer.", 0, 255, 1.f, 0, 0 );
		AddCheck( 1, SecVulkan, "GlideGamma", true, "3dfx gamma curve",
			"The in-game brightness slider uses the 3dfx Glide gamma curve, like retail. Off: the older flat brightness." );
		{
			const std::vector<std::string> Gpus = VulkanDevices();
			std::vector<std::pair<std::string, std::string>> E;
			for( size_t i = 0; i < Gpus.size(); ++i )
				E.push_back( { Gpus[i] + ( i == 0 ? "  (best)" : "" ), std::to_string( i ) } );
			if( E.empty() )
				E.push_back( { "Automatic", "0" } );
			FItem& I = AddCombo( 1, SecVulkan, "VkDeviceIndex", "Graphics card",
				"Which graphics card renders the game, for PCs with more than one (such as laptops). "
				"The first one is the fastest kind and is used by default.",
				E, Get( SecVulkan, "VkDeviceIndex", "0" ) );
			EnableWindow( I.Ctrl, E.size() > 1 );
		}

		// ---- Input ----
		AddCheck( 2, SecClient, "RawMouseInput", true, "Raw mouse input",
			"Reads the mouse directly, without Windows pointer speed and acceleration. "
			"Off: moves like the retail game, with your Windows pointer settings." );
		AddCheck( 2, SecClient, "WindowsKeyCodes", true, "Keys follow the keyboard layout",
			"Reads keys as Windows key codes, like retail, so your bindings follow your keyboard layout. "
			"Off: physical key positions of a US keyboard." );
		AddCheck( 2, SecClient, "UseJoystick", true, "Use a controller",
			"Lets a controller play the game (twin-stick layout; rebind buttons in the Controls menu). Off: controllers are ignored, the retail default." );
		AddSlider( 2, SecClient, "ScaleRUV", 100.f, "Right stick speed",
			"How fast the right stick turns and looks. 100 = default.", 25, 300, 1.f, 0, 0 );
		AddSlider( 2, SecClient, "DeadZoneXYZ", 0.2f, "Left stick dead zone",
			"How far the left stick moves before the player does. Raise it if you drift with the stick let go.",
			0, 50, 0.01f, 2, 0 );
		AddSlider( 2, SecClient, "DeadZoneRUV", 0.15f, "Right stick dead zone",
			"How far the right stick moves before the view does. Raise it if the view creeps with the stick let go.",
			0, 50, 0.01f, 2, 0 );
		AddCheck( 2, SecClient, "InvertV", false, "Invert right stick look",
			"Push the right stick up to look down, like a flight stick." );

		// ---- Audio ----
		{
			std::vector<std::pair<std::string, std::string>> E;
			E.push_back( { "Windows default device", "" } );
			for( const std::string& D : AudioDevices( BaseDir ) )
				E.push_back( { D, D } );
			std::string Cur;
			Ini.Get( SecAudio, "DeviceName", Cur );
			AddCombo( 3, SecAudio, "DeviceName", "Output device",
				"Speakers or headset for the game sound and the movies. The default follows the Windows default device.",
				E, Cur );
		}
		AddSlider( 3, SecAudio, "MasterVolume", 255.f, "Master volume",
			"Overall volume. Sound effects volume is in the game's Audio/Video menu.", 0, 255, 1.f, 0, 1 );
		AddSlider( 3, SecAudio, "MusicVolume", 63.f, "Music volume",
			"Volume of the music. The game's own menus have no setting for it.", 0, 255, 1.f, 0, 1 );
		AddCheck( 3, SecAudio, "UseHRTF", true, "3D sound for headphones (HRTF)",
			"Places sounds around you on headphones. Turn it off if sound seems odd on speakers." );
		AddCheck( 3, SecAudio, "UseReverb", true, "Room reverb",
			"Echo in caves, halls and corridors." );
		AddCheck( 3, SecAudio, "UseCDMusic", true, "Music from ripped CD tracks",
			"Plays the game CD's music from the Music folder (Track02.ogg and so on) in the ten levels that use it. "
			"See KHGWin-README.txt." );
	}

	void ShowPage( int Page )
	{
		for( FItem& I : Items )
		{
			const int Cmd = I.Page == Page ? SW_SHOW : SW_HIDE;
			for( HWND W : { I.LabelWnd, I.Ctrl, I.ValueWnd } )
				if( W )
					ShowWindow( W, Cmd );
		}
	}

	bool Changed( const FItem& I ) const
	{
		if( I.Kind == K_Check )
			return ( SendMessageA( I.Ctrl, BM_GETCHECK, 0, 0 ) == BST_CHECKED ) != I.InitialCheck;
		if( I.Kind == K_Combo )
			return (int)SendMessageW( I.Ctrl, CB_GETCURSEL, 0, 0 ) != I.InitialIndex;
		return (int)SendMessageA( I.Ctrl, TBM_GETPOS, 0, 0 ) != I.InitialPos;
	}

	void Apply()
	{
		for( const FItem& I : Items )
		{
			if( !Changed( I ) )
				continue;
			if( I.Kind == K_Check )
			{
				Ini.Set( I.Section, I.Key, SendMessageA( I.Ctrl, BM_GETCHECK, 0, 0 ) == BST_CHECKED ? "True" : "False" );
				continue;
			}
			if( I.Kind == K_Slider )
			{
				char Buf[32];
				snprintf( Buf, sizeof(Buf), "%.*f", I.Decimals, I.Base + (int)SendMessageA( I.Ctrl, TBM_GETPOS, 0, 0 ) * I.Scale );
				Ini.Set( I.Section, I.Key, Buf );
				continue;
			}
			const int Sel = (int)SendMessageW( I.Ctrl, CB_GETCURSEL, 0, 0 );
			if( Sel < 0 || Sel >= (int)I.Values.size() )
				continue;
			const std::string& V = I.Values[Sel];
			if( I.Special == SP_DisplayMode )
			{
				Ini.Set( SecClient, "StartupFullscreen", V == "window" ? "False" : "True" );
				if( V != "window" )
					Ini.Set( SecClient, "BorderlessFullscreen", V == "borderless" ? "True" : "False" );
			}
			else if( I.Special == SP_Resolution || I.Special == SP_WindowSize )
			{
				const size_t X = V.find( 'x' );
				const bool Win = I.Special == SP_WindowSize;
				if( X != std::string::npos )
				{
					Ini.Set( SecClient, Win ? "WindowedViewportX" : "ViewportX", V.substr( 0, X ) );
					Ini.Set( SecClient, Win ? "WindowedViewportY" : "ViewportY", V.substr( X + 1 ) );
				}
			}
			else
				Ini.Set( I.Section, I.Key, V );
		}
		const bool Show = SendMessageA( ShowAtStart, BM_GETCHECK, 0, 0 ) == BST_CHECKED;
		if( Show != InitialShowAtStart )
			Ini.Set( SecKHG, "ShowLauncher", Show ? "True" : "False" );
	}

	void OpenSavesFolder()
	{
		std::string Dir = UserDir;
		if( _stricmp( UserDir.c_str(), BaseDir.c_str() ) == 0 )
			Dir = BaseDir + "..\\Save";   // portable: the install's own Save folder, like retail
		ShellExecuteA( Wnd, "open", Dir.c_str(), NULL, NULL, SW_SHOWNORMAL );
	}

	void Create()
	{
		HINSTANCE Inst = GetModuleHandleA( NULL );
		const int CW = S( ImageW + PanelW + 10 ), CH = S( ImageH + BarH );
		RECT R = { 0, 0, CW, CH };
		const DWORD Style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_CLIPCHILDREN;
		AdjustWindowRectEx( &R, Style, FALSE, 0 );
		const int WW = R.right - R.left, WH = R.bottom - R.top;
		RECT Work;
		SystemParametersInfoA( SPI_GETWORKAREA, 0, &Work, 0 );
		Wnd = CreateWindowExA( WS_EX_CONTROLPARENT, "KHGLauncher", "Klingon Honor Guard", Style,
			Work.left + ( Work.right - Work.left - WW ) / 2, Work.top + std::max( 0L, ( Work.bottom - Work.top - WH ) / 2 ),
			WW, WH, NULL, NULL, Inst, this );

		Tips = CreateWindowExA( WS_EX_TOPMOST, TOOLTIPS_CLASSA, NULL, WS_POPUP | TTS_ALWAYSTIP | TTS_NOPREFIX,
			0, 0, 0, 0, Wnd, NULL, Inst, NULL );
		if( Tips )
		{
			SendMessageA( Tips, TTM_SETMAXTIPWIDTH, 0, S( 320 ) );
			SendMessageA( Tips, TTM_SETDELAYTIME, TTDT_AUTOPOP, 30000 );
			SendMessageA( Tips, TTM_SETDELAYTIME, TTDT_INITIAL, 400 );
		}

		Tab = CreateWindowExA( 0, WC_TABCONTROLA, "", WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_CLIPSIBLINGS,
			S( ImageW + 10 ), S( 10 ), S( PanelW - 10 ), S( ImageH - 10 ), Wnd, (HMENU)ID_TAB, Inst, NULL );
		SetFont( Tab );
		for( int i = 0; i < NumPages; ++i )
		{
			TCITEMA TI;
			memset( &TI, 0, sizeof(TI) );
			TI.mask = TCIF_TEXT;
			TI.pszText = (LPSTR)PageNames[i];
			SendMessageA( Tab, TCM_INSERTITEMA, i, (LPARAM)&TI );
		}

		BuildItems();
		// The page controls sit above the tab control.
		for( FItem& I : Items )
			for( HWND W : { I.LabelWnd, I.Ctrl, I.ValueWnd } )
				if( W )
					SetWindowPos( W, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
		ShowPage( 0 );

		// Bottom bar.
		const int BY = S( ImageH ) + ( S( BarH ) - S( 26 ) ) / 2;
		InitialShowAtStart = ParseBool( Get( SecKHG, "ShowLauncher", "" ), true );
		ShowAtStart = CreateWindowExA( 0, "BUTTON", "Show this window at startup", WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX,
			S( 14 ), BY + S( 2 ), S( 220 ), S( 22 ), Wnd, (HMENU)ID_SHOWATSTART, Inst, NULL );
		SendMessageA( ShowAtStart, BM_SETCHECK, InitialShowAtStart ? BST_CHECKED : BST_UNCHECKED, 0 );
		SetFont( ShowAtStart );
		AddTip( ShowAtStart, "Untick to start the game straight away next time. Hold Shift while starting KHGWin.exe "
			"(or start it with -launcher) to bring this window back. -nolauncher skips it once." );
		HWND Saves = CreateWindowExA( 0, "BUTTON", "Open saves folder", WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON,
			S( 250 ), BY, S( 130 ), S( 26 ), Wnd, (HMENU)ID_SAVES, Inst, NULL );
		SetFont( Saves );
		AddTip( Saves, "Opens the folder with your saved games, KHGWin.ini and KHGWin.log." );
		const int Right = S( ImageW + PanelW );
		HWND PlayBtn = CreateWindowExA( 0, "BUTTON", "Play", WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_DEFPUSHBUTTON,
			Right - S( 196 ), BY, S( 96 ), S( 26 ), Wnd, (HMENU)ID_PLAY, Inst, NULL );
		HWND QuitBtn = CreateWindowExA( 0, "BUTTON", "Quit", WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON,
			Right - S( 94 ), BY, S( 84 ), S( 26 ), Wnd, (HMENU)ID_QUIT, Inst, NULL );
		SetFont( PlayBtn );
		SetFont( QuitBtn );
		AddTip( PlayBtn, "Saves these settings to KHGWin.ini and starts the game." );
		AddTip( QuitBtn, "Closes without starting the game or saving changes." );

		ShowWindow( Wnd, SW_SHOWNORMAL );
		SetForegroundWindow( Wnd );
		SetFocus( PlayBtn );
	}

	void Paint( HDC DC )
	{
		RECT Client;
		GetClientRect( Wnd, &Client );
		RECT Img = { 0, 0, S( ImageW ), S( ImageH ) };
		RECT Panel = { Img.right, 0, Client.right, Img.bottom };
		RECT Bar = { 0, Img.bottom, Client.right, Client.bottom };
		FillRect( DC, &Panel, PageBrush );
		FillRect( DC, &Bar, BarBrush );
		RECT Line = { 0, Bar.top, Client.right, Bar.top + 1 };
		FillRect( DC, &Line, GetSysColorBrush( COLOR_3DLIGHT ) );
		if( Splash )
		{
			BITMAPINFO BI;
			memset( &BI, 0, sizeof(BI) );
			BI.bmiHeader.biSize = sizeof(BI.bmiHeader);
			BI.bmiHeader.biWidth = SplashW;
			BI.bmiHeader.biHeight = -SplashH;   // top-down
			BI.bmiHeader.biPlanes = 1;
			BI.bmiHeader.biBitCount = 32;
			BI.bmiHeader.biCompression = BI_RGB;
			SetStretchBltMode( DC, HALFTONE );
			SetBrushOrgEx( DC, 0, 0, NULL );
			StretchDIBits( DC, 0, 0, Img.right, Img.bottom, 0, 0, SplashW, SplashH, Splash, &BI, DIB_RGB_COLORS, SRCCOPY );
		}
		else
		{
			FillRect( DC, &Img, (HBRUSH)GetStockObject( BLACK_BRUSH ) );
			HGDIOBJ Old = SelectObject( DC, Font );
			SetTextColor( DC, RGB( 200, 40, 30 ) );
			SetBkMode( DC, TRANSPARENT );
			DrawTextA( DC, "Klingon Honor Guard", -1, &Img, DT_CENTER | DT_VCENTER | DT_SINGLELINE );
			SelectObject( DC, Old );
		}
	}

	LRESULT Proc( UINT Msg, WPARAM WParam, LPARAM LParam )
	{
		switch( Msg )
		{
		case WM_ERASEBKGND:
			return 1;
		case WM_PAINT:
		{
			PAINTSTRUCT PS;
			HDC DC = BeginPaint( Wnd, &PS );
			Paint( DC );
			EndPaint( Wnd, &PS );
			return 0;
		}
		case WM_CTLCOLORSTATIC:
		{
			HDC DC = (HDC)WParam;
			const bool OnBar = (HWND)LParam == ShowAtStart;
			SetBkColor( DC, GetSysColor( OnBar ? COLOR_BTNFACE : COLOR_WINDOW ) );
			SetTextColor( DC, GetSysColor( COLOR_WINDOWTEXT ) );
			return (LRESULT)( OnBar ? BarBrush : PageBrush );
		}
		case WM_HSCROLL:
			for( FItem& I : Items )
				if( I.Ctrl == (HWND)LParam )
					UpdateValue( I );
			return 0;
		case WM_NOTIFY:
		{
			const NMHDR* N = (const NMHDR*)LParam;
			if( N->hwndFrom == Tab && N->code == TCN_SELCHANGE )
				ShowPage( (int)SendMessageA( Tab, TCM_GETCURSEL, 0, 0 ) );
			return 0;
		}
		case WM_COMMAND:
			switch( LOWORD( WParam ) )
			{
			case ID_PLAY:
				Apply();
				if( !Ini.Save( IniPath.c_str() ) )
					MessageBoxA( Wnd, ( "Could not save the settings to " + IniPath + ". The game starts with the old settings." ).c_str(),
						"Klingon Honor Guard", MB_OK | MB_ICONWARNING );
				Play = true;
				DestroyWindow( Wnd );
				return 0;
			case ID_QUIT:
				DestroyWindow( Wnd );
				return 0;
			case ID_SAVES:
				OpenSavesFolder();
				return 0;
			}
			break;
		case WM_CLOSE:
			DestroyWindow( Wnd );
			return 0;
		case WM_DESTROY:
			PostQuitMessage( 0 );
			return 0;
		}
		return DefWindowProcA( Wnd, Msg, WParam, LParam );
	}

	static LRESULT CALLBACK StaticProc( HWND W, UINT Msg, WPARAM WParam, LPARAM LParam )
	{
		if( Msg == WM_NCCREATE )
		{
			FLauncher* L = (FLauncher*)( (CREATESTRUCTA*)LParam )->lpCreateParams;
			L->Wnd = W;
			SetWindowLongPtrA( W, GWLP_USERDATA, (LONG_PTR)L );
		}
		FLauncher* L = (FLauncher*)GetWindowLongPtrA( W, GWLP_USERDATA );
		return L ? L->Proc( Msg, WParam, LParam ) : DefWindowProcA( W, Msg, WParam, LParam );
	}
};

// Common controls 6 (themed buttons, tabs, sliders) for the launcher only: the manifest is resource
// 2 (RT_MANIFEST), which Windows doesn't apply to the process by itself, so the game is unaffected.
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
		AC.lpResourceName = MAKEINTRESOURCEA( 2 );
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

void SetDpiAware()
{
	HMODULE User = GetModuleHandleA( "user32.dll" );
	typedef BOOL (WINAPI *FSetCtx)( HANDLE );
	FSetCtx SetCtx = User ? (FSetCtx)(void*)GetProcAddress( User, "SetProcessDpiAwarenessContext" ) : nullptr;
	// Same setting SDL asks for later (SDL_HINT_WINDOWS_DPI_AWARENESS=permonitorv2 in NSDLClient.cpp).
	if( !SetCtx || !SetCtx( (HANDLE)-4 /*DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2*/ ) )
		SetProcessDPIAware();
}

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

bool LoadSplash( const std::string& BaseDir, unsigned char** Out, int* W, int* H )
{
	static const char* Names[] = { "Khg.exe", "KHG.EXE", "khg.exe", "KHG.exe" };
	for( const char* N : Names )
		if( KHGLoadPEBitmapBGRA( ( BaseDir + N ).c_str(), KHG_SPLASH_RESOURCE_ID, Out, W, H ) )
			return true;
	return false;
}

} // namespace

bool KHGLauncherWanted( const char* CmdLine, const char* IniPath )
{
	// Tokens of the command line, without a leading - or /.
	std::vector<std::string> Args;
	for( const char* P = CmdLine ? CmdLine : ""; *P; )
	{
		while( *P == ' ' || *P == '\t' )
			P++;
		const char* B = P;
		while( *P && *P != ' ' && *P != '\t' )
			P++;
		if( P > B && ( *B == '-' || *B == '/' ) )
			Args.push_back( std::string( B + 1, P ) );
	}
	auto Has = [&]( const char* Name ) {
		for( const std::string& A : Args )
			if( _stricmp( A.c_str(), Name ) == 0 )
				return true;
		return false;
	};
	if( Has( "nolauncher" ) )
		return false;
	if( Has( "launcher" ) || ( GetAsyncKeyState( VK_SHIFT ) & 0x8000 ) )
		return true;
	for( const std::string& A : Args )
		if( _strnicmp( A.c_str(), "khg", 3 ) == 0 )   // test and tool runs (-khgshots, -khgmaptest, -khgreplay, ...)
			return false;
	FIniFile Ini;
	std::string V;
	return !( Ini.Load( IniPath ) && Ini.Get( SecKHG, "ShowLauncher", V ) && !ParseBool( V, true ) );
}

bool KHGRunLauncher( const char* BaseDir, const char* UserDir, const char* IniPath )
{
	SetDpiAware();
	FVisualStyles Styles;
	HMODULE ComCtl = LoadLibraryA( "comctl32.dll" );
	typedef BOOL (WINAPI *FInit)( const INITCOMMONCONTROLSEX* );
	FInit Init = ComCtl ? (FInit)(void*)GetProcAddress( ComCtl, "InitCommonControlsEx" ) : nullptr;
	if( Init )
	{
		INITCOMMONCONTROLSEX ICC = { sizeof(ICC), ICC_TAB_CLASSES | ICC_BAR_CLASSES | ICC_STANDARD_CLASSES };
		Init( &ICC );
	}

	FLauncher L;
	L.BaseDir = BaseDir ? BaseDir : "";
	L.UserDir = UserDir ? UserDir : "";
	L.IniPath = IniPath ? IniPath : "";
	L.Ini.Load( IniPath );
	L.Dpi = SystemDpi();
	LoadSplash( L.BaseDir, &L.Splash, &L.SplashW, &L.SplashH );
	L.Font = CreateFontA( -MulDiv( 9, L.Dpi, 72 ), 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH, "Segoe UI" );
	L.PageBrush = GetSysColorBrush( COLOR_WINDOW );
	L.BarBrush = GetSysColorBrush( COLOR_BTNFACE );

	WNDCLASSEXA WC;
	memset( &WC, 0, sizeof(WC) );
	WC.cbSize = sizeof(WC);
	WC.lpfnWndProc = FLauncher::StaticProc;
	WC.hInstance = GetModuleHandleA( NULL );
	WC.hIcon = LoadIconA( WC.hInstance, MAKEINTRESOURCEA( 128 ) );
	WC.hIconSm = WC.hIcon;
	WC.hCursor = LoadCursorA( NULL, (LPCSTR)IDC_ARROW );
	WC.lpszClassName = "KHGLauncher";
	RegisterClassExA( &WC );

	L.Create();
	MSG Msg;
	while( GetMessageA( &Msg, NULL, 0, 0 ) > 0 )
	{
		if( L.Wnd && IsWindow( L.Wnd ) && IsDialogMessageA( L.Wnd, &Msg ) )
			continue;
		TranslateMessage( &Msg );
		DispatchMessageA( &Msg );
	}
	UnregisterClassA( "KHGLauncher", GetModuleHandleA( NULL ) );
	if( L.Font )
		DeleteObject( L.Font );
	free( L.Splash );
	return L.Play;
}
