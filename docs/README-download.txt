Frog Hopper
===========

Get the frog across the lanes of traffic to the pond. Move with the arrow
keys. Touching a car ends the run; reaching the top wins. From either end
screen, release any key to play again; close the window to quit.

Running it
----------

Windows x64: run frogHopper.exe. The SDL2 DLLs it needs are in the same
folder.

Linux x64: run ./frogHopper_executable. It needs SDL2 and SDL2_image, which
on Debian or Ubuntu are installed with:

    sudo apt-get install libsdl2-2.0-0 libsdl2-image-2.0-0

The resources folder must stay next to the executable.

Usage reporting
---------------

The game reports one startup event per launch to trace
(https://trace.danielstephenson.dev): the program name, its version and a
random installation ID. Set TRACE_USAGE_REPORTING=off or DO_NOT_TRACK=1 in
the environment, or enabled=false in the usage-reporting.conf file it
writes, to turn this off. The project's README has the details.

Source and licence: https://github.com/Stephenson-Software/Frog-Hopper-2D
(LICENSE.txt).
