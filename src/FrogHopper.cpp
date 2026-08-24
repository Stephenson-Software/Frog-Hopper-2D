#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <string>
#include "header/FrogHopper.h"
#include "header/Frog.h"
#include "header/Vehicle.h"

FrogHopper::FrogHopper() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "SDL could not initialize: " << SDL_GetError() << std::endl;
		return;
	}

	gWindow = SDL_CreateWindow("Frog Hopper", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (gWindow == NULL) {
		std::cout << "Window could not be created: " << SDL_GetError() << std::endl;
		return;
	}

	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (gRenderer == NULL) {
		std::cout << "Renderer could not be created: " << SDL_GetError() << std::endl;
		return;
	}

	// resolve the assets against the directory holding the executable, so the game can be
	// launched from any working directory
	char* basePath = SDL_GetBasePath();
	if (basePath != NULL) {
		assetPath = std::string(basePath) + "resources/";
		SDL_free(basePath);
	}

	// initialize objects
	frog.init(frogX, frogY, frogW, frogH);	
	bottomCarRight.init(-200, 550, 200, 100, 6, -200);
	topCarRight.init(500, 200, 200, 100, 7, -200);
	bottomCarLeft.init(400, 450, 200, 100, -8, SCREEN_WIDTH);
	topCarLeft.init(200, 100, 200, 100, -9, SCREEN_WIDTH);
	
	//  initialize PNG loading
	int imgFlags = IMG_INIT_PNG;
	if ((IMG_Init(imgFlags) & imgFlags) != imgFlags) {
		std::cout << "SDL_image could not initialize: " << IMG_GetError() << std::endl;
		return;
	}

	initialized = true;
}

// loads one asset into a texture, keying out cyan where the asset needs transparency
SDL_Texture* FrogHopper::loadTexture(std::string fileName, bool useColorKey) {
	std::string path = assetPath + fileName;

	SDL_Surface* temp_surface = IMG_Load(path.c_str());
	if (temp_surface == NULL) {
		std::cout << "Could not load " << path << ": " << IMG_GetError() << std::endl;
		return NULL;
	}

	if (useColorKey) {
		SDL_SetColorKey(temp_surface, SDL_TRUE, SDL_MapRGB(temp_surface->format, 0, 0xFF, 0xFF));
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(gRenderer, temp_surface);
	if (texture == NULL) {
		std::cout << "Could not create a texture from " << path << ": " << SDL_GetError() << std::endl;
	}

	SDL_FreeSurface(temp_surface);
	return texture;
}

// returns false if any asset failed to load
bool FrogHopper::loadMedia() {
	background = loadTexture("background.png", false);
	frogTexture = loadTexture("frog.png", true);
	carRightTexture = loadTexture("carRight.png", true);
	carLeftTexture = loadTexture("carLeft.png", true);
	loseTexture = loadTexture("playerLose.png", true);
	winTexture = loadTexture("playerWin.png", true);

	return background != NULL && frogTexture != NULL && carRightTexture != NULL &&
		carLeftTexture != NULL && loseTexture != NULL && winTexture != NULL;
}

void FrogHopper::cleanUp() {
	SDL_DestroyTexture(background);
	SDL_DestroyTexture(frogTexture);
	SDL_DestroyTexture(carRightTexture);
	SDL_DestroyTexture(carLeftTexture);
	SDL_DestroyTexture(loseTexture);
	SDL_DestroyTexture(winTexture);
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	IMG_Quit();
	SDL_Quit();
}

// box collision detector
bool FrogHopper::checkCollision(SDL_Rect a, SDL_Rect b) {
	int leftA = a.x;
	int rightA = a.x + a.w;
	int topA = a.y;
	int bottomA = a.y + a.h;
	int leftB = b.x;
	int rightB = b.x + b.w;
	int topB = b.y;
	int bottomB = b.y + b.h;
	
	// check if boxes are not touching
	if (leftA >= rightB) {
		return false;
	}
	if (rightA <= leftB) {
		return false;
	}
	if (topA >= bottomB) {
		return false;
	}
	if (bottomA <= topB) {
		return false;
	}
	
	// otherwise
	return true;
}

void FrogHopper::renderScene() {	
	SDL_RenderCopy(gRenderer, background, NULL, NULL);
	frog.render(gRenderer, frogTexture);
	bottomCarRight.render(gRenderer, carRightTexture, carLeftTexture, "right");
	topCarRight.render(gRenderer, carRightTexture, carLeftTexture, "right");
	bottomCarLeft.render(gRenderer, carRightTexture, carLeftTexture, "left");
	topCarLeft.render(gRenderer, carRightTexture, carLeftTexture, "left");
}

bool FrogHopper::checkWin() {
	if (frog.ypos < -75) {
		winScreen();
		return true;
	}
	return false;
}

void FrogHopper::gameScreen() {
	SDL_Event e;
	bool running = true;
	while (running) {
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				running = false;
			}
			frog.handleEvent(e);
		}
		SDL_RenderClear(gRenderer);
		renderScene();
		frog.move(SCREEN_WIDTH, SCREEN_HEIGHT);

	// if collided with a car
	if (checkCollision(frog.collider, bottomCarLeft.collider) ||
		checkCollision(frog.collider, topCarLeft.collider) ||
		checkCollision(frog.collider, bottomCarRight.collider) ||
		checkCollision(frog.collider, topCarRight.collider)) {
			frog.xvel = 0;
			frog.yvel = 0;
			loseScreen();
		}

		bottomCarRight.move(SCREEN_WIDTH);
		topCarRight.move(SCREEN_WIDTH);
		bottomCarLeft.move(SCREEN_WIDTH);
		topCarLeft.move(SCREEN_WIDTH);
		
		checkWin();
		SDL_RenderPresent(gRenderer);
	}
}

void FrogHopper::loseScreen() {
	frog.ypos = 675;
	bool showing = true;
	SDL_Event e;
	while (showing) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				cleanUp();
			}
			if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
				cleanUp();
			}
		}
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(gRenderer);
		SDL_RenderCopy(gRenderer, loseTexture, NULL, NULL);
		SDL_RenderPresent(gRenderer);
	}
}

void FrogHopper::winScreen() {
	frog.ypos = 675;
	bool showing = true;
	SDL_Event ev;
	while (showing) {
		while (SDL_PollEvent(&ev)) {
			if (ev.type == SDL_QUIT) {
				cleanUp();
			}
			if (ev.type == SDL_KEYDOWN && ev.key.repeat == 0) {
				cleanUp();
			}
		}
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(gRenderer);
		SDL_RenderCopy(gRenderer, winTexture, NULL, NULL);
		SDL_RenderPresent(gRenderer);
	}
}

int main(int argc, char* args[]) {
	FrogHopper frogHopper;
	if (!frogHopper.initialized) {
		std::cout << "Frog Hopper could not start up." << std::endl;
		frogHopper.cleanUp();
		return 1;
	}

	if (!frogHopper.loadMedia()) {
		std::cout << "Frog Hopper could not load its assets." << std::endl;
		frogHopper.cleanUp();
		return 1;
	}

	frogHopper.gameScreen();
	frogHopper.cleanUp();
	return 0;
}