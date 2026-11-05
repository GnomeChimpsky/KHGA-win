#!/usr/bin/env bash
# Cross-builds the Windows desktop port with MinGW-w64 and packages it as a zip.
#
# usage: build-windows.sh <i686|x86_64> [ffmpeg-source-dir]
#
# Outputs (under build/win32 or build/win64), <ver> = git describe (or $KHG_VERSION):
#   stage/KHGWin/                    the release folder: KHGWin.exe, its DLLs and files, README, licences.
#                                    Runs from anywhere (it finds or asks for the retail install), or can be
#                                    copied into the install's System folder like earlier builds.
#   KHGWin-<ver>-<arch>.zip          that folder, zipped (no game data)
#   KHGWin-<ver>-<arch>-debug.zip    unstripped KHGWin.exe + symbolize-crash.sh, for crash reports
# desktop/scripts/package-lgpl-sources.sh makes the matching LGPL source zip.
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

# Rebuild FFmpeg when it is missing or build-ffmpeg.sh or our FFmpeg patches changed since it was built.
PATCH_SHA="$(cat "$ROOT"/desktop/scripts/build-ffmpeg.sh "$ROOT"/desktop/patches/ffmpeg-*.patch 2>/dev/null | sha1sum | cut -d' ' -f1)"
if [ ! -f "$FFMPEG_PREFIX/include/libavcodec/avcodec.h" ] || [ ! -f "$FFMPEG_PREFIX/COPYING.LGPLv2.1" ] \
   || [ "$(cat "$FFMPEG_PREFIX/khg-patches.sha1" 2>/dev/null)" != "$PATCH_SHA" ]; then
  if [ ! -d "$FFMPEG_SRC" ]; then
    git clone --depth 1 -b n6.1.2 https://github.com/FFmpeg/FFmpeg "$FFMPEG_SRC"
  fi
  "$ROOT/desktop/scripts/build-ffmpeg.sh" "$ARCH" "$FFMPEG_SRC" "$FFMPEG_PREFIX"
fi

VERSION="${KHG_VERSION:-$(git -C "$ROOT" describe --tags --always --dirty --abbrev=7 2>/dev/null || echo unknown)}"

cmake -S "$ROOT/desktop" -B "$BUILD" -G Ninja \
  -DKHG_VERSION="$VERSION" \
  -DCMAKE_TOOLCHAIN_FILE="$ROOT/desktop/cmake/mingw-w64-$ARCH.cmake" \
  -DCMAKE_BUILD_TYPE=RelWithDebInfo \
  -DFFMPEG_ROOT="$FFMPEG_PREFIX"
rm -rf "$BUILD/stage"
cmake --build "$BUILD"

STAGE="$BUILD/stage/KHGWin"
mkdir -p "$BUILD/debug"
cp "$STAGE/KHGWin.exe" "$BUILD/debug/"
"$ARCH-w64-mingw32-strip" --strip-unneeded "$STAGE"/*.exe "$STAGE"/*.dll
cp "$ROOT/desktop/README-Windows.txt" "$STAGE/KHGWin-README.txt"
mkdir -p "$STAGE/KHGWin-Licenses"
cp "$ROOT/THIRDPARTY.md" "$STAGE/KHGWin-Licenses/"
cp "$ROOT/desktop/licenses/LGPL-NOTICE.txt" "$STAGE/KHGWin-Licenses/"
cp "$ROOT/app/src/main/cpp/thirdparty/SDL2/LICENSE.txt" "$STAGE/KHGWin-Licenses/SDL2.txt"
cp "$ROOT/app/src/main/cpp/thirdparty/openal-soft/COPYING" "$STAGE/KHGWin-Licenses/OpenAL-Soft.txt"
cp "$FFMPEG_PREFIX/COPYING.LGPLv2.1" "$STAGE/KHGWin-Licenses/FFmpeg-LGPL-2.1.txt"
cp "$ROOT"/desktop/patches/ffmpeg-*.patch "$STAGE/KHGWin-Licenses/"   # our FFmpeg changes (LGPL)
cp "$ROOT/app/src/main/cpp/thirdparty/libxmp/docs/COPYING" "$STAGE/KHGWin-Licenses/libxmp.txt"
cp "$ROOT/app/src/main/cpp/UE1/Source/ZVulkan/LICENSE.md" "$STAGE/KHGWin-Licenses/ZVulkan-volk-VMA-glslang.txt"
cp "$ROOT/app/src/main/cpp/thirdparty/unshield/LICENSE" "$STAGE/KHGWin-Licenses/unshield.txt"
sed -n '1,/^ \*\//p' "$ROOT/app/src/main/cpp/thirdparty/unshield/lib/md5/md5c.c" > "$STAGE/KHGWin-Licenses/unshield-RSA-MD5.txt"
cp "$ROOT/app/src/main/cpp/thirdparty/zlib/LICENSE" "$STAGE/KHGWin-Licenses/zlib.txt"
# MinGW-w64 runtime (statically linked C runtime start-up code).
if [ -f /usr/share/doc/mingw-w64-common/copyright ]; then
  cp /usr/share/doc/mingw-w64-common/copyright "$STAGE/KHGWin-Licenses/MinGW-w64-runtime.txt"
fi
echo "$VERSION" > "$STAGE/KHGWin-version.txt"

cp "$ROOT/desktop/scripts/symbolize-crash.sh" "$BUILD/debug/"
ZIP="$BUILD/KHGWin-$VERSION-$NAME.zip"
DEBUGZIP="$BUILD/KHGWin-$VERSION-$NAME-debug.zip"
rm -f "$BUILD"/KHGWin-*.zip
(cd "$BUILD/stage" && zip -qr "$ZIP" KHGWin)
(cd "$BUILD/debug" && zip -q "$DEBUGZIP" KHGWin.exe symbolize-crash.sh)
echo "Packaged $ZIP"
