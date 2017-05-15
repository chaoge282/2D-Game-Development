#include "player.h"
#include "gamedata.h"
#include "frameFactory.h"
#include "vector2f.h"

void Player::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
        if(velocityX()>0) currentFrame = (currentFrame+1) % (numberOfFrames/2);
        else  currentFrame= ((currentFrame+1) % (numberOfFrames/2)) +8;
   // currentFrame = (currentFrame+1) % numberOfFrames;
		timeSinceLastFrame = 0;
	}
}

Player::Player( const std::string& name) :
  Drawable(name,
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"),
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")),
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  frames( FrameFactory::getInstance().getFrames(name) ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval") ),
  timeSinceLastFrame( 0 ),
  frameWidth(frames[0]->getWidth()),
  frameHeight(frames[0]->getHeight())
{ }

Player::Player(const Player& s) :
  Drawable(s),
  frames(s.frames),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight ),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  frameWidth( s.frameWidth ),
  frameHeight( s.frameHeight )
  { }

  void Player::draw() const {
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  frames[currentFrame]->draw(x, y);
}

void Player::update(Uint32 ticks) {
  advanceFrame(ticks);
}

void Player::updateMove(Uint32 ticks,int i) {
    if(i){
            float incr = velocityX() * static_cast<float>(ticks) * 0.0001;
            float x=X()+incr;
            setPosition(Vector2f(x,Y()));
    }
    else{
            float incc = velocityY() * static_cast<float>(ticks) * 0.0001;
            float y=Y()+incc;
            setPosition(Vector2f(X(),y));
    }
  if ( Y() < 0) {
    velocityY( abs( velocityY() ) );
    setPosition(Vector2f(X(),0));
  }
  if ( Y() > worldHeight-frameHeight) {
    velocityY( -abs( velocityY() ) );
    setPosition(Vector2f(X(),worldHeight-frameHeight));
  }

  if ( X() < 0) {
    velocityX( -abs( velocityX() ) );
  setPosition(Vector2f(0,Y()));
  }
  if ( X() > worldWidth-frameWidth) {
    velocityX( abs( velocityX() ) );
  setPosition(Vector2f(worldWidth-frameWidth,Y()));
  }

}
