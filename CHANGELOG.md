# Changelog

All notable changes to KHGWin, the Windows port of *Star Trek: Klingon Honor Guard*.

## [1.0.0] - 2026-11-05

The first release: a native 64-bit Windows 10/11 build, forked from the Android port and
brought to retail PC fidelity. Needs your own copy of the game; contains no game data.

### Install and setup

- `KHGWin.exe` runs from any folder. It finds the game install next to or around its own folder,
  remembers it, and otherwise asks for it. `-gamedir=<folder>` and `-pickgamedir` choose another
  one.
- Installs the game from **disc 1** (a drive or a `.cue/.bin/.iso` image) when no install is
  found, because the 1998 InstallShield setup doesn't run on 64-bit Windows. Nothing is written
  to the registry.
- Copies the movies and CD music from **disc 2** into the install once (the `AVI` folder into
  `System`, the audio tracks ripped to `Music\TrackNN.wav`). Offered at startup when they are
  missing and from the launcher's "Game CD..." button. Files go to Saved Games when the install
  can't be written to.
- Works with retail 1.0, 1.0 patched to 1.1, and installs prepared by others (Collection
  Chamber). All 40 maps load and play the same on 1.0 and 1.1.
- Launcher picture on 1.0 installs: the disc's autorun backdrop, since 1.0 has no MicroProse
  splash.

### Launcher

- Settings window before the game starts, with five tabs (Display, Graphics, Game, Controls,
  Audio) and a short description on hover. Only changed settings are written.
- Adds settings the game's menus lack: monitor, any resolution, borderless fullscreen, vsync,
  frame cap, MSAA, anisotropic filtering, field of view, widescreen, lighting, fine mouse
  sensitivity, raw mouse, controller, audio device and music.
- "Reset settings" (keeps a `.bak` and your saves), portable mode toggle, log viewer.
- "Show this window at startup" can be turned off; hold Shift or use `-launcher` to bring it
  back.

### Graphics

- Vulkan renderer matched to the retail Glide driver: world lighting at the Glide lightmap level
  (`LightMode=3`), HUD tiles with Glide 16-bit colour math, masked translucent surfaces without
  lightmaps, mirror floors at a quarter of the Android strength (`MirrorReflection=0.25`).
- No more black triangles along the bottom edge at resolutions above 1200 rows.
- Resizable window, Alt+Enter, borderless fullscreen, any resolution, `setres` and `vsync`
  console commands, DPI aware.
- Widescreen: Hor+ field of view on by default; HUD panels anchored 4:3 in the corners with the
  weapon bar centred; menus, title splash and legal screen at 4:3 with black bars. `HorPlus` and
  `WideHUD` switch back to the retail stretch.
- Optional field of view (`[KHGWin] FOV`); menus keep the retail FOV so the character select
  screen lines up.

### Startup and movies

- Retail startup order: MicroProse splash while loading, then the title splash and legal screen.
  The intro movie plays only from the video log menu, as in retail (`PlayIntroAtStartup=True`
  plays it at startup). A mouse click skips the splash screens.
- Movies play like the retail `PlayAVI`: 640x480 at 4:3, the Klingon console frame built up and
  broken down around briefings, retail skip rules. Indeo 5 transparency is decoded (patched
  FFmpeg). Esc, Space, Enter or a mouse click skips a movie.

### Audio

- CD music from ripped files (`.ogg`, `.flac`, `.mp3`, `.wav`, `.opus`) in the `Music` folder,
  played once per track like the CD; `cdtrack <n>` console command.
- Automatic music choice (`MusicMode=0`): a level plays its CD track when there is a file for it,
  otherwise its module music, never both. Fixes the doubled music on fan-patched maps.
- Retail Galaxy software reverb rebuilt for reverb zones (six taps, feedback, treble damping), on
  sound effects only. `UseReverb=False` turns it off.
- Movie sound and game sound go to the default Windows device (WASAPI).

### Input

- Keyboard read as Windows key codes like retail, so retail key bindings mean the same keys on
  any layout. Raw mouse input (`RawMouseInput=False` for the retail cursor feel).
- Controllers work when plugged in, with a twin-stick layout (bindable in the controls menu as
  Joy1, Joy2, ...). Smooth stick turning at any uptime.
- W/A/S/D with mouse look by default.

### Frame rate

- Frame pacer with optional cap (`FrameRateLimit`) and a 1 ms floor, so the game no longer runs
  ahead of real time above 1000 fps.
- Retail scripts that count frames (holodeck box fade and control lock, pushers, particle
  attractors, effects) run at 60 Hz above 60 fps (`FrameRateFixes=2`). `OriginalTiming=True`
  gives fixed 1/60 s steps.

### Saves and settings

- Settings, log, saves, screenshots and crash reports live in
  `Saved Games\Klingon Honor Guard`, so the install can stay in Program Files and is never
  written to.
- First start sets up `KHGWin.ini` from your retail `Default.ini`/`Khg.ini` (key bindings and
  options carry over) and copies retail saves and their names, including Windows' VirtualStore
  copies.
- Portable mode: `portable.txt` next to the exe (or `-portable`); `-userdir=<folder>`.
- Tested: saves move both ways between the retail game, 64-bit and 32-bit KHGWin in every map
  ([docs/Saves.md](docs/Saves.md)).

### Mods

- Retail-style mods (KHG Restored) work: the keys they change in `Khg.ini` are copied into
  `KHGWin.ini` on every start, and uninstalling the mod turns it off again. `ModSync=False`
  turns this off.

### Fixes to the engine and the Android base

- 64-bit: skipped `None` context expressions clear the full member size ("Accessed invalid object"
  fix); file search handles no longer truncated.
- `[DefaultPlayer]` options reach the first map.
- The in-game Audio/Video menu no longer blanks audio settings (`SET`/`GET` use the live audio
  driver) and no longer resets the resolution to 1280x720. Damaged `KHGWin.ini` audio sections
  are repaired at start.
- Command-line paths with spaces work when the whole argument is quoted.
- The 32-bit build uses SSE2 math, so 32-bit and 64-bit play identically.

### Release and diagnostics

- Crash reports: a readable `.txt`, a `.dmp` and the log in `Crashes\`; the `-debug` zip and
  `symbolize-crash.sh` turn them into source lines.
- Version from `git describe` in the exe, the log and `KHGWin-version.txt`.
- Release zip with README and all license notices; FFmpeg and OpenAL Soft source zip for the
  shipped LGPL DLLs. Releases are 64-bit only; the 32-bit build stays as a fidelity reference.
- Test tools for Windows in `desktop/tools` (map, save, replay, render, widescreen, timing and
  save compatibility tests).

### Known issues

- CD audio from a physical drive is tested only with images and an emulated drive.
- The engine's own frame-rate effects remain at very high rates (see
  [docs/Timing.md](docs/Timing.md)).
- Multiplayer, Android saves and hub saves are untested.
