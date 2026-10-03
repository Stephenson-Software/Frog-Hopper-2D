# Frog-Hopper-2D

[![Play in your browser](https://img.shields.io/badge/Play-in%20your%20browser-2ea44f)](https://danielstephenson.dev/play/frog-hopper-2d)

2D game made to practice collision detection and movement. Get the frog across the lanes of traffic to the Pond.

## Requirements

- a C++ compiler (`g++`)
- `make`
- the SDL2 and SDL2_image development packages, which on Debian/Ubuntu are:

```
sudo apt-get install libsdl2-dev libsdl2-image-dev
```

A VS Code dev container that installs these is provided under `.devcontainer/`.

## Building

```
make
```

This writes an executable named `frogHopper_executable` to the repository root. `make clean` removes it.

## Running

```
./frogHopper_executable
```

The game resolves its assets against the directory holding the executable, so `resources/` must sit next to `frogHopper_executable`. The default build satisfies this because both end up in the repository root. The working directory the game is launched from does not matter.

A window titled "Frog Hopper" opens at 1000x750.

## How to play

The frog is controlled with the four arrow keys. Movement continues while a key is held and stops when it is released.

Four cars cross the screen, two travelling right and two travelling left. Each car reappears on the far side once it has left the screen.

- Touching a car ends the run and shows the lose screen.
- Moving the frog off the top of the screen shows the win screen.

From either end screen, pressing and releasing any key returns to the game, and closing the window exits.

## Play in your browser

The game also runs in a web browser, built with [Emscripten](https://emscripten.org):

- https://frog-hopper.play.danielstephenson.dev
- more games: https://danielstephenson.dev/play

On a keyboard the arrow keys work as in the desktop game. On a phone or tablet, four on-screen arrow buttons appear under the game; hold one to keep moving, or tap it for a short hop. They also leave the win and lose screens. The browser version does not send usage reports.

To build it, install and activate the [Emscripten SDK](https://emscripten.org/docs/getting_started/downloads.html), then run:

```
web/build.sh
```

This writes `index.html`, `index.js`, `index.wasm`, `index.data` (the preloaded `resources/`) and `arcade-scores.js` (see below) to `web/build/`. Serve that directory over HTTP to play it locally, e.g. `python3 -m http.server --directory web/build 8000` and open http://localhost:8000. The page itself is `web/shell.html`. The `Browser build` workflow builds it on every pull request and deploys it to the address above.

### High scores

In the browser version, a signed-in player's crossing times go to the "Fastest crossing" leaderboard (`fastest-crossing`, lower is better) of [arcade-social](https://github.com/Stephenson-Software/arcade-social) at `https://api.play.danielstephenson.dev`. A crossing is timed in game time: the frames from the frog's first move of the attempt until it reaches the pond, at 60 frames a second, in seconds to two decimals (holding up from the start, about 3.13 s, is the fastest possible). Every crossing is sent and the service keeps each player's best. Two achievements are unlocked too: `first-win` (the first crossing) and `under-five` (a crossing under five seconds).

Sign-in happens on arcade-social's own page; the game never sees a password, and nothing is sent while signed out, from any address other than https://frog-hopper.play.danielstephenson.dev, or from the desktop game (the calls are compiled only under `__EMSCRIPTEN__`). Scores are reported by the player's browser and can be forged, so the board is labelled "not verified". The client is `web/arcade-scores.js`, vendored unchanged from arcade-social's `clients/js`; `web/build.sh` copies it next to the page, `web/shell.html` loads it, and `src/FrogHopper.cpp` calls it through `EM_JS`. Its calls never throw: a refused or lost score is dropped and the game carries on.

## Usage reporting

The game reports to [trace](https://trace.danielstephenson.dev) by default: one `startup` event per launch, carrying the program name (`Frog-Hopper-2D`) and its version from `version.txt`. Nothing about you, your machine, your IP address or the game is sent.

The first run prints one line saying so on stderr and writes a small settings file, `usage-reporting.conf`, to `$XDG_CONFIG_HOME/Frog-Hopper-2D/` (by default `~/.config/Frog-Hopper-2D/`; `~/Library/Application Support/Frog-Hopper-2D/` on macOS, `%APPDATA%\Frog-Hopper-2D\` on Windows). To turn reporting off:

- set `enabled=false` in that file, or
- set `TRACE_USAGE_REPORTING=off` or `DO_NOT_TRACK=1` in the environment (this turns it off for every trace-reporting program, and nothing is printed or written).

The event is sent in the background by the vendored [trace-client-cpp](https://github.com/Stephenson-Software/trace-client-cpp) header (`src/header/trace_client.hpp`) through the system `curl`; if curl is missing or the machine is offline, nothing is sent and the game is unaffected. `FROG_HOPPER_2D_USAGE_REPORTING_ENDPOINT` points reporting at another server, e.g. a local one while testing. Details: https://github.com/Stephenson-Software/trace#usage-reporting

## License

This project is licensed under the Stephenson Software Non-Commercial License (Stephenson-NC). See [LICENSE](LICENSE).
