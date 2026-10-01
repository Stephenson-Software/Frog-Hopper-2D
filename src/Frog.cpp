#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <string>

#include "header/Frog.h"

Frog::Frog() {
	xpos = 0;
	ypos = 0;
	width = 0;
	height = 0;
	xvel = 0;
	yvel = 0;
	speed = 0;
	collider = {0, 0, 0, 0};
}

void Frog::render(SDL_Renderer* gRenderer, SDL_Texture* frogTexture) {
	SDL_Rect renderQuad = {xpos, ypos, width, height};
	SDL_RenderCopy(gRenderer, frogTexture, NULL, &renderQuad);
}

void Frog::init(int x, int y, int w, int h, int s) {
	xpos = x;
	ypos = y;
	width = w;
	height = h;
	speed = s;
	collider = {x, y, w, h};
}

// sets the velocity from the arrow keys held right now, rather than adding on each press
// and subtracting on each release, so a release whose press never reached here (e.g. a key
// held across an end screen) cannot leave the frog drifting
void Frog::handleEvent(SDL_Event &e) {
	if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP) {
		const Uint8* keys = SDL_GetKeyboardState(NULL);
		xvel = (keys[SDL_SCANCODE_RIGHT] - keys[SDL_SCANCODE_LEFT]) * speed;
		yvel = (keys[SDL_SCANCODE_DOWN] - keys[SDL_SCANCODE_UP]) * speed;
	}
}

void Frog::move(const int SCREEN_WIDTH, const int SCREEN_HEIGHT) {
	xpos += xvel;
	collider.x = xpos;
	
	// if too far left or right
	if ((xpos < 0) || (xpos + width > SCREEN_WIDTH)) {
		// move back
		xpos -= xvel;
		collider.x = xpos;
	}
	
	ypos += yvel;
	collider.y = ypos;
	
	// if too far up or down
	if ((ypos < -100) || (ypos + height > SCREEN_HEIGHT)) {
		// move back
		ypos -= yvel;
		collider.y = ypos;
	}
}