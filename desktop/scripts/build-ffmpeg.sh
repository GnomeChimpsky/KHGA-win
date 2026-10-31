#!/usr/bin/env bash
# Cross-builds the minimal LGPL FFmpeg used by the FMV player (AVI demuxer, Indeo 5 and
# MS-ADPCM decoders, swscale, file protocol) as Windows DLLs with MinGW-w64.
# Same component set as the Android build (see docs/FMV.md); no GPL or nonfree parts. The desktop
# build also decodes Indeo 5 transparency (desktop/patches/ffmpeg-indeo5-transparency.patch).
#
# usage: build-ffmpeg.sh <i686|x86_64> <ffmpeg-source-dir> <install-prefix>
set -euo pipefail

ARCH="$1"
SRC="$(cd "$2" && pwd)"
mkdir -p "$3"
PREFIX="$(cd "$3" && pwd)"
CROSS="${ARCH}-w64-mingw32-"
CC="${CROSS}gcc"
if command -v "${CROSS}gcc-posix" >/dev/null; then CC="${CROSS}gcc-posix"; fi

# Our FFmpeg changes (desktop/patches/ffmpeg-*.patch, LGPL like FFmpeg) are applied to the source
# first; a patch that is already applied is skipped.
PATCHES="$(cd "$(dirname "$0")/../patches" && pwd)"
for P in "$PATCHES"/ffmpeg-*.patch; do
  [ -e "$P" ] || continue
  if patch -d "$SRC" -p1 -R --dry-run -s -f < "$P" >/dev/null 2>&1; then
    echo "FFmpeg patch already applied: $(basename "$P")"
  else
    patch -d "$SRC" -p1 -N -s < "$P"
    echo "Applied FFmpeg patch: $(basename "$P")"
  fi
done

BUILD="${PREFIX}/.build"
rm -rf "$BUILD"
mkdir -p "$BUILD"
cd "$BUILD"

"$SRC/configure" \
  --prefix="$PREFIX" \
  --target-os=mingw32 --arch="$ARCH" --enable-cross-compile \
  --cross-prefix="$CROSS" --cc="$CC" \
  --extra-ldflags="-static-libgcc -static" \
  --disable-pthreads --enable-w32threads \
  --disable-static --enable-shared \
  --disable-programs --disable-doc --disable-avdevice --disable-postproc \
  --disable-avfilter --disable-network --disable-autodetect \
  --disable-hwaccels --disable-everything \
  --enable-demuxer=avi --enable-decoder=indeo5 --enable-decoder=adpcm_ms \
  --enable-protocol=file --enable-swscale --enable-swresample \
  --disable-gpl --disable-nonfree --disable-encoders --disable-muxers

make -j"$(nproc)"
make install
cp "$SRC/COPYING.LGPLv2.1" "$PREFIX/"   # packaged with the DLLs (build-windows.sh)
rm -rf "$BUILD"
cat "$PATCHES"/ffmpeg-*.patch 2>/dev/null | sha1sum | cut -d' ' -f1 > "$PREFIX/khg-patches.sha1"
