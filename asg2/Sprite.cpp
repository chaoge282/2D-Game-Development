#include <SDL.h>
#include "Sprite.h"

bool Sprite::update(float& y){
	static unsigned int remainder = 0u; // ***
	static unsigned int currentTicks = 0u;
	static unsigned int prevTicks = SDL_GetTicks();
	currentTicks = SDL_GetTicks();
	unsigned int elapsedTicks = currentTicks - prevTicks + remainder; // ***
	if( elapsedTicks < DT ) return false; // ***

	float incr = Y_VELOCITY * DT * 0.001; // ***
	y -= incr;

	prevTicks = currentTicks;
	remainder = elapsedTicks - DT; // ***

	return true; 
}
