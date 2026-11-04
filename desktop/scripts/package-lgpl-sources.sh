#!/usr/bin/env bash
# Packages the source of the LGPL libraries a KHGWin release ships as DLLs (FFmpeg, OpenAL Soft),
# exactly as built: build/KHGWin-<ver>-lgpl-sources.zip. See desktop/licenses/LGPL-NOTICE.txt.
#
# usage: package-lgpl-sources.sh [ffmpeg-source-dir]   (default build/ffmpeg-src, a clone of FFmpeg n6.1.2)
set -euo pipefail

ROOT="$(cd "$(dirname "$0")/../.." && pwd)"
FFMPEG_SRC="${1:-${FFMPEG_SRC:-$ROOT/build/ffmpeg-src}}"
VERSION="${KHG_VERSION:-$(git -C "$ROOT" describe --tags --always --dirty --abbrev=7 2>/dev/null || echo unknown)}"
OUT="$ROOT/build/lgpl-sources"
ZIP="$ROOT/build/KHGWin-$VERSION-lgpl-sources.zip"

if [ ! -d "$FFMPEG_SRC" ]; then
  git clone --depth 1 -b n6.1.2 https://github.com/FFmpeg/FFmpeg "$FFMPEG_SRC"
fi

rm -rf "$OUT" "$ZIP"
mkdir -p "$OUT/ffmpeg" "$OUT/openal-soft"
# FFmpeg: the tracked files of the checkout (no build output), then our patch applied, as build-ffmpeg.sh does.
git -C "$FFMPEG_SRC" archive --format=tar HEAD | tar -x -C "$OUT/ffmpeg"
for P in "$ROOT"/desktop/patches/ffmpeg-*.patch; do
  patch -d "$OUT/ffmpeg" -p1 --forward --quiet < "$P"
  cp "$P" "$OUT/"
done
cp "$ROOT/desktop/scripts/build-ffmpeg.sh" "$OUT/ffmpeg/"
# OpenAL Soft: the vendored tree the build uses.
cp -r "$ROOT/app/src/main/cpp/thirdparty/openal-soft/." "$OUT/openal-soft/"
cp "$ROOT/desktop/licenses/LGPL-NOTICE.txt" "$OUT/README.txt"
(cd "$OUT" && zip -qr "$ZIP" .)
echo "Packaged $ZIP"
