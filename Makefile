FG_FILES = src/Frog.cpp src/FrogHopper.cpp src/Vehicle.cpp
FG_HEADERS = $(wildcard src/header/*.h) $(wildcard src/header/*.hpp)
# The game reports a startup event to trace on a background thread
# (src/header/usageReporting.h), tagged with this version.
VERSION_FLAGS = -DFROG_HOPPER_VERSION='"$(shell cat version.txt)"'

.PHONY: all clean

all: frogHopper_executable

frogHopper_executable: $(FG_FILES) $(FG_HEADERS) version.txt
	g++ -pthread $(VERSION_FLAGS) $(FG_FILES) -lSDL2main -lSDL2 -lSDL2_image -o frogHopper_executable

clean:
	rm -f frogHopper_executable
