# Frog-Hopper-2D
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

## Usage reporting

The game reports to [trace](https://trace.danielstephenson.dev) by default: one `startup` event per launch, carrying the program name (`Frog-Hopper-2D`) and its version from `version.txt`. Nothing about you, your machine, your IP address or the game is sent.

The first run prints one line saying so on stderr and writes a small settings file, `usage-reporting.conf`, to `$XDG_CONFIG_HOME/Frog-Hopper-2D/` (by default `~/.config/Frog-Hopper-2D/`; `~/Library/Application Support/Frog-Hopper-2D/` on macOS, `%APPDATA%\Frog-Hopper-2D\` on Windows). To turn reporting off:

- set `enabled=false` in that file, or
- set `TRACE_USAGE_REPORTING=off` or `DO_NOT_TRACK=1` in the environment (this turns it off for every trace-reporting program, and nothing is printed or written).

The event is sent in the background by the vendored [trace-client-cpp](https://github.com/Stephenson-Software/trace-client-cpp) header (`src/header/trace_client.hpp`) through the system `curl`; if curl is missing or the machine is offline, nothing is sent and the game is unaffected. `FROG_HOPPER_2D_USAGE_REPORTING_ENDPOINT` points reporting at another server, e.g. a local one while testing. Details: https://github.com/Stephenson-Software/trace#usage-reporting

## License

This project is licensed under the Stephenson Software Non-Commercial License (Stephenson-NC). See [LICENSE](LICENSE).
