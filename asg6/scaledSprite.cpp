#include <iostream>
#include <cmath>
#include "vector2f.h"
#include "SDL/SDL_rotozoom.h"
#include "scaledSprite.h"
#include "gamedata.h"
#include "frameFactory.h"


float ScaledSprite::getRandFloat(float min, float max) {
    return min + (rand() / (RAND_MAX + 1.0f)) * (max - min);
}

ScaledSprite::ScaledSprite(const std::string& name):
    MultiSprite(name,
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
  smooth(1)
{
}

ScaledSprite::ScaledSprite(const ScaledSprite& s) :
    MultiSprite(s.getName(), s.getPosition(), s.getVelocity()),
  scale(s.scale),
    smooth(s.smooth)
{ }

ScaledSprite& ScaledSprite::operator=(const ScaledSprite& rhs) {
  setName( rhs.getName() );
  setPosition(rhs.getPosition());
  setVelocity(rhs.getVelocity());
  scale = rhs.scale;
  smooth= rhs.smooth;
  return *this;
}


unsigned ScaledSprite::getPixel(Uint32 i, Uint32 j) const {
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  x = i - x;
  y = j - y;
  Uint32 *pixels = static_cast<Uint32 *>(frames[currentFrame]->getSurface()->pixels);
  return pixels[ ( y * frames[currentFrame]->getWidth() ) + x ];
}

void ScaledSprite::explode(){
    if(explosion)
        return;
    else {
        Sprite sprite(this->getName(),this->getPosition(),this->getVelocity(),this->getFrame());
           explodingSprite =new ExplodingSprite(sprite);
           explosion = true;
    }
}

void ScaledSprite::draw() const {
    if(explosion){
        explodingSprite->draw();
    }
    else{
        Uint32 x = static_cast<Uint32>(X());
        Uint32 y = static_cast<Uint32>(Y());
        frames[currentFrame]->draw(x, y,0,scale,smooth);
    }
}


void ScaledSprite::update(Uint32 ticks) {
    if(explosion){
        explodingSprite->update(ticks);
    }
    else{
    advanceFrame(ticks);
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.005 *scale;
  setPosition(getPosition() + incr);

  if ( Y() < 0) {
    velocityY( abs( velocityY() ) );
  }
  if ( Y() > worldHeight) {
      setPosition(Vector2f( getRandom(Gamedata::getInstance().getXmlInt(getName()+"/loc/x"),
                              Gamedata::getInstance().getXmlInt(getName()+"/loc/xm")),-frameHeight));
    velocityY( abs( velocityY() ) );
  }

  if ( X() < 0) {
    velocityX( abs( velocityX() ) );
  }
  if ( X() > worldWidth-frameWidth) {
    velocityX( -abs( velocityX() ) );
  }

    }
}
