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

From either end screen, pressing any key returns to the game and closing the window exits.

## License

This project is licensed under the Stephenson Software Non-Commercial License (Stephenson-NC). See [LICENSE](LICENSE).
