#include "multisprite.h"
void MultiSprite::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame+1) % numberOfFrames;
		timeSinceLastFrame = 0;
	}
}

int MultiSprite::getRandom(int a, int b) {
  return a + (rand() % b + 1);
}

MultiSprite::~MultiSprite(){
    delete explodingSprite;
}


MultiSprite::MultiSprite( const std::string& name) :
  Drawable(name,
          Vector2f( getRandom(Gamedata::getInstance().getXmlInt(name+"/loc/x"),
                              Gamedata::getInstance().getXmlInt(name+"/loc/xm")),
                    getRandom(Gamedata::getInstance().getXmlInt(name+"/loc/y"),
                              Gamedata::getInstance().getXmlInt(name+"/loc/ym"))
                ),
           Vector2f(getRandom(
                            Gamedata::getInstance().getXmlInt(name+"/speed/x"),
                            Gamedata::getInstance().getXmlInt(name+"/speed/xm")),
                    getRandom(
                            Gamedata::getInstance().getXmlInt(name+"/speed/y"),
                            Gamedata::getInstance().getXmlInt(name+"/speed/ym"))
                   )
           ),
  frames( FrameFactory::getInstance().getFrames(name) ),
    explodingSprite(NULL),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),

  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval") ),
  timeSinceLastFrame( 0 ),
  frameWidth(frames[0]->getWidth()),
  frameHeight(frames[0]->getHeight()),
  explosion(false)
{ }


MultiSprite::MultiSprite(const std::string& name, const Vector2f& pos,const Vector2f& vel) :
    Drawable(name,pos, vel),
    frames( FrameFactory::getInstance().getFrames(name)),
    explodingSprite(NULL),
    worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
    worldHeight(Gamedata::getInstance().getXmlInt("world/height")),

    currentFrame(0),
    numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
    frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval") ),
    timeSinceLastFrame( 0 ),
    frameWidth(frames[0]->getWidth()),
    frameHeight(frames[0]->getHeight()),
    explosion(false)
{ }

MultiSprite::MultiSprite(const MultiSprite& s) :
    Drawable(s),
    frames(s.frames),
    explodingSprite(NULL),
    worldWidth( s.worldWidth ),
    worldHeight( s.worldHeight ),
    currentFrame(s.currentFrame),
    numberOfFrames( s.numberOfFrames ),
    frameInterval( s.frameInterval ),
    timeSinceLastFrame( s.timeSinceLastFrame ),
    frameWidth( s.frameWidth ),
    frameHeight( s.frameHeight ),
    explosion(s.explosion)
  { }

void MultiSprite::explode(){
    if(explosion)
        return;
    else{
        Sprite sprite(this->getName(),this->getPosition(),this->getVelocity(),this->getFrame());
        explodingSprite = new ExplodingSprite(sprite);
        explosion =true;
    }
}

void MultiSprite::draw() const {
    if(explosion)
        explodingSprite->draw();
    else{
        Uint32 x = static_cast<Uint32>(X());
        Uint32 y = static_cast<Uint32>(Y());
        frames[currentFrame]->draw(x, y);
    }
}


void MultiSprite::update(Uint32 ticks) {
    if(explosion){
        explodingSprite->update(ticks);
        if(explodingSprite->chunkCount() == 0){
            explosion =false;
        }
    }
    else{
        advanceFrame(ticks);

        Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
        setPosition(getPosition() + incr);

        if ( Y() < 0) {
            velocityY( abs( velocityY() ) );
        }
        if ( Y() > worldHeight-frameHeight) {
            velocityY( -abs( velocityY() ) );
        }

        if ( X() < 0) {
            velocityX( abs( velocityX() ) );
        }
        if ( X() > worldWidth-frameWidth) {
            velocityX( -abs( velocityX() ) );
        }

    }
}


void MultiSprite::reset()
{
    this->setPosition( Vector2f( getRandom(Gamedata::getInstance().getXmlInt(getName()+"/loc/x"),
                              Gamedata::getInstance().getXmlInt(getName()+"/loc/xm")),
                            Gamedata::getInstance().getXmlInt(getName()+"/loc/y")
                                ));
    this->setVelocity( Vector2f(getRandom(
                            Gamedata::getInstance().getXmlInt(getName()+"/speed/x"),
                            Gamedata::getInstance().getXmlInt(getName()+"/speed/xm")),
                                getRandom(
                            Gamedata::getInstance().getXmlInt(getName()+"/speed/y"),
                            Gamedata::getInstance().getXmlInt(getName()+"/speed/ym"))
                   ));
}
