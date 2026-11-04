/*=============================================================================
	KHGTiming.cpp: M8 high frame rates (desktop build).

	1. FKHGFramePacer: the desktop main loop's frame pacing (frame cap, 1 ms floor,
	   "original timing" fixed steps). See KHGTiming.h.

	2. KHGRunTimingTest: KHGWin.exe -khgtiming[=<dir>] runs the same scripted input at
	   several fixed tick rates and writes <dir>\Timing-<map>-<arch>.html/.txt/.csv
	   comparing every measurement with the 60 Hz run.
	     -khgmap=X.unr          map (default M02.unr); -khgmaps=A.unr,B.unr for several
	     -khgrates=60,144,...   tick rates (default 60,144,240,500,1000)
	     -khgworld=N            seconds of the idle "world" run per rate (default 20)
	   The ticks are not paced to real time, so the 500 and 1000 Hz runs do not need a
	   PC that renders that fast. Input goes through the real input system: spare
	   joystick buttons are bound to the ini's aliases (MoveForward, Jump, Fire...)
	   for the run and pressed/released by the script; mouse moves are IK_MouseX
	   axis events. Every run starts from a fresh map load with rand() seeded to 1.
=============================================================================*/

#ifdef _WIN32
#include <windows.h>
#endif
#include "Engine.h"
#include "KHGTiming.h"

#include <stdio.h>
#include <cmath>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <unordered_set>
#include <algorithm>

#ifdef PLATFORM_WIN32

extern INT   GKHGSyntheticInput;   // NSDLViewport.cpp: don't release keys that aren't physically down
extern ENGINE_API DWORD GKHGTimerEvents;
extern ENGINE_API void (*GKHGTimerHook)( AActor* Actor );   // UnLevTic.cpp: called before each Timer()
extern ENGINE_API UBOOL GKHGNoDraw;
extern ENGINE_API INT GKHGFrameRateFixes;   // UnLevTic.cpp: 60 Hz Tick() for frame-counting script        // UnGame.cpp: skip Client->Tick (drawing)      // UnLevTic.cpp: Timer() calls since startup

/*-----------------------------------------------------------------------------
	Frame pacing.
-----------------------------------------------------------------------------*/

static void KHGWaitUntil( DOUBLE Target )
{
	// Windows sleeps are coarse (about 1 ms at best): sleep most of the wait, spin the rest.
	for( ;; )
	{
		const DOUBLE Left = Target - appSeconds();
		if( Left <= 0.0 )
			return;
		if( Left > 0.002 )
			appSleep( Left - 0.0015 );
	}
}

void FKHGFramePacer::Init()
{
	INT InCap = 0, Rate = 60;
	UBOOL Original = 0;
	GetConfigInt( "KHGWin", "FrameRateLimit", InCap );
	GetConfigBool( "KHGWin", "OriginalTiming", Original );
	GetConfigInt( "KHGWin", "OriginalTimingRate", Rate );
	// Command line overrides for one run: -khgframecap=N, -khgoriginaltiming=0|N.
	Parse( appCmdLine(), "KHGFRAMECAP=", InCap );
	INT Arg = -1;
	if( Parse( appCmdLine(), "KHGORIGINALTIMING=", Arg ) )
	{
		Original = Arg != 0;
		if( Arg > 1 )
			Rate = Arg;
	}
	Configure( InCap, Original ? Rate : 0 );
	INT Fixes = 2;
	GetConfigInt( "KHGWin", "FrameRateFixes", Fixes );
	Parse( appCmdLine(), "KHGFRAMERATEFIXES=", Fixes );
	GKHGFrameRateFixes = Clamp( Fixes, 0, 2 );
	debugf( NAME_Log, "[KHG] Timing: FrameRateFixes=%i (%s)", GKHGFrameRateFixes,
		GKHGFrameRateFixes == 0 ? "off, retail" : GKHGFrameRateFixes == 1 ? "gameplay" : "gameplay and effects" );
	if( FixedRate )
		debugf( NAME_Log, "[KHG] Timing: original timing, fixed %i Hz game steps", FixedRate );
	else if( Cap )
		debugf( NAME_Log, "[KHG] Timing: frame rate limit %i fps", Cap );
	else
		debugf( NAME_Log, "[KHG] Timing: variable steps, no frame rate limit (game steps of 1 ms or more)" );
}

void FKHGFramePacer::Configure( INT InCap, INT InFixedRate )
{
	Cap       = InCap > 0 ? Clamp( InCap, 20, 1000 ) : 0;
	FixedRate = InFixedRate > 0 ? Clamp( InFixedRate, 20, 1000 ) : 0;
	Last      = appSeconds();
}

DOUBLE FKHGFramePacer::BeginFrame()
{
	if( FixedRate )
	{
		// One fixed game step per frame. If the PC falls behind by more than 0.1 s the game slows
		// down instead of running a burst of catch-up steps.
		const DOUBLE Step = 1.0 / FixedRate;
		const DOUBLE Next = Last + Step;
		KHGWaitUntil( Next );
		Last = Max( Next, appSeconds() - 0.1 );
		return Step;
	}
	// Variable steps: wait for the frame cap, and always for the 1 ms the engine's clamp needs.
	KHGWaitUntil( Last + ( Cap ? 1.0 / Cap : 0.001 ) );
	const DOUBLE Now = appSeconds();
	const DOUBLE Delta = Now - Last;
	Last = Now;
	return Delta;
}

/*-----------------------------------------------------------------------------
	Timing test helpers.
-----------------------------------------------------------------------------*/

// The text after "-<Key>=" up to the next space (Parse() would stop at a comma).
static UBOOL KHGTArg( const char* Key, char* Out, INT Max )
{
	const char* Cmd = appCmdLine();
	const INT Len = appStrlen( Key );
	for( const char* C = Cmd; *C; C++ )
		if( ( C == Cmd || C[-1] == ' ' || C[-1] == '-' || C[-1] == '/' ) && appStrnicmp( C, Key, Len )==0 )
		{
			C += Len;
			INT n = 0;
			while( *C && *C != ' ' && n < Max - 1 )
				Out[n++] = *C++;
			Out[n] = 0;
			return 1;
		}
	return 0;
}

static UProperty* KHGTFindProp( UObject* Obj, const char* Name )
{
	if( !Obj )
		return NULL;
	for( TFieldIterator<UProperty> It( Obj->GetClass() ); It; ++It )
		if( appStricmp( It->GetName(), Name )==0 )
			return *It;
	return NULL;
}
static INT KHGTGetInt( UObject* Obj, const char* Name, INT Default=-1 )
{
	UIntProperty* Prop = Cast<UIntProperty>( KHGTFindProp( Obj, Name ) );
	return Prop ? *(INT*)( (BYTE*)Obj + Prop->Offset ) : Default;
}
static void KHGTSetInt( UObject* Obj, const char* Name, INT Value )
{
	if( UIntProperty* Prop = Cast<UIntProperty>( KHGTFindProp( Obj, Name ) ) )
		*(INT*)( (BYTE*)Obj + Prop->Offset ) = Value;
}
static FLOAT KHGTGetFloat( UObject* Obj, const char* Name, FLOAT Default=0.f )
{
	UFloatProperty* Prop = Cast<UFloatProperty>( KHGTFindProp( Obj, Name ) );
	return Prop ? *(FLOAT*)( (BYTE*)Obj + Prop->Offset ) : Default;
}
static UBOOL KHGTGetBool( UObject* Obj, const char* Name )
{
	UBoolProperty* Prop = Cast<UBoolProperty>( KHGTFindProp( Obj, Name ) );
	return Prop ? ( *(DWORD*)( (BYTE*)Obj + Prop->Offset ) & Prop->BitMask ) != 0 : 0;
}
static const char* KHGTStateName( UObject* Obj )
{
	return ( Obj && Obj->GetMainFrame() && Obj->GetMainFrame()->StateNode ) ? Obj->GetMainFrame()->StateNode->GetName() : "None";
}
static std::string KHGTFmt( const char* Fmt, ... )
{
	char Buf[1024];
	va_list Args;
	va_start( Args, Fmt );
	vsnprintf( Buf, sizeof(Buf), Fmt, Args );
	va_end( Args );
	return Buf;
}

// One measurement: a value per tick rate, compared with the first (60 Hz) rate.
struct FKHGTMetric
{
	std::string Test, Name, Unit;
	DOUBLE Tol;        // allowed difference from the 60 Hz value: relative if TolAbs==0, else absolute
	DOUBLE TolAbs;
	std::vector<DOUBLE> Values;   // per rate; NAN = not measured
	std::string Note;
};

static UBOOL GKHGTimingDebug = 0;
static char  GKHGTimingOnly[256] = "";   // -khgtests=a,b: only these test groups (debugging)
static UBOOL KHGTWant( const char* Group )
{
	if( !GKHGTimingOnly[0] )
		return 1;
	char Key[64];
	appSprintf( Key, ",%s,", Group );
	char List[260];
	appSprintf( List, ",%s,", GKHGTimingOnly );
	for( char* C = List; *C; C++ ) *C = tolower( *C );
	return strstr( List, Key ) != NULL;
}
static std::map<std::string,INT>* GKHGTimerCounts = NULL;
static void KHGTCountTimer( AActor* Actor )
{
	if( GKHGTimerCounts )
		(*GKHGTimerCounts)[Actor->GetClassName()]++;
}

struct FKHGTiming
{
	UGameEngine*  GE;
	UViewport*    VP;
	APlayerPawn*  P;
	ULevel*       L;
	INT           RateIndex, NumRates;
	DOUBLE        Rate, SimTime;
	std::vector<FKHGTMetric> Metrics;
	std::map<std::string,EInputKey> Keys;
	FString       SavedBindings[16];
	FVector       StartLoc;
	FRotator      StartRot;
	UBOOL         Ok;

	void Tick()
	{
		// Without drawing no zone is ever "seen", and actors with bStasis stop ticking 5 s after their zone
		// was last drawn. Mark the player's zone as seen so things next to the player keep running.
		if( GKHGNoDraw && P && L && L->Model && L->Model->Nodes && P->Region.ZoneNumber < 64 )
			L->Model->Nodes->Zones[P->Region.ZoneNumber].LastRenderTime = L->TimeSeconds;
		GE->Tick( 1.0 / Rate );
		SimTime += 1.0 / Rate;
		L = GE->GLevel;
		P = VP ? VP->Actor : NULL;
	}
	INT Steps( DOUBLE Seconds ) { return Max( 1, (INT)floor( Seconds * Rate + 0.5 ) ); }
	void Wait( DOUBLE Seconds ) { for( INT i = Steps( Seconds ); i > 0 && !GIsRequestingExit; i-- ) Tick(); }

	FKHGTMetric& Metric( const std::string& Test, const std::string& Name, const char* Unit, DOUBLE Tol, DOUBLE TolAbs=0.0 )
	{
		for( size_t i = 0; i < Metrics.size(); i++ )
			if( Metrics[i].Test == Test && Metrics[i].Name == Name )
				return Metrics[i];
		FKHGTMetric M;
		M.Test = Test; M.Name = Name; M.Unit = Unit; M.Tol = Tol; M.TolAbs = TolAbs;
		M.Values.assign( NumRates, NAN );
		Metrics.push_back( M );
		return Metrics.back();
	}
	void Set( const std::string& Test, const std::string& Name, const char* Unit, DOUBLE Value, DOUBLE Tol, DOUBLE TolAbs=0.0 )
	{
		Metric( Test, Name, Unit, Tol, TolAbs ).Values[RateIndex] = Value;
		debugf( NAME_Log, "[KHG] Timing %.0fHz %s %s=%.4f %s", Rate, Test.c_str(), Name.c_str(), Value, Unit );
	}

	// Input: spare joystick buttons bound to the ini's aliases for the run.
	void BindKeys()
	{
		static const char* const Aliases[] = { "MoveForward", "MoveBackward", "StrafeRight", "TurnRight", "LookUp", "Jump", "Fire", "AltFire", "Walking", "Duck" };
		Keys.clear();
		for( INT i = 0; i < ARRAY_COUNT(Aliases); i++ )
		{
			EInputKey Key = (EInputKey)( IK_Joy1 + 4 + i );   // Joy5-Joy14
			SavedBindings[i] = VP->Input->Bindings[Key];
			VP->Input->Bindings[Key] = Aliases[i];
			Keys[Aliases[i]] = Key;
		}
	}
	void UnbindKeys()
	{
		INT i = 0;
		for( std::map<std::string,EInputKey>::iterator It = Keys.begin(); It != Keys.end(); ++It )
			GE->InputEvent( VP, It->second, IST_Release );
		for( i = 0; i < 10; i++ )
			VP->Input->Bindings[IK_Joy1 + 4 + i] = SavedBindings[i];
		Keys.clear();
	}
	void Press( const char* Alias )   { GE->InputEvent( VP, Keys[Alias], IST_Press ); }
	void Release( const char* Alias ) { GE->InputEvent( VP, Keys[Alias], IST_Release ); }
	void ReleaseAll()
	{
		for( std::map<std::string,EInputKey>::iterator It = Keys.begin(); It != Keys.end(); ++It )
			GE->InputEvent( VP, It->second, IST_Release );
	}

	// Put the player back at the start spot, standing still, facing the test direction.
	void ResetPlayer( INT FaceOffset=0 )
	{
		ReleaseAll();
		if( !P )
			return;
		L->FarMoveActor( P, StartLoc, 0, 1 );
		P->Velocity = P->Acceleration = FVector(0,0,0);
		const FRotator Face( StartRot.Pitch, StartRot.Yaw + FaceOffset, 0 );
		P->ViewRotation = Face;
		P->Rotation = FRotator( 0, Face.Yaw, 0 );
		Wait( 1.0 );
		P->ViewRotation = Face;
	}

	static INT YawDiff( INT A, INT B ) { INT D = ( A - B ) & 65535; return D > 32767 ? D - 65536 : D; }
	static DOUBLE Dist2D( FVector A, FVector B ) { return sqrt( (A.X-B.X)*(A.X-B.X) + (A.Y-B.Y)*(A.Y-B.Y) ); }

	UBOOL LoadMap( const char* Map )
	{
		char URL[256], Error[256] = "";
		appSprintf( URL, "..\\Maps\\%s", Map );
		srand( 1 );
		if( !GE->Browse( FURL( &GE->LastURL, URL, TRAVEL_Absolute ), Error ) )
		{
			debugf( NAME_Log, "[KHG] Timing: %s FAILED: %s", Map, Error );
			return 0;
		}
		L = GE->GLevel;
		P = VP->Actor;
		SimTime = 0;
		return P != NULL;
	}

	// Let the map's opening play out until the player can walk (or 40 s pass).
	UBOOL WaitForControl()
	{
		for( INT i = Steps( 40.0 ); i > 0 && !GIsRequestingExit; i-- )
		{
			Tick();
			if( P && P->Physics == PHYS_Walking && appStricmp( KHGTStateName( P ), "PlayerWalking" )==0 && P->Velocity.SizeSquared() < 1.f )
				return 1;
		}
		debugf( NAME_Log, "[KHG] Timing: player not in PlayerWalking after 40 s (state %s, physics %i)", KHGTStateName( P ), P ? (INT)P->Physics : -1 );
		return 0;
	}

	// The player tests run without the map's other pawns, so nothing shoots or bumps the player.
	void ClearPawns()
	{
		INT Removed = 0;
		for( INT a = 0; a < L->Num(); a++ )
		{
			APawn* Pn = Cast<APawn>( L->Actors(a) );
			if( Pn && Pn != P && !Pn->bDeleteMe && L->DestroyActor( Pn ) )
				Removed++;
		}
		P->ReducedDamageType = FName( "All" );
		Wait( 0.1 );
		debugf( NAME_Log, "[KHG] Timing: removed %i pawns for the player tests", Removed );
	}

	// Free distance from Loc along Yaw for a walking pawn.
	FLOAT FreeDistance( FVector Loc, INT Yaw )
	{
		const FVector Dir = FRotator( 0, Yaw, 0 ).Vector();
		FCheckResult Hit;
		const FVector Extent( P->CollisionRadius, P->CollisionRadius, P->CollisionHeight * 0.5f );
		return L->SingleLineCheck( Hit, P, Loc + Dir * 1500.f, Loc, TRACE_AllColliding, Extent ) ? 1500.f : Hit.Time * 1500.f;
	}
	// A spot for the movement tests with room to run: the player start or a path node near it,
	// whichever has the longest free run in one of 16 directions. Runs at every rate pick the same spot.
	void ChooseStart()
	{
		FVector BestLoc = P->Location;
		INT BestYaw = P->Rotation.Yaw;
		FLOAT Best = -1.f;
		const FVector Origin = P->Location;
		for( INT a = -1; a < L->Num() && Best < 1500.f; a++ )
		{
			FVector Loc = Origin;
			if( a >= 0 )
			{
				ANavigationPoint* N = Cast<ANavigationPoint>( L->Actors(a) );
				if( !N || ( N->Location - Origin ).SizeSquared() > 3000.f * 3000.f )
					continue;
				Loc = N->Location;
				FCheckResult Hit;
				if( !L->SingleLineCheck( Hit, P, Loc, Origin, TRACE_VisBlocking ) )
					continue;
			}
			for( INT d = 0; d < 16; d++ )
			{
				const FLOAT Free = FreeDistance( Loc, d * 4096 );
				if( Free > Best + 1.f )
				{
					Best = Free;
					BestYaw = d * 4096;
					BestLoc = Loc;
				}
			}
		}
		// Drop the player there and let them land, then remember where they stand.
		L->FarMoveActor( P, BestLoc, 0, 0 );
		P->Velocity = FVector(0,0,0);
		Wait( 1.5 );
		StartLoc = P->Location;
		StartRot = FRotator( 0, BestYaw, 0 );
		debugf( NAME_Log, "[KHG] Timing: test spot (%.0f,%.0f,%.0f) yaw %i, %.0f units free, physics %i", StartLoc.X, StartLoc.Y, StartLoc.Z, BestYaw, Best, (INT)P->Physics );
	}

	// Give the player every weapon class in the loaded packages, through the normal pickup code.
	void GiveWeapons()
	{
		// The retail weapon classes (Klingons.u), loaded if the map didn't need them yet.
		static const char* const Names[] = { "Batleth", "Daktagh", "DisruptorPistol", "DisruptorRifle", "AssaultDisruptor",
			"GrenadeLauncher", "RocketLauncher", "ParticleCannon", "SithHar", "SpinClaw" };
		for( INT n = 0; n < ARRAY_COUNT(Names); n++ )
		{
			char Path[64];
			appSprintf( Path, "Klingons.%s", Names[n] );
			GObj.LoadClass( AWeapon::StaticClass, NULL, Path, NULL, LOAD_NoWarn | LOAD_KeepImports, NULL );
		}
		INT Given = 0;
		for( TObjectIterator<UClass> It; It; ++It )
		{
			UClass* C = *It;
			if( !C->IsChildOf( AWeapon::StaticClass ) || C == AWeapon::StaticClass || ( C->ClassFlags & CLASS_Abstract ) || appStricmp( C->GetName(), "KlingonWeapons" )==0 )
				continue;
			if( appStricmp( C->GetParent() ? C->GetParent()->GetName() : "", "Klingons" ) != 0 )
				continue;
			AActor* W = L->SpawnActor( C, NAME_None, NULL, NULL, P->Location, P->Rotation, NULL, 0, 1 );
			if( !W )
				continue;
			W->eventTouch( P );
			Given++;
		}
		Wait( 0.5 );
		INT Have = 0;
		for( AInventory* I = P->Inventory; I && Have < 100; I = I->Inventory )
			if( I->IsA( AWeapon::StaticClass ) )
				Have++;
		debugf( NAME_Log, "[KHG] Timing: spawned %i weapon pickups, player now has %i weapons", Given, Have );
	}

	/*-------------------------------------------------------------------------
		Tests.
	-------------------------------------------------------------------------*/

	// Hold one or two keys for some seconds: distance moved, end speed and height change.
	// MoveYaw: direction of the move relative to the view (0 forward, 32768 back, 16384 right).
	void TestMove( const char* Name, const char* Alias, const char* Alias2, DOUBLE Seconds, INT MoveYaw=0 )
	{
		ResetPlayer( -MoveYaw );
		const FVector From = P->Location;
		Press( Alias );
		if( Alias2 )
			Press( Alias2 );
		Wait( Seconds );
		const DOUBLE Speed = sqrt( P->Velocity.X*P->Velocity.X + P->Velocity.Y*P->Velocity.Y );
		ReleaseAll();
		Set( Name, "distance", "units", Dist2D( P->Location, From ), 0.02, 2.0 );
		Set( Name, "end speed", "units/s", Speed, 0.02, 2.0 );
		Wait( 1.0 );
		Set( Name, "stop distance", "units", Dist2D( P->Location, From ), 0.02, 2.0 );
	}

	// Hold a turn or look key: how far the view turns.
	void TestTurnKey( const char* Name, const char* Alias, DOUBLE Seconds, UBOOL Pitch )
	{
		ResetPlayer();
		INT Total = 0, LastV = Pitch ? P->ViewRotation.Pitch : P->ViewRotation.Yaw;
		Press( Alias );
		for( INT i = Steps( Seconds ); i > 0; i-- )
		{
			Tick();
			const INT V = Pitch ? P->ViewRotation.Pitch : P->ViewRotation.Yaw;
			Total += YawDiff( V, LastV );
			LastV = V;
		}
		ReleaseAll();
		Tick();
		Set( Name, Pitch ? "pitch" : "yaw", "rot units", Total, 0.02, 32.0 );
	}

	// Mouse: CountsPerSecond counts, sent as events of EventCounts each 1/EventHz s of game time.
	void TestMouse( const char* Name, DOUBLE EventHz, INT EventCounts, DOUBLE Seconds )
	{
		ResetPlayer();
		INT Total = 0, LastV = P->ViewRotation.Yaw;
		INT Sent = 0;
		const DOUBLE T0 = SimTime;
		for( INT i = Steps( Seconds ); i > 0; i-- )
		{
			// Events that are due by the end of this tick.
			const INT Due = (INT)floor( ( SimTime - T0 + 1.0 / Rate ) * EventHz + 1e-6 );
			for( ; Sent < Due; Sent++ )
				GE->InputEvent( VP, IK_MouseX, IST_Axis, (FLOAT)EventCounts );
			Tick();
			Total += YawDiff( P->ViewRotation.Yaw, LastV );
			LastV = P->ViewRotation.Yaw;
		}
		Wait( 0.25 );
		Total += YawDiff( P->ViewRotation.Yaw, LastV );
		Set( Name, "yaw", "rot units", Total, 0.03, 32.0 );
		Set( Name, "mouse counts", "counts", Sent * EventCounts, 0.0, 0.5 );
	}

	// Jump from standing, or after a run-up: height, air time, distance in the air.
	void TestJump( const char* Name, UBOOL RunUp )
	{
		ResetPlayer();
		if( RunUp )
		{
			Press( "MoveForward" );
			Wait( 0.75 );
		}
		const FVector From = P->Location;
		Press( "Jump" );
		DOUBLE TakeOff = -1, Land = -1, MaxZ = From.Z;
		FVector TakeOffLoc = From, LandLoc = From;
		for( INT i = Steps( 2.0 ); i > 0; i-- )
		{
			Tick();
			if( TakeOff < 0 && P->Physics == PHYS_Falling && P->Velocity.Z > 0.f )
			{
				TakeOff = SimTime;
				TakeOffLoc = P->Location;
			}
			if( TakeOff >= 0 && Land < 0 && P->Physics == PHYS_Walking )
			{
				Land = SimTime;
				LandLoc = P->Location;
			}
			MaxZ = Max<DOUBLE>( MaxZ, P->Location.Z );
			if( GKHGTimingDebug && i > Steps( 2.0 ) - Steps( 0.6 ) )
				debugf( NAME_Log, "[KHG] Timing debug %s t=%.3f z=%.2f vz=%.1f phys=%i state=%s anim=%s", Name, SimTime, P->Location.Z, P->Velocity.Z, (INT)P->Physics, KHGTStateName( P ), *P->AnimSequence );
			if( i == Steps( 2.0 ) - Steps( 0.1 ) )
				Release( "Jump" );
		}
		ReleaseAll();
		Set( Name, "height", "units", MaxZ - From.Z, 0.02, 1.0 );
		Set( Name, "air time", "s", ( TakeOff >= 0 && Land >= 0 ) ? Land - TakeOff : NAN, 0.0, 0.03 );
		if( RunUp )
			Set( Name, "air distance", "units", Dist2D( LandLoc, TakeOffLoc ), 0.03, 3.0 );
	}

	// Count actors this pawn spawns while it fires.
	struct FSpawnWatch
	{
		std::unordered_set<AActor*> Seen;
		void Prime( ULevel* L ) { Seen.clear(); for( INT a = 0; a < L->Num(); a++ ) if( L->Actors(a) ) Seen.insert( L->Actors(a) ); }
		template<class F> void Scan( ULevel* L, F Fn )
		{
			for( INT a = L->iFirstDynamicActor; a < L->Num(); a++ )
			{
				AActor* A = L->Actors(a);
				if( A && Seen.insert( A ).second )
					Fn( A );
			}
		}
	};

	// The player's weapon classes, sorted by name.
	std::vector<UClass*> WeaponClasses()
	{
		std::vector<UClass*> Classes;
		for( AInventory* I = P->Inventory; I && Classes.size() < 32; I = I->Inventory )
			if( I->IsA( AWeapon::StaticClass ) && appStricmp( I->GetClassName(), "KlingonWeapons" ) != 0
			 && std::find( Classes.begin(), Classes.end(), I->GetClass() ) == Classes.end() )
				Classes.push_back( I->GetClass() );
		std::sort( Classes.begin(), Classes.end(), []( UClass* A, UClass* B ) { return appStricmp( A->GetName(), B->GetName() ) < 0; } );
		return Classes;
	}

	// One weapon: select it the way the game does, then hold fire (or alt fire) for 3 s from a standing start.
	void TestWeapon( const char* ClassName, UBOOL Alt )
	{
		{
			{
				ResetPlayer();
				AWeapon* W = NULL;
				for( AInventory* I = P->Inventory; I; I = I->Inventory )
					if( appStricmp( I->GetClassName(), ClassName )==0 )
						W = (AWeapon*)I;
				const std::string Test = KHGTFmt( "%s %s", ClassName, Alt ? "alt fire" : "fire" );
				if( !W )
				{
					Set( Test, "selected", "", 0, 0.0, 0.5 );
					return;
				}
				for( INT Try = 0; Try < 4 && P->Weapon != W; Try++ )
				{
					// The game's own weapon switch: SwitchWeapon <group> cycles through the group.
					char Cmd[64];
					appSprintf( Cmd, "SwitchWeapon %i", (INT)W->InventoryGroup );
					VP->Exec( Cmd, GSystem );
					Wait( 2.0 );
				}
				if( P->Weapon != W )
				{
					debugf( NAME_Log, "[KHG] Timing: could not select %s (holding %s, state %s)", ClassName, P->Weapon ? P->Weapon->GetClassName() : "nothing", KHGTStateName( P->Weapon ) );
					Set( Test, "selected", "", 0, 0.0, 0.5 );
					return;
				}
				UObject* Ammo = (UObject*)W->AmmoType;
				if( Ammo )
					KHGTSetInt( Ammo, "AmmoAmount", Max( KHGTGetInt( Ammo, "MaxAmmo", 999 ), 1 ) );
				const INT Ammo0 = Ammo ? KHGTGetInt( Ammo, "AmmoAmount", 0 ) : 0;
				FSpawnWatch Watch;
				Watch.Prime( L );
				INT Projectiles = 0, Effects = 0;
				const DOUBLE Hold = 3.0;
				Press( Alt ? "AltFire" : "Fire" );
				for( INT i = Steps( Hold + 1.5 ); i > 0; i-- )
				{
					if( i == Steps( 1.5 ) )
						Release( Alt ? "AltFire" : "Fire" );
					Tick();
					Watch.Scan( L, [&]( AActor* A ) { if( A->IsA( AProjectile::StaticClass ) ) Projectiles++; else Effects++; } );
				}
				ReleaseAll();
				if( Ammo )
					Set( Test, "ammo used", "rounds", Ammo0 - KHGTGetInt( Ammo, "AmmoAmount", 0 ), 0.0, 1.0 );
				Set( Test, "projectiles", "actors", Projectiles, 0.0, 1.0 );
				Set( Test, "other actors spawned", "actors", Effects, 0.25, 3.0 );
			}
		}
	}

	// HoloBox (holodeck boxes in M02 and the last missions): Trigger fades the box in, the next Trigger fades
	// it out and locks the player's controls until the fade ends. Measures both fades and the lock.
	void TestHoloBoxes()
	{
		INT n = 0;
		for( INT a = 0; a < L->Num(); a++ )
		{
			AActor* H = L->Actors(a);
			if( !H || H->bDeleteMe || appStricmp( H->GetClassName(), "HoloBox" ) != 0 )
				continue;
			const std::string Test = KHGTFmt( "HoloBox %i", n++ );
			debugf( NAME_Log, "[KHG] Timing: %s = %s index %i (first dynamic %i) static %i state %s tag %s visible %i", Test.c_str(), H->GetName(), a, L->iFirstDynamicActor, (INT)H->bStatic, KHGTStateName( H ), *H->Tag, KHGTGetBool( H, "HolodeckVisible" ) );
			ResetPlayer();
			// Fade in, then out. Trigger toggles, so first get it to a resting state.
			for( INT Phase = 0; Phase < 2; Phase++ )
			{
				const char* Want = KHGTGetBool( H, "HolodeckVisible" ) ? "HoloInvisible" : "HoloVisible";
				const DOUBLE T0 = SimTime;
				H->eventTrigger( P, P );
				DOUBLE Done = NAN, Unlock = NAN;
				UBOOL Locked = 0;
				for( INT i = Steps( 60.0 ); i > 0; i-- )
				{
					Tick();
					const UBOOL NowLocked = appStricmp( KHGTStateName( P ), "LockControls" )==0;
					Locked |= NowLocked;
					if( Locked && !NowLocked && std::isnan( Unlock ) )
						Unlock = SimTime - T0;
					if( std::isnan( Done ) && appStricmp( KHGTStateName( H ), Want )==0 )
						Done = SimTime - T0;
					if( !std::isnan( Done ) && ( !Locked || !std::isnan( Unlock ) ) )
						break;
					if( GKHGTimingDebug && ( i % Steps( 0.5 ) ) == 0 )
						debugf( NAME_Log, "[KHG] Timing debug %s t=%.2f box state %s glow %.3f style %i player %s", Test.c_str(), SimTime - T0, KHGTStateName( H ), H->ScaleGlow, (INT)H->Style, KHGTStateName( P ) );
				}
				Set( Test, appStricmp( Want, "HoloVisible" )==0 ? "fade in" : "fade out", "s", Done, 0.05, 0.05 );
				// The player stays locked after the fade in (the box's next Trigger fades it out); the fade out
				// ends the lock.
				if( Locked && appStricmp( Want, "HoloInvisible" )==0 )
					Set( Test, "controls locked by fade out", "s", Unlock, 0.05, 0.05 );
				Wait( 0.5 );
			}
		}
	}

	// PushActor (wind/conveyor pushers): stand the player in each one that is switched on for 1 s.
	void TestPushActors()
	{
		INT n = 0;
		for( INT a = 0; a < L->Num(); a++ )
		{
			AActor* A = L->Actors(a);
			if( !A || A->bDeleteMe || appStricmp( A->GetClassName(), "PushActor" ) != 0 )
				continue;
			const std::string Test = KHGTFmt( "PushActor %i (frequency %.2f)", n++, KHGTGetFloat( A, "PushFrequency" ) );
			if( !KHGTGetBool( A, "bPushActorOn" ) )
			{
				A->eventTrigger( P, P );
				Wait( 0.1 );
			}
			ReleaseAll();
			L->FarMoveActor( P, A->Location, 0, 1 );
			P->Velocity = FVector(0,0,0);
			const FVector From = P->Location;
			DOUBLE Peak = 0;
			for( INT i = Steps( 1.0 ); i > 0; i-- )
			{
				Tick();
				Peak = Max<DOUBLE>( Peak, P->Velocity.Size() );
			}
			Set( Test, "player moved", "units", ( P->Location - From ).Size(), 0.05, 5.0 );
			Set( Test, "peak speed", "units/s", Peak, 0.05, 5.0 );
		}
	}

	// The map left running with the player standing still: timers, spawned effects, AI, movers.
	void TestWorld( DOUBLE Seconds )
	{
		FName All( "All" );
		P->ReducedDamageType = All;   // the player can't die, so AI keeps doing what it does
		std::map<std::string,INT> Spawned;
		FSpawnWatch Watch;
		Watch.Prime( L );
		std::map<AMover*,INT> MoverMoves;
		std::map<AMover*,BYTE> MoverKey;
		for( INT a = 0; a < L->Num(); a++ )
			if( AMover* M = Cast<AMover>( L->Actors(a) ) )
				MoverKey[M] = M->KeyNum;
		const DWORD Timers0 = GKHGTimerEvents;
		std::map<std::string,INT> TimerCounts;
		GKHGTimerCounts = &TimerCounts;
		GKHGTimerHook = KHGTCountTimer;
		const FLOAT Time0 = L->GetLevelInfo()->TimeSeconds;
		INT PawnsStart = 0;
		for( INT a = 0; a < L->Num(); a++ )
			if( Cast<APawn>( L->Actors(a) ) && !L->Actors(a)->bDeleteMe )
				PawnsStart++;
		for( INT i = Steps( Seconds ); i > 0 && !GIsRequestingExit; i-- )
		{
			Tick();
			Watch.Scan( L, [&]( AActor* A ) { Spawned[A->GetClassName()]++; } );
			for( std::map<AMover*,BYTE>::iterator It = MoverKey.begin(); It != MoverKey.end(); ++It )
				if( It->first->KeyNum != It->second )
				{
					It->second = It->first->KeyNum;
					MoverMoves[It->first]++;
				}
		}
		GKHGTimerHook = NULL;
		GKHGTimerCounts = NULL;
		const DOUBLE Secs = L->GetLevelInfo()->TimeSeconds - Time0;
		Set( "world", "game time", "s", Secs, 0.0, 0.01 );
		Set( "world", "timer events per s", "calls/s", ( GKHGTimerEvents - Timers0 ) / Seconds, 0.05 );
		for( std::map<std::string,INT>::iterator It = TimerCounts.begin(); It != TimerCounts.end(); ++It )
			Set( "world timers", It->first, "calls/s", It->second / Seconds, 0.05, 0.1 );
		INT Total = 0;
		for( std::map<std::string,INT>::iterator It = Spawned.begin(); It != Spawned.end(); ++It )
		{
			Total += It->second;
			Set( "world spawns", It->first, "actors", It->second, 0.25, 3.0 );
		}
		Set( "world", "actors spawned", "actors", Total, 0.15, 5.0 );
		INT Pawns = 0, Alive = 0, Health = 0;
		for( INT a = 0; a < L->Num(); a++ )
		{
			APawn* Pn = Cast<APawn>( L->Actors(a) );
			if( !Pn || Pn->bDeleteMe || Pn == P )
				continue;
			Pawns++;
			if( Pn->Health > 0 )
			{
				Alive++;
				Health += Pn->Health;
			}
		}
		Set( "world", "other pawns alive", "pawns", Alive, 0.0, 0.5 );
		for( INT a = 0; a < L->Num(); a++ )
		{
			APawn* Pn = Cast<APawn>( L->Actors(a) );
			if( Pn && !Pn->bDeleteMe && Pn != P && Pn->Health != ((APawn*)Pn->GetClass()->GetDefaultActor())->Health )
				debugf( NAME_Log, "[KHG] Timing %.0fHz world pawn %s health %i state %s", Rate, Pn->GetName(), Pn->Health, KHGTStateName( Pn ) );
		}
		Set( "world", "other pawns health", "points", Health, 0.10, 10.0 );
		INT Moves = 0;
		for( std::map<AMover*,INT>::iterator It = MoverMoves.begin(); It != MoverMoves.end(); ++It )
			Moves += It->second;
		Set( "world", "mover key changes", "keys", Moves, 0.0, 1.0 );
	}

	// The real main loop for a few seconds per frame cap: frame rate reached and game time vs real time.
	void TestPacing( INT Cap, INT Fixed, DOUBLE Seconds, std::vector<std::string>& Lines )
	{
		const UBOOL NoDraw = GKHGNoDraw;
		GKHGNoDraw = 0;
		FKHGFramePacer Pacer;
		Pacer.Configure( Cap, Fixed );
		const DOUBLE Wall0 = appSeconds();
		const FLOAT Game0 = L->GetLevelInfo()->TimeSeconds;
		INT Frames = 0;
		DOUBLE MinDelta = 1e9, MaxDelta = 0;
		while( appSeconds() - Wall0 < Seconds && !GIsRequestingExit )
		{
			const DOUBLE Delta = Pacer.BeginFrame();
			GE->Tick( Delta );
			L = GE->GLevel;
			Frames++;
			MinDelta = Min( MinDelta, Delta );
			MaxDelta = Max( MaxDelta, Delta );
		}
		GKHGNoDraw = NoDraw;
		const DOUBLE Wall = appSeconds() - Wall0;
		const DOUBLE Game = L->GetLevelInfo()->TimeSeconds - Game0;
		std::string Line = KHGTFmt( "%-22s %8.1f fps   game time / real time %.4f   step %.2f-%.2f ms",
			Fixed ? KHGTFmt( "original timing %i Hz", Fixed ).c_str() : Cap ? KHGTFmt( "frame cap %i", Cap ).c_str() : "no cap",
			Frames / Wall, Game / Wall, MinDelta * 1000.0, MaxDelta * 1000.0 );
		debugf( NAME_Log, "[KHG] Timing pacing: %s", Line.c_str() );
		Lines.push_back( Line );
	}
};

static UBOOL KHGTFlag( const FKHGTMetric& M, INT r )
{
	const DOUBLE Base = M.Values[0], V = M.Values[r];
	if( std::isnan( Base ) != std::isnan( V ) )
		return 1;
	if( std::isnan( Base ) )
		return 0;
	const DOUBLE Diff = fabs( V - Base );
	return Diff > M.TolAbs && Diff > M.Tol * fabs( Base );
}

static void KHGTWriteReport( const char* Dir, const char* Map, const std::vector<INT>& Rates, const FKHGTiming& T, const std::vector<std::string>& Pacing )
{
	const char* Arch = sizeof(void*) == 8 ? "win64" : "win32";
	std::string Base = KHGTFmt( "%s\\Timing-%s-%s", Dir, Map, Arch );
	size_t Dot = Base.rfind( ".unr" );
	if( Dot != std::string::npos )
		Base.erase( Dot, 4 );
	INT Flagged = 0;
	for( size_t m = 0; m < T.Metrics.size(); m++ )
		for( INT r = 1; r < (INT)Rates.size(); r++ )
			if( Rates[r] > 0 )
				Flagged += KHGTFlag( T.Metrics[m], r );

	// CSV: one row per measurement, one column per rate.
	if( FILE* F = fopen( ( Base + ".csv" ).c_str(), "w" ) )
	{
		fprintf( F, "test,measurement,unit" );
		for( size_t r = 0; r < Rates.size(); r++ )
			fprintf( F, Rates[r] > 0 ? ",%dHz" : ",%dHz fixes off", Abs( Rates[r] ) );
		fprintf( F, "\n" );
		for( size_t m = 0; m < T.Metrics.size(); m++ )
		{
			const FKHGTMetric& M = T.Metrics[m];
			fprintf( F, "\"%s\",\"%s\",%s", M.Test.c_str(), M.Name.c_str(), M.Unit.c_str() );
			for( size_t r = 0; r < Rates.size(); r++ )
				fprintf( F, ",%.4f", M.Values[r] );
			fprintf( F, "\n" );
		}
		fclose( F );
	}

	// HTML: the same table with every difference from 60 Hz beyond its tolerance in red.
	if( FILE* F = fopen( ( Base + ".html" ).c_str(), "w" ) )
	{
		fprintf( F, "<!doctype html><meta charset=utf-8><title>KHGWin timing %s %s</title>\n"
			"<style>body{font:14px system-ui,sans-serif;margin:16px;background:#111;color:#ddd}table{border-collapse:collapse}"
			"td,th{padding:3px 8px;border-bottom:1px solid #333;text-align:right}td:nth-child(-n+3),th:nth-child(-n+3){text-align:left}"
			".bad{background:#622;color:#fff}.retail{background:#553a10;color:#fff}.same{color:#7c7}h2{margin-top:24px}pre{background:#1c1c1c;padding:8px}</style>\n", Map, Arch );
		fprintf( F, "<h1>KHGWin timing report: %s, %s</h1>\n", Map, Arch );
		fprintf( F, "<p>Same scripted input at fixed tick rates; every value is compared with the %d Hz run. "
			"Red = outside the tolerance in the last column. %d difference%s flagged. "
			"A \"fixes off\" column runs the retail script unchanged ([KHGWin] FrameRateFixes=0); brown cells there "
			"are what the fixes correct.</p>\n", Rates[0], Flagged, Flagged == 1 ? "" : "s" );
		fprintf( F, "<table><tr><th>test</th><th>measurement</th><th>unit</th>" );
		for( size_t r = 0; r < Rates.size(); r++ )
			fprintf( F, Rates[r] > 0 ? "<th>%d Hz</th>" : "<th>%d Hz<br>fixes off</th>", Abs( Rates[r] ) );
		fprintf( F, "<th>tolerance</th></tr>\n" );
		for( size_t m = 0; m < T.Metrics.size(); m++ )
		{
			const FKHGTMetric& M = T.Metrics[m];
			fprintf( F, "<tr><td>%s</td><td>%s</td><td>%s</td>", M.Test.c_str(), M.Name.c_str(), M.Unit.c_str() );
			for( size_t r = 0; r < Rates.size(); r++ )
			{
				const UBOOL Bad = r > 0 && KHGTFlag( M, (INT)r );
				const char* Class = !Bad ? "" : Rates[r] > 0 ? " class=bad" : " class=retail";
				if( std::isnan( M.Values[r] ) )
					fprintf( F, "<td%s>-</td>", Class );
				else
					fprintf( F, "<td%s>%.2f</td>", Class, M.Values[r] );
			}
			if( M.Tol > 0 )
				fprintf( F, "<td>%.0f%% or %.2f</td></tr>\n", M.Tol * 100.0, M.TolAbs );
			else
				fprintf( F, "<td>%.2f</td></tr>\n", M.TolAbs );
		}
		fprintf( F, "</table>\n<h2>Main loop pacing (real time)</h2>\n<pre>" );
		for( size_t i = 0; i < Pacing.size(); i++ )
			fprintf( F, "%s\n", Pacing[i].c_str() );
		fprintf( F, "</pre>\n" );
		fclose( F );
	}
	debugf( NAME_Log, "[KHG] Timing: report %s.html, %i difference(s) flagged", Base.c_str(), Flagged );
}

void KHGRunTimingTest( UEngine* Engine )
{
	guard(KHGRunTimingTest);
	UGameEngine* GE = Cast<UGameEngine>( Engine );
	if( !GE || !GE->Client || !GE->Client->Viewports.Num() )
		return;
	char Dir[256] = "..\\Timing", MapList[1024] = "M02.unr", RateList[256] = "60,144,240,500,1000,500r";
	Parse( appCmdLine(), "KHGTIMING=", Dir, ARRAY_COUNT(Dir) );
	KHGTArg( "KHGMAP=", MapList, ARRAY_COUNT(MapList) );
	KHGTArg( "KHGMAPS=", MapList, ARRAY_COUNT(MapList) );
	KHGTArg( "KHGRATES=", RateList, ARRAY_COUNT(RateList) );
	INT WorldSeconds = 20;
	Parse( appCmdLine(), "KHGWORLD=", WorldSeconds );
	CreateDirectoryA( Dir, NULL );

	std::vector<INT> Rates;
	for( char* S = RateList; *S; )
	{
		// "500r" = 500 Hz with the frame rate fixes off (retail script behaviour), stored as -500.
		INT R = atoi( S );
		char* E = S;
		while( *E >= '0' && *E <= '9' ) E++;
		if( R >= 10 )
			Rates.push_back( ( *E == 'r' || *E == 'R' ) ? -R : R );
		while( *S && *S != ',' ) S++;
		if( *S ) S++;
	}
	std::vector<std::string> Maps;
	for( char* S = MapList; *S; )
	{
		char* E = S;
		while( *E && *E != ',' ) E++;
		Maps.push_back( std::string( S, E - S ) );
		S = *E ? E + 1 : E;
	}
	GKHGSyntheticInput = 1;
	// -khgnodraw: tick without drawing, for quick runs on a slow renderer. The numbers that count come
	// from runs that draw every frame, like the game does.
	GKHGNoDraw = ParseParam( appCmdLine(), "KHGNODRAW" );
	GKHGTimingDebug = ParseParam( appCmdLine(), "KHGTIMINGDEBUG" );
	KHGTArg( "KHGTESTS=", GKHGTimingOnly, ARRAY_COUNT(GKHGTimingOnly) );
	debugf( NAME_Log, "[KHG] Timing: %i map(s), %i rates", (INT)Maps.size(), (INT)Rates.size() );

	for( size_t m = 0; m < Maps.size() && !GIsRequestingExit; m++ )
	{
		const char* Map = Maps[m].c_str();
		FKHGTiming T;
		T.GE = GE;
		T.VP = GE->Client->Viewports(0);
		T.NumRates = (INT)Rates.size();
		std::vector<std::string> Pacing;
		const INT ConfigFixes = GKHGFrameRateFixes;
		for( INT r = 0; r < (INT)Rates.size() && !GIsRequestingExit; r++ )
		{
			T.RateIndex = r;
			T.Rate = Abs( Rates[r] );
			GKHGFrameRateFixes = Rates[r] < 0 ? 0 : ConfigFixes;
			debugf( NAME_Log, "[KHG] Timing: %s at %d Hz, FrameRateFixes=%i", Map, Abs( Rates[r] ), GKHGFrameRateFixes );
			// Player tests.
			if( !T.LoadMap( Map ) )
				break;
			const UBOOL Control = T.WaitForControl();
			T.Set( "map start", "time until the player can move", "s", Control ? T.SimTime : NAN, 0.0, 0.05 );
			std::vector<std::string> Weapons;
			if( Control )
			{
				T.ClearPawns();
				T.ChooseStart();
				T.BindKeys();
				if( KHGTWant( "move" ) )
				{
					T.TestJump( "jump", 0 );
					T.TestJump( "running jump", 1 );
					T.TestMove( "run forward 2 s", "MoveForward", NULL, 2.0 );
					T.TestMove( "walk forward 2 s", "MoveForward", "Walking", 2.0 );
					T.TestMove( "back 1 s", "MoveBackward", NULL, 1.0, 32768 );
					T.TestMove( "strafe 1 s", "StrafeRight", NULL, 1.0, 16384 );
					T.TestMove( "crouch forward 1.5 s", "MoveForward", "Duck", 1.5 );
				}
				if( KHGTWant( "turn" ) )
				{
					T.TestTurnKey( "turn key 1 s", "TurnRight", 1.0, 0 );
					T.TestTurnKey( "look key 0.25 s", "LookUp", 0.25, 1 );
					T.TestMouse( "mouse 1000 Hz, 1 count", 1000.0, 1, 1.0 );
					T.TestMouse( "mouse 125 Hz, 8 counts", 125.0, 8, 1.0 );
					T.TestMouse( "mouse 60 Hz, 40 counts", 60.0, 40, 1.0 );
				}
				if( KHGTWant( "holo" ) )
					T.TestHoloBoxes();
				if( KHGTWant( "push" ) )
					T.TestPushActors();
				T.GiveWeapons();
				std::vector<UClass*> Classes = T.WeaponClasses();
				for( size_t c = 0; c < Classes.size(); c++ )
					Weapons.push_back( Classes[c]->GetName() );
				T.UnbindKeys();
			}
			// Each weapon and fire mode from a fresh load, so a thrown or emptied weapon can't affect the next one.
			for( size_t w = 0; w < Weapons.size() * 2 && KHGTWant( "weapons" ) && !GIsRequestingExit; w++ )
			{
				if( !T.LoadMap( Map ) || !T.WaitForControl() )
					break;
				T.ClearPawns();
				T.ChooseStart();
				T.BindKeys();
				T.GiveWeapons();
				T.TestWeapon( Weapons[w / 2].c_str(), w & 1 );
				T.UnbindKeys();
			}
			// World test, from a fresh load.
			if( KHGTWant( "world" ) )
			{
				if( !T.LoadMap( Map ) )
					break;
				T.WaitForControl();
				T.TestWorld( WorldSeconds );
			}
		}
		GKHGFrameRateFixes = ConfigFixes;
		// Real-time main loop pacing in the same map.
		if( KHGTWant( "pacing" ) && T.LoadMap( Map ) && !GIsRequestingExit )
		{
			T.Rate = 60;
			T.WaitForControl();
			static const INT Caps[] = { 60, 144, 240, 500, 0 };
			for( INT c = 0; c < ARRAY_COUNT(Caps); c++ )
				T.TestPacing( Caps[c], 0, 3.0, Pacing );
			T.TestPacing( 0, 60, 3.0, Pacing );
		}
		KHGTWriteReport( Dir, Map, Rates, T, Pacing );
	}
	GKHGSyntheticInput = 0;
	GKHGNoDraw = 0;
	debugf( NAME_Log, "[KHG] Timing: done" );
	unguard;
}

#endif
