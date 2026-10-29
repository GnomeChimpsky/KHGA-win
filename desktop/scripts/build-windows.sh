#!/usr/bin/env bash
# Cross-builds the Windows desktop port with MinGW-w64 and packages it as a zip.
#
# usage: build-windows.sh <i686|x86_64> [ffmpeg-source-dir]
#
# Outputs (under build/win32 or build/win64):
#   stage/System/            files to copy into the retail install's System folder
#   KHGWin-<arch>.zip        the same files, zipped
#   KHGWin-<arch>-debug.zip  unstripped KHGWin.exe for crash analysis
#
# FFmpeg is built once per arch into build/ffmpeg-<arch> (see build-ffmpeg.sh). Pass the FFmpeg
# source dir, or set FFMPEG_SRC; it defaults to build/ffmpeg-src (git clone of FFmpeg n6.1.2).
set -euo pipefail

ARCH="$1"
case "$ARCH" in
  i686)   NAME=win32 ;;
  x86_64) NAME=win64 ;;
  *) echo "arch must be i686 or x86_64" >&2; exit 1 ;;
esac

ROOT="$(cd "$(dirname "$0")/../.." && pwd)"
FFMPEG_SRC="${2:-${FFMPEG_SRC:-$ROOT/build/ffmpeg-src}}"
FFMPEG_PREFIX="$ROOT/build/ffmpeg-$ARCH"
BUILD="$ROOT/build/$NAME"

if [ ! -f "$FFMPEG_PREFIX/include/libavcodec/avcodec.h" ] || [ ! -f "$FFMPEG_PREFIX/COPYING.LGPLv2.1" ]; then
  if [ ! -d "$FFMPEG_SRC" ]; then
    git clone --depth 1 -b n6.1.2 https://github.com/FFmpeg/FFmpeg "$FFMPEG_SRC"
  fi
  "$ROOT/desktop/scripts/build-ffmpeg.sh" "$ARCH" "$FFMPEG_SRC" "$FFMPEG_PREFIX"
fi

cmake -S "$ROOT/desktop" -B "$BUILD" -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE="$ROOT/desktop/cmake/mingw-w64-$ARCH.cmake" \
  -DCMAKE_BUILD_TYPE=RelWithDebInfo \
  -DFFMPEG_ROOT="$FFMPEG_PREFIX"
rm -rf "$BUILD/stage"
cmake --build "$BUILD"

STAGE="$BUILD/stage/System"
mkdir -p "$BUILD/debug"
cp "$STAGE/KHGWin.exe" "$BUILD/debug/"
"$ARCH-w64-mingw32-strip" --strip-unneeded "$STAGE"/*.exe "$STAGE"/*.dll
cp "$ROOT/desktop/README-Windows.txt" "$STAGE/KHGWin-README.txt"
mkdir -p "$STAGE/KHGWin-Licenses"
cp "$ROOT/THIRDPARTY.md" "$STAGE/KHGWin-Licenses/"
cp "$ROOT/app/src/main/cpp/thirdparty/SDL2/LICENSE.txt" "$STAGE/KHGWin-Licenses/SDL2.txt"
cp "$ROOT/app/src/main/cpp/thirdparty/openal-soft/COPYING" "$STAGE/KHGWin-Licenses/OpenAL-Soft.txt"
cp "$FFMPEG_PREFIX/COPYING.LGPLv2.1" "$STAGE/KHGWin-Licenses/FFmpeg-LGPL-2.1.txt"
cp "$ROOT/app/src/main/cpp/thirdparty/libxmp/docs/COPYING" "$STAGE/KHGWin-Licenses/libxmp.txt"

rm -f "$BUILD/KHGWin-$NAME.zip" "$BUILD/KHGWin-$NAME-debug.zip"
(cd "$BUILD/stage" && zip -qr "$BUILD/KHGWin-$NAME.zip" System)
(cd "$BUILD/debug" && zip -q "$BUILD/KHGWin-$NAME-debug.zip" KHGWin.exe)
echo "Packaged $BUILD/KHGWin-$NAME.zip"
