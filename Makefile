FG_FILES = src/Frog.cpp src/FrogHopper.cpp src/Vehicle.cpp

.PHONY: all clean

all: frogHopper_executable

frogHopper_executable: $(FG_FILES)
	g++ $(FG_FILES) -lSDL2main -lSDL2 -lSDL2_image -o frogHopper_executable

clean:
	rm -f frogHopper_executable
