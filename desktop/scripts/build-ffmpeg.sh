#!/usr/bin/env bash
# Cross-builds the minimal LGPL FFmpeg used by the FMV player (AVI demuxer, Indeo 5 and
# MS-ADPCM decoders, swscale, file protocol) as Windows DLLs with MinGW-w64.
# Same component set as the Android build (see docs/FMV.md); no GPL or nonfree parts.
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
