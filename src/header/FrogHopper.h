#ifndef FrogHopper_h
#define FrogHopper_h

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <string>
#include "Frog.h"
#include "Vehicle.h"

class FrogHopper {
    public:
        FrogHopper();
        SDL_Texture* loadTexture(std::string fileName, bool useColorKey);
        bool loadMedia();
        void cleanUp();
        bool checkCollision(SDL_Rect a, SDL_Rect b);
        void renderScene();
        void gameScreen();
        bool endScreen(SDL_Texture* texture);
        bool checkWin();

        SDL_Window* gWindow = NULL;
        SDL_Renderer* gRenderer = NULL;

        // false until the window, the renderer and PNG loading are all ready
        bool initialized = false;

        // directory the assets are read from, resolved from the location of the executable
        std::string assetPath = "resources/";

        // textures
        SDL_Texture* background = NULL;
        SDL_Texture* frogTexture = NULL;
        SDL_Texture* carRightTexture = NULL;
        SDL_Texture* carLeftTexture = NULL;
        SDL_Texture* loseTexture = NULL;
        SDL_Texture* winTexture = NULL;

        // objects
        Frog frog;
        Vehicle bottomCarRight;
        Vehicle topCarRight;
        Vehicle bottomCarLeft;
        Vehicle topCarLeft;

        int frogX = 475;
        int frogY = 675;
        int frogW = 75;
        int frogH = 75;
        int frogSpeed = 4;

        const int SCREEN_WIDTH = 1000;
        const int SCREEN_HEIGHT = 750;

        // lane geometry. every car is the same size, and CAR_WIDTH must not exceed
        // Vehicle::OFFSCREEN_MARGIN. cars travelling right respawn off the left edge and
        // cars travelling left respawn off the right edge
        const int CAR_WIDTH = 200;
        const int CAR_HEIGHT = 100;
        const int CAR_RIGHT_RESPAWN_X = -Vehicle::OFFSCREEN_MARGIN;
        const int CAR_LEFT_RESPAWN_X = SCREEN_WIDTH;

        // pixels per frame for each lane, negative moving left
        const int BOTTOM_CAR_RIGHT_SPEED = 6;
        const int TOP_CAR_RIGHT_SPEED = 7;
        const int BOTTOM_CAR_LEFT_SPEED = -8;
        const int TOP_CAR_LEFT_SPEED = -9;
};

#endif