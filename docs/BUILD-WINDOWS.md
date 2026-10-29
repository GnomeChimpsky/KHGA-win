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
sudo apt install mingw-w64 g++-mingw-w64 cmake ninja-build nasm zip git
```

## Build

```bash
desktop/scripts/build-windows.sh x86_64    # build/win64/KHGWin-win64.zip
desktop/scripts/build-windows.sh i686      # build/win32/KHGWin-win32.zip
```

The first run clones FFmpeg n6.1.2 and builds the minimal LGPL configuration used for the
cutscenes (`desktop/scripts/build-ffmpeg.sh`, same components as the Android build). Later runs
reuse it from `build/ffmpeg-<arch>`.

`build/<win32|win64>/stage/System/` holds the files to copy into a retail install's `System`
folder. The zip contains the same files plus a README and the third-party licenses.

CI (`.github/workflows/build.yml`) runs the same script for both architectures on every push and
pull request and uploads the zips as artifacts. It also builds the Android debug APK so changes
to the shared engine tree are checked on both platforms.

## Running

Copy the staged files into the retail `System` folder and run `KHGWin.exe`. On first run it
writes `KHGWin.ini` from the retail `Default.ini` (or `Khg.ini`), replacing only the driver keys
listed in `desktop/config/KHGWinDefaults.ini`. The log is `KHGWin.log`; every native function the
game needs but the engine lacks is logged with the tag `[KHG] MissingNative`.

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
| `app/src/main/cpp/UE1/Source/Unreal/Src/KHGDesktopConfig.*` | First-run `KHGWin.ini` |
