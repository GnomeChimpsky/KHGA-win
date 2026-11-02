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

Results: see the table below (filled in from the test logs).

| Save written by | Loaded by KHGWin win64 | Loaded by KHGWin win32 | Loaded by retail Khg.exe |
|---|---|---|---|
| Retail Khg.exe 219 | pending | pending | (native) |
| KHGWin win64 | (native) | pending | pending |
| KHGWin win32 | pending | (native) | pending |
| Android port | not tested: no Android saves available | not tested | not tested |
