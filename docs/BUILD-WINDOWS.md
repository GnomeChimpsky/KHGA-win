# Building the Windows desktop port

The Windows build shares the engine tree under `app/src/main/cpp/` with the Android build and
adds its own CMake entry point in `desktop/`. It cross-compiles with MinGW-w64 from Linux and
produces a single `KHGWin.exe` (engine, SDL2, libxmp statically linked) plus `OpenAL32.dll` and
the FFmpeg DLLs, which stay dynamic because they are LGPL.

Both a 32-bit (`i686`) and a 64-bit (`x86_64`) build are produced. The 64-bit build is the one
meant for players; the 32-bit build is the fidelity reference, since the retail game is 32-bit.

## Requirements

Ubuntu 24.04 or similar (WSL works) with:

```bash
sudo apt install mingw-w64 g++-mingw-w64 cmake ninja-build nasm zip git patch
```

## Build

```bash
desktop/scripts/build-windows.sh x86_64    # build/win64/KHGWin-<ver>-win64.zip
desktop/scripts/build-windows.sh i686      # build/win32/KHGWin-<ver>-win32.zip
desktop/scripts/package-lgpl-sources.sh    # build/KHGWin-<ver>-lgpl-sources.zip
```

`<ver>` is `git describe --tags --always --dirty` (or `$KHG_VERSION`); it is also written into
`KHGWin.exe` (version resource, log line `[KHG] KHGWin <ver>`, crash reports) and
`KHGWin-version.txt`.

The first run clones FFmpeg n6.1.2 and builds the minimal LGPL configuration used for the
cutscenes (`desktop/scripts/build-ffmpeg.sh`, same components as the Android build). Later runs
reuse it from `build/ffmpeg-<arch>`.

## Release contents

`build/<win32|win64>/stage/KHGWin/` is the release folder, zipped as `KHGWin-<ver>-<arch>.zip`:
`KHGWin.exe`, `OpenAL32.dll`, the FFmpeg DLLs, `KHGWin.int`, `KHGWinDefaults.ini`, the README,
`KHGWin-version.txt` and `KHGWin-Licenses/`. No game data. `KHGWin-<ver>-<arch>-debug.zip` holds
the unstripped `KHGWin.exe` and `symbolize-crash.sh`, which turns the `KHGWin.exe+0x...` lines
of a crash report into function names and source lines.

The folder runs from anywhere: `KHGDesktopChooseGameDir` (`KHGDesktopConfig.cpp`) finds the
retail install (`-gamedir=`, the exe's own folder, `[KHGWin] GameDir`, the folders around the
exe, else a folder dialog) and the engine runs in its `System` folder (`appSetBaseDir`). The
port's own files are looked up next to the exe (`appExeDir`). Copying the files into the retail
`System` folder still works, as the test bats in `desktop/tools` do.

## CI

`.github/workflows/build.yml` runs on every push and pull request: both Windows architectures
(artifacts `KHGWin-<arch>` = the release folder, `KHGWin-<arch>-zips` = release + debug zip),
the LGPL source zip, and the Android debug APK so changes to the shared engine tree are checked
on both platforms. Pushing a tag `v<x.y.z>` also creates a **draft** GitHub release with all the
zips; it stays private until someone publishes it on GitHub.

## Running

Run `KHGWin.exe` from the release folder (or from the retail `System` folder). On first run it
writes `KHGWin.ini` from the retail `Default.ini` (or `Khg.ini`), replacing only the driver keys
listed in `desktop/config/KHGWinDefaults.ini`. The log is `KHGWin.log`; every native function the
game needs but the engine lacks is logged with the tag `[KHG] MissingNative`. Crashes write a
report to `Crashes\` in the user folder (`KHGCrash.cpp`); `-khgcrashtest=av|error|abort` tests
that, `-khgnocrashdialog` leaves out the message box.

## Layout

| Path | What it is |
|---|---|
| `desktop/CMakeLists.txt` | Desktop entry point: third-party libs, engine options, staging |
| `desktop/cmake/` | MinGW-w64 toolchain files |
| `desktop/config/KHGWinDefaults.ini` | Driver settings applied on top of the retail defaults |
| `desktop/res/KHGWin.rc` | Icon and version resource for `KHGWin.exe` |
| `desktop/scripts/` | FFmpeg and package build scripts |
| `app/src/main/cpp/UE1/Source/VulkanDrv/PlatformSurface.*` | Vulkan surface per platform (Android / Win32) |
| `app/src/main/cpp/UE1/Source/Unreal/Src/KHGSplash.*` | Reads the MicroProse splash from the user's `Khg.exe` |
| `app/src/main/cpp/UE1/Source/Unreal/Src/KHGDesktopConfig.*` | First-run `KHGWin.ini`, user folder, game folder |
| `app/src/main/cpp/UE1/Source/Unreal/Src/KHGCrash.*` | Crash reports (minidump, call stack, log) |
| `app/src/main/cpp/UE1/Source/Unreal/Src/KHGLauncher.*` | Settings window before the game starts |
| `desktop/licenses/LGPL-NOTICE.txt` | LGPL notice shipped in the release |
