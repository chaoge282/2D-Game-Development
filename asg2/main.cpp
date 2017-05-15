// lichao xu

#include <SDL.h>
#include <iostream>
#include <string>
#include "Manager.h"
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

static bool makeVideo = false;

int main() {
try{
	Sprite spr;
	Manager manager(&spr,854u,480u);
    manager.init();
   
	SDL_Surface *screen = SDL_SetVideoMode(manager.getWidth(),manager.getHeight(), 0, SDL_DOUBLEBUF);
    if (screen == NULL) {
      throw std::string("Unable to set video mode: ")+SDL_GetError();
    }
	
	TTF_Font *font = TTF_OpenFont("msyh.ttf",50);
	SDL_Color textColor = {0,0,0,255};
	TTF_Font *font2 = TTF_OpenFont("msyh.ttf",70);
	SDL_Color textColor2 = {255,0,0,255};
	
    SDL_Surface *sky = manager.getImage("images/sky.png", false);
    SDL_Surface *balloon = manager.getImage("images/ball2.png", true);
    SDL_Surface *angel = manager.getImage("images/angel.png", true);
	SDL_Surface *message =	TTF_RenderText_Solid(font, "HELLO!! C++!!",textColor);
	SDL_Surface *message2 =	TTF_RenderText_Solid(font, "GOODBYE!! C++!!",textColor2);

	float  x = (manager.getWidth()/2)-(balloon->w)/2;
	float  y = sky->h;
    
	SDL_Event event;
    bool done = false;
    bool freshFrame = false; // ***
    GenerateFrames genFrames(screen);
    while ( !done) {
      while ( SDL_PollEvent(&event) ) {
        if (event.type == SDL_QUIT) done = true;
        if (event.type == SDL_KEYDOWN) {
          if (event.key.keysym.sym == SDLK_ESCAPE) done = true;
          if (event.key.keysym.sym == SDLK_F4) makeVideo =true;
		}
      }
     
	  if ( y> -balloon->h ) {
        freshFrame = spr.update(y);
      manager.draw(sky, screen,0.0,0.0);
      manager.draw(balloon, screen, x, y);
	  manager.draw(message, screen,0.0,(y+balloon->h/2));
      SDL_Flip(screen);
      }
      else if( y> (-balloon->h)*2) {
		freshFrame = spr.update(y);
		std::cout<<y<<std::endl;
		manager.draw(angel,screen,manager.getWidth()/2-(angel->w)/2,50.0);
	    manager.draw(message2, screen,manager.getWidth()/2-200,angel->h+50.0);	
		SDL_Flip(screen);
		std::cout<<"draw"<<std::endl;
		}
	  else{
        makeVideo = false;
		done = true;
      }

      if(freshFrame){
        freshFrame=false;
        if ( makeVideo ) {
          genFrames.makeFrame();
        }
      }
		std::cout<<"flip"<<std::endl;
    }
    SDL_FreeSurface(sky);
    SDL_FreeSurface(balloon);
	SDL_FreeSurface(angel);
	SDL_FreeSurface(message);
	
	TTF_CloseFont(font);
	TTF_CloseFont(font2);
	TTF_Quit();
	SDL_Quit();
  }
  catch(const std::string& msg) { std::cout << msg << std::endl;  }
  catch(...) { std::cout << "oops" << std::endl;  }
}
        

