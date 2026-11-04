#!/usr/bin/env bash
# Turns the module+offset lines of a KHGWin crash report (Crashes\KHGWin-*.txt) into function names and
# source lines, using the unstripped KHGWin.exe of the same build (KHGWin-<ver>-<arch>-debug.zip, or
# build/<arch>/debug/KHGWin.exe).
#
# usage: symbolize-crash.sh <crash report .txt> [debug KHGWin.exe]   (needs MinGW-w64 binutils)
set -euo pipefail

REPORT="$1"
EXE="${2:-$(dirname "$0")/KHGWin.exe}"
case "$(grep -m1 '^Build:' "$REPORT")" in
  *32-bit*) PREFIX=i686-w64-mingw32 ;;
  *)        PREFIX=x86_64-w64-mingw32 ;;
esac
ADDR2LINE="$(command -v $PREFIX-addr2line || command -v addr2line)"
# The image base the offsets are relative to.
BASE=$(( 0x$($PREFIX-objdump -p "$EXE" | awk '/^ImageBase/ {print $2}') ))

tr -d '\r' < "$REPORT" | while IFS= read -r LINE; do
  if [[ "$LINE" =~ KHGWin\.exe\+0x([0-9a-fA-F]+) ]]; then
    ADDR=$(printf '0x%x' $(( BASE + 0x${BASH_REMATCH[1]} )))
    WHERE="$("$ADDR2LINE" -f -C -i -p -e "$EXE" "$ADDR" | tr '\n' ' ')"
    echo "$LINE   ->  $WHERE"
  else
    echo "$LINE"
  fi
done
