#!/usr/bin/env bash
# Builds the browser version into web/build/ (index.html, index.js, index.wasm,
# index.data). Needs Emscripten's em++ on PATH (source emsdk_env.sh first).
# The native build (make) is unaffected.
set -euo pipefail
cd "$(dirname "$0")/.."

out=web/build
rm -rf "$out"
mkdir -p "$out"

# -sASYNCIFY lets the game's own while loops run unchanged: each frame waits for
# the browser's next animation frame (see waitForNextFrame in src/FrogHopper.cpp).
# No pthreads, so the page needs no cross-origin isolation.
em++ -O2 -std=c++17 \
  -DFROG_HOPPER_VERSION="\"$(tr -d '[:space:]' < version.txt)\"" \
  src/Frog.cpp src/FrogHopper.cpp src/Vehicle.cpp \
  -sUSE_SDL=2 -sUSE_SDL_IMAGE=2 -sSDL2_IMAGE_FORMATS='["png"]' \
  -sASYNCIFY -sALLOW_MEMORY_GROWTH \
  --preload-file resources@/resources \
  --shell-file web/shell.html \
  -o "$out/index.html"

echo "Built $out:"
ls -l "$out"
