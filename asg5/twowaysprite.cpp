#include "twowaysprite.h"

void twoWaySp::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
        if(velocityX()>0) currentFrame = (currentFrame+1) % (numberOfFrames/2);
        else  currentFrame= ((currentFrame+1) % (numberOfFrames/2)) +numberOfFrames/2;
		timeSinceLastFrame = 0;
	}
}

twoWaySp::twoWaySp( const std::string& name) :
    MultiSprite(name)
{ }

twoWaySp::twoWaySp(const twoWaySp& s) :
    MultiSprite(s)
  { }


void twoWaySp::explode(){
    if(explosion)
        return;
    else{
        Sprite sprite(this->getName(),this->getPosition(),this->getVelocity(),this->getFrame());
        explodingSprite = new ExplodingSprite(sprite);
        explosion =true;
    }
}

void twoWaySp::resetVP()
{
    this->setPosition( Vector2f( getRandom(Gamedata::getInstance().getXmlInt(getName()+"/loc/x"),
                              Gamedata::getInstance().getXmlInt(getName()+"/loc/xm")),
                                getRandom(Gamedata::getInstance().getXmlInt(getName()+"/loc/y"),
                            Gamedata::getInstance().getXmlInt(getName()+"/loc/ym"))
                                ));
    this->setVelocity( Vector2f(getRandom(
                            Gamedata::getInstance().getXmlInt(getName()+"/speed/x"),
                            Gamedata::getInstance().getXmlInt(getName()+"/speed/xm")),
                                getRandom(
                            Gamedata::getInstance().getXmlInt(getName()+"/speed/y"),
                            Gamedata::getInstance().getXmlInt(getName()+"/speed/ym"))
                   ));
}
