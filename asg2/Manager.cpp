#include "Manager.h"
#include <string>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>	
	void Manager::init(){
	if (SDL_Init(SDL_INIT_VIDEO)!=0){
		throw (std::string("Unable to initialize SDL:")+SDL_GetError());
	}
	if (TTF_Init() !=0){
		throw std::string("Unable to initialize SDL:")+SDL_GetError();
	}
	atexit(SDL_Quit);
}

	SDL_Surface* Manager::getImage(const std::string& filename, bool setColorKey){
	SDL_Surface* temp = IMG_Load(filename.c_str());
	if(temp == NULL){
		throw std::string("Unable to load bitmap.")+SDL_GetError();
	}
	if(setColorKey){
		Uint32 colorkey = SDL_MapRGB(temp->format,255,255,255);
		SDL_SetColorKey(temp, SDL_SRCCOLORKEY|SDL_RLEACCEL, colorkey);
	}
	SDL_Surface* image = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);
	return image;
}

	void Manager::draw(SDL_Surface *image, SDL_Surface* screen,float x,float y){
	Uint16 w=image->w;
	Uint16 h=image->h;
	Sint16 xCoord = static_cast<Sint16>(x);
	Sint16 yCoord = static_cast<Sint16>(y);
	SDL_Rect src = {0,0,w,h};
	SDL_Rect dest = {xCoord,yCoord,0,0};
	SDL_BlitSurface(image,&src,screen,&dest);
}

