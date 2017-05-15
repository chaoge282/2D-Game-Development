#include <iostream>
#include <cmath>
#include "vector2f.h"
#include "SDL/SDL_rotozoom.h"
#include "scaledSprite.h"
#include "gamedata.h"
#include "frameFactory.h"

int getRandom(int a, int b) {
  return a + (rand() % b + 1);
}

float getRandFloat(float min, float max) {
    return min + (rand() / (RAND_MAX + 1.0f)) * (max - min);
}

ScaledSprite::ScaledSprite(const std::string& name,
                            SDL_Surface* surface):
  Drawable(name,
          Vector2f( getRandom(Gamedata::getInstance().getXmlInt(name+"/loc/x"),
                              Gamedata::getInstance().getXmlInt(name+"/loc/xm")),
              Gamedata::getInstance().getXmlInt(name+"/loc/y")),
           Vector2f(getRandom(
                    Gamedata::getInstance().getXmlInt(name+"/cushion"),
                    Gamedata::getInstance().getXmlInt(name+"/speed/x")),
                     getRandom(
                    Gamedata::getInstance().getXmlInt(name+"/cushion"),
                    Gamedata::getInstance().getXmlInt(name+"/speed/y"))
                   )
  ),
  scale(getRandFloat(Gamedata::getInstance().getXmlFloat(name+"/scale/min"),
                     Gamedata::getInstance().getXmlFloat(name+"/scale/max"))
  ),
  scaledSurface( rotozoomSurface(surface, 30, scale, SMOOTHING_ON) ),
  frame(new Frame(scaledSurface, scaledSurface->w, scaledSurface->h)),
  frameWidth(frame->getWidth()),
  frameHeight(frame->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height"))
{ }

ScaledSprite::ScaledSprite(const ScaledSprite& s) :
  Drawable(s.getName(), s.getPosition(), s.getVelocity()),
  scale(s.scale),
  scaledSurface(s.scaledSurface),
  frame(s.frame),
  frameWidth(s.frameWidth),
  frameHeight(s.frameHeight),
  worldWidth(s.worldWidth),
  worldHeight(s.worldHeight)
{ }

ScaledSprite& ScaledSprite::operator=(const ScaledSprite& rhs) {
  setName( rhs.getName() );
  setPosition(rhs.getPosition());
  setVelocity(rhs.getVelocity());
  scale = rhs.scale;
  scaledSurface = rhs.scaledSurface;
  frame = rhs.frame;
  frameWidth = rhs.frameWidth;
  frameHeight = rhs.frameHeight;
  worldWidth = rhs.worldWidth;
  worldHeight = rhs.worldHeight;
  return *this;
}

ScaledSprite::~ScaledSprite() {
    SDL_FreeSurface(scaledSurface);
    delete frame;
}

void ScaledSprite::draw() const {
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  frame->draw(x, y);
}

unsigned ScaledSprite::getPixel(Uint32 i, Uint32 j) const {
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  x = i - x;
  y = j - y;
  Uint32 *pixels = static_cast<Uint32 *>(frame->getSurface()->pixels);
  return pixels[ ( y * frame->getWidth() ) + x ];
}


void ScaledSprite::update(Uint32 ticks) {
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.002 *scale;
  setPosition(getPosition() + incr);

  if ( Y() < 0) {
    velocityY( abs( velocityY() ) );
  }
  if ( Y() > worldHeight) {
      Vector2f p(X(),-frameHeight);
      setPosition(p);
    velocityY( abs( velocityY() ) );
  }

  if ( X() < 0) {
    velocityX( abs( velocityX() ) );
  }
  if ( X() > worldWidth-frameWidth) {
    velocityX( -abs( velocityX() ) );
  }
}
void ScaledSprite::updateMove(Uint32 ticks,int i) {
       Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
       setPosition(getPosition()+incr);
       i++;
  if ( Y() < 0) {
    velocityY( abs( velocityY() ) );
  }
  if ( Y() > worldHeight-frameHeight) {
    velocityY( -abs( velocityY() ) );
  }

  if ( X() < 0) {
    velocityX( -abs( velocityX() ) );
  }
  if ( X() > worldWidth-frameWidth) {
    velocityX( abs( velocityX() ) );
  }

}
