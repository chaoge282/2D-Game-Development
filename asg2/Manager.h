#include <SDL.h>
#include <string>
#include "Sprite.h"
#include "generateFrames.h"
class Manager{
 public:
	Manager(Sprite* s,const unsigned int w,const unsigned int h):sprite(s),WIDTH(w),HEIGHT(h){};
	~Manager(){};
	unsigned int getWidth()  const {return WIDTH;}
	unsigned int getHeight() const  {return HEIGHT;}
	void init();
	SDL_Surface* getImage(const std::string& filename, bool setColorKey);
	void draw(SDL_Surface* image, SDL_Surface* screen, float x=0.0, float y=0.0);
  private:
	Sprite *sprite;
	const unsigned int WIDTH;
	const unsigned int HEIGHT;
	Manager(const Manager&);
	Manager& operator=(const Manager&);
};
