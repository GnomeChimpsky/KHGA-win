# Saves and config (desktop build)

## Where KHGWin keeps its files

| Mode | How to select | KHGWin.ini, KHGWin.log, screenshots | Saves |
|---|---|---|---|
| Default | nothing | `<Saved Games>\Klingon Honor Guard\` | `<Saved Games>\Klingon Honor Guard\Save\` |
| Portable | `portable.txt` next to KHGWin.exe, or `-portable` | the System folder | the install's `Save` folder (shared with retail, like retail) |
| Other folder | `-userdir=<dir>` (relative to System unless absolute) | `<dir>` | `<dir>\Save\` |

`SavePath`/`CachePath` in the ini keep the retail value `..\Save` / `..\Cache`; in the default and
`-userdir` modes that value means "the user folder" (set at startup, not written back). Any other
value in the ini is used as it is. `LOG=` and `INI=` work as in retail (relative to System).

The Saved Games path is taken from `SHGetKnownFolderPath(FOLDERID_SavedGames)`. The engine opens
files with ANSI paths, so a user name outside the ANSI code page falls back to the 8.3 short path,
and to portable mode if even that fails (logged as a warning).

## First-run migration (default mode only)

When `<Saved Games>\Klingon Honor Guard\KHGWin.ini` does not exist yet:

1. If an older KHGWin left `System\KHGWin.ini`, it is copied (with the pending input-default steps
   applied); otherwise the ini is created from the retail `Default.ini` (or `Khg.ini`) plus
   `KHGWinDefaults.ini`, as before.
2. Every `*.usa` in the retail save folder (`[Core.System] SavePath` of `Khg.ini`, default `..\Save`)
   and in its VirtualStore copy (`%LOCALAPPDATA%\VirtualStore\...`, where Windows puts the writes of
   a retail game installed under Program Files) is copied to `Save\` unless a file of that name is
   already there. The old port ini's `SavePath`, if different, is copied from too.
3. The save slot names (`[Klingons.KlingonMenuSlot] SlotNames[]`, which the load menu shows) come
   from the old `System\KHGWin.ini`, or else from `Khg.ini` (its VirtualStore copy first).

Nothing in the install is changed. `KHGWin.log` lists each step as `[KHG] Migration: ...`.
In portable mode the saves are shared with retail, so when `KHGWin.ini` has no slot names yet they
are taken from `Khg.ini`.

## Save compatibility

Tested with `builds\tools\SaveCompat.bat` (desktop/tools): the retail Khg.exe (build 219, patch 1.1)
saves once in every single-player map (`savegame 1xx` from the console), both KHGWin builds load
those saves and log every pawn, both KHGWin builds save in every map, each loads the other's saves,
and the retail Khg.exe loads the KHGWin saves.

Results of the run on 2026-11-02/03 (build 80d650e, Windows 11, retail install patched to 1.1).
Retail saves are written with `savegame 9` and renamed to slot 1xx, because the retail SAVEGAME
command only takes a single digit.

| Save written by | Loaded by KHGWin win64 | Loaded by KHGWin win32 | Loaded by retail Khg.exe |
|---|---|---|---|
| Retail Khg.exe 219 (27 single-player maps, after ~12 s of play) | 27/27 | 27/27, pawn states identical to win64 | (native) |
| Retail Khg.exe 219, player's own menu save (M01) | 1/1 (M2, 9fa9035) | 1/1 (M2, 9fa9035) | (native) |
| KHGWin win64 (all 40 maps incl. deathmatch, after 300 ticks) | 40/40 | 40/40, pawn states identical | 27/27 single-player maps |
| KHGWin win32 (same) | 40/40 | 40/40 | 27/27 single-player maps |
| Android port | not tested: no Android saves available | not tested | not tested |

Notes:
- "Pawn states identical" compares every pawn's AI state, physics, health, zone, enemy and weapon
  in the KHGWin logs (positions are left out; they differ only by float rounding). A win64 save and a
  win32 save of the same map have the same size; 16 bytes differ (the package GUID).
- On load the player goes from IntroScene/LockControls to PlayerWalking. That is stock UE1 behaviour
  (the player is re-possessed), the same for every save, including retail ones.
- Retail loads were checked by the retail log (the player is possessed in the loaded Save package)
  and a screenshot each (builds\tools\Compat\Shots). One retail run (win64 m19 save) crashed in the
  retail Glide shutdown when quitting, after the save had loaded and run: a retail exit crash.
- Script warnings after loading retail saves (missing animation sequences WaitGL/WaitBA, Accessed
  None in DWCaptain.OverHeadDamageTarget and BireQT.BiteDamageTarget) are the original scripts and
  appear identically on both builds.
- Hub saves (Save<n><i>.usa) were not covered: the test saves are made at map start, where the hub
  stack is empty.
