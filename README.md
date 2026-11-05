# KHG-Win: Klingon Honor Guard, a port and complete fixpack for Windows 10/11

A native 64-bit Windows port of *Star Trek: Klingon Honor Guard* (MicroProse, 1998), the
**Unreal Engine 1** first-person shooter. It runs the original retail game data on modern
Windows with a Vulkan renderer, and keeps the game looking, sounding and playing the way the
1998 PC release does. Widescreen, high frame rates and modern controllers are supported.

No game data is included. You need your own copy of the game: the retail CDs, disc images or an existing
install. KHGWin does not overwrite files or write anything to the registry.

KHGWin is a fork of the Android port
[imjustadudegamer/Klingon-Honor-GUARD-Android](https://github.com/imjustadudegamer/Klingon-Honor-GUARD-Android)
and shares its engine tree, which is adapted to be binary-compatible with KHG's **build-219**
fork of the engine as well as retail 1.0/1.1. The Android build still compiles from this repository.

## Quick start
Using the Collection Chamber release (recommended):
1. Install the game in any location
2. Download `KHGWin-<version>-win64.zip` from [Releases](../../releases) and unzip it anywhere.
3. Run `KHGWin.exe`. KHGWin finds the install next to or around its own folder, or
   asks for it once ("Installed game...").
4. No updates needen. The Collection Chamber release is already v1.1.
5. The launcher window opens: pick your settings, then click **Play**.

Using retail discs or disc images (.bin/.cue recommended):
1. Download `KHGWin-<version>-win64.zip` from [Releases](../../releases) and unzip it anywhere.
2. Run `KHGWin.exe`. The 1998 installer doesn't run on 64-bit Windows, so KHGWin installs
   the game itself from **disc 1** in a drive or a `.cue/.bin/.iso` image. This process might take a while.
3. KHGWin detects that movies and CD music from disc 2 are missing and offers to copy them from **disc 2** into
   the install folder (movies from its `AVI` folder, music ripped from its audio tracks). This might also take a while.
4. **Optional** Install patch 1.1. The patch is only needed for certain mods and for multiplayer against players running 1.1.    
5. The launcher window opens: pick your settings, then click **Play**.

Full player instructions, every setting and its `KHGWin.ini` key are in
[desktop/README-Windows.txt](desktop/README-Windows.txt), shipped in the zip as
`KHGWin-README.txt`.

### Requirements

- Windows 10 or 11, 64-bit.
- A GPU with a Vulkan driver.
- *Star Trek: Klingon Honor Guard*: the retail CD, version 1.0 or patched to 1.1, or an install
  prepared by others (such as the Collection Chamber release). Patch 1.1 is optional (it adds
  the MicroProse splash and is needed by mods made for 1.1). No Indeo codec, no-CD exe,
  dgVoodoo or edited `Khg.ini` is needed.

## What it does

- **Retail look.** A Vulkan render device lights the world at the retail Glide level, draws the
  HUD with Glide's 16-bit colour math and handles masked, translucent and mirror surfaces like
  Glide. Checked side by side against screenshots from the retail game in every mission.
- **Retail startup and movies.** MicroProse splash while loading, then the title splash and the
  legal screen, as in retail. Briefings and cutscenes play like the retail `PlayAVI`, inside the
  Klingon console frame, with Indeo 5 transparency decoded.
- **Music and sound.** CD tracks from ripped files, or the module music of fan-patched installs,
  picked automatically per level so the two never play together. Reverb zones echo with a rebuild
  of the retail Galaxy software reverb.
- **Widescreen.** Hor+ field of view; HUD panels keep their 4:3 shape in the screen corners and
  menus show at 4:3 with black bars. `HorPlus` and `WideHUD` switch back to the retail stretch.
- **High frame rates.** Optional frame cap, a 1 ms floor above 1000 fps, and the retail scripts
  that count frames instead of time (the holodeck box fade, pushers, effects) run at 60 Hz, so
  144 or 240 fps plays like 60 fps.
- **Input.** Keys are read as Windows key codes like the retail game, so your retail bindings
  carry over. Raw mouse, and controllers with a twin-stick layout as soon as they are plugged in.
- **Launcher.** A settings window before the game: display mode, any resolution, monitor, vsync,
  frame cap, MSAA, anisotropy, field of view, widescreen, lighting, mouse, controller, audio
  device and music, each with a short description.
- **Saves and settings.** Kept in `Saved Games\Klingon Honor Guard`, so the install can stay in
  Program Files. Retail saves and names are copied over on the first start. Saves move freely
  between the retail game and KHGWin ([docs/Saves.md](docs/Saves.md)). `portable.txt` keeps
  everything next to the exe instead.
- **Mods.** Retail-style mods such as KHG Restored install the usual way; KHGWin picks up the
  keys they change in `Khg.ini` on every start.
- **Crash reports.** A readable report, a crash dump and the log go to `Crashes\` in the user
  folder.
- **Never touches the original files.** `Khg.exe` is only read, for the splash image.

## Known issues

- Reading CD audio from a physical drive (the disc 2 music copy) is tested only with disc images
  and an emulated drive. Please report how it works with a real drive.
- The engine's own frame-rate effects remain at very high rates: jumps are about 2.6% lower at
  500 fps and above, and some walking and collision details differ (see
  [docs/Timing.md](docs/Timing.md)).
- Multiplayer has not been tested.
- Saves from the Android port, and hub saves, have not been tested.

Please report problems with the crash report files and `KHGWin.log` attached.

## Documentation

- [desktop/README-Windows.txt](desktop/README-Windows.txt): the player README, with every setting.
- [CHANGELOG.md](CHANGELOG.md): what changed in each release.
- [docs/BUILD-WINDOWS.md](docs/BUILD-WINDOWS.md): building, release contents, CI.
- [docs/ARCHITECTURE.md](docs/ARCHITECTURE.md): engine base, source layout, build-219 compatibility.
- [docs/RENDERER.md](docs/RENDERER.md): the Vulkan render device.
- [docs/FMV.md](docs/FMV.md): movies and the console frame.
- [docs/Audio.md](docs/Audio.md): music, reverb and the audio settings.
- [docs/Timing.md](docs/Timing.md): frame rate measurements and the frame-counting scripts.
- [docs/Saves.md](docs/Saves.md): save compatibility between the retail game and KHGWin.
- [docs/CONTROLS.md](docs/CONTROLS.md): controller configuration.
- [docs/BUILD.md](docs/BUILD.md): building the Android app.

## License and credits

This is an **unofficial, non-commercial fan project**, not affiliated with or endorsed by Epic
Games, MicroProse, Paramount, Microsoft or any rights holder. It contains **no game assets**; you
supply your own copy of the game.

- **Unreal Engine** © Epic Games. The engine baseline derives from publicly available UE1 v200
  source with proprietary assets removed.
- **Star Trek: Klingon Honor Guard** © MicroProse and the respective rights holders.
- Built on the Android port by **imjustadudegamer**, **[fgsfdsfgs/UE1](https://github.com/fgsfdsfgs/UE1)**
  and **Andiweli/Unreal-Android**. Uses **SDL2**, **OpenAL Soft**, **libxmp**, **ZVulkan**,
  **volk**, **VMA**, **unshield** and **FFmpeg** (LGPL-2.1, dynamically linked; the sources of the
  shipped DLLs are in the release's `lgpl-sources` zip).

Full attribution is in [CREDITS.md](CREDITS.md); the third-party license notices are in
[THIRDPARTY.md](THIRDPARTY.md) and in the `KHGWin-Licenses` folder of the release.
