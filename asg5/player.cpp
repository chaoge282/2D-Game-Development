#include "player.h"
#include "gamedata.h"
#include "frameFactory.h"
#include "vector2f.h"

void Player::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
        if(velocityX()>0) currentFrame = (currentFrame+1) % (numberOfFrames/2);
        else  currentFrame= ((currentFrame+1) % (numberOfFrames/2)) +numberOfFrames/2;
		timeSinceLastFrame = 0;
	}
}

Player::Player( const std::string& name) :
  MultiSprite(name,
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"),
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")),
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
    perPixelColl( PerPixelCollision()),
    objectPool(ObjectPool::getInstance()),
  isJumping(false),
  running(false),
    reDisp(true),
    gameover(false),
    resurval(true),
    deathCount(0)
{ }

Player::Player(const Player& s) :
  MultiSprite(s),
  perPixelColl(s.perPixelColl),
  objectPool(s.objectPool),
    isJumping(s.isJumping),
    running(s.running),
    reDisp(s.reDisp),
    gameover(s.gameover),
    resurval(s.resurval),
    deathCount(s.deathCount)
{ }

bool Player::beatSprite(const Drawable* d) const {
    return objectPool.collidedWith(d);
}

bool Player:: collidedWith(const Drawable* d) const{
      return perPixelColl.execute(*this,*d);
  }

void Player::shoot(){
    resurval =false;
    if(!explosion){
        if(velocityX()>0){
            Vector2f p(X()+frames[currentFrame]->getWidth()*0.8,Y()), v(velocityX(),0);
            objectPool.acquireBullet(p,v);
        }
        else{
            Vector2f p(X()-frames[currentFrame]->getWidth(),Y()), v(velocityX(),0);
            objectPool.acquireBullet(p,v);
        }
    }
}

void Player::explode(){
    if(explosion)
        return;
    else {
        Sprite sprite(this->getName(),this->getPosition(),this->getVelocity(),this->getFrame());
           explodingSprite =new ExplodingSprite(sprite);
           explosion = true;
           deathCount++;
           if(deathCount >2)
               setOver(true);
    }
}


void Player::draw() const {
    if(!gameover){
  objectPool.draw();
    if(explosion) {
        explodingSprite->draw();
    }
    if(reDisp){
        Uint32 x = static_cast<Uint32>(X());
        Uint32 y = static_cast<Uint32>(Y());
        frames[currentFrame]->draw(x, y);
    }
    }
}

void Player::right() {
    resurval = false;
    if ( X() < worldWidth-frameWidth) {
        velocityX(abs(velocityX()));
        running =true;
  }
}
void Player::left()  {
    resurval =false;
    if (X() > 0) {
        velocityX(-abs(velocityX()));
        running =true;
  }
}
void Player::jump(){
    resurval =false;
    if(!isJumping)
        isJumping=true;
}


void Player::update(Uint32 ticks) {
  if(!gameover){
    objectPool.releaseBullet(ticks);
    if(explosion) {
        explodingSprite->update(ticks);
        if(explodingSprite->chunkCount() ==0){
            explosion =false;
            setAppear(true);
            setResurval(true);

        }
    }
    if(reDisp){
        advanceFrame(ticks);
        if(running){
        float incr = velocityX() * static_cast<float>(ticks) * 0.0008;
        float x=X()+incr;
        setPosition(Vector2f(x,Y()));
        running =false;
        }
        if(isJumping){
            float a =200;
            float incc = velocityY() * static_cast<float>(ticks) * 0.002;
            Y(Y()+incc);
            velocityY(velocityY()+a*static_cast<float>(ticks)*0.002);
            setPosition(Vector2f(X(),Y()));
            }
        if ( Y() < 0) {
            velocityY( abs( velocityY() ) );
         }
        if ( Y() >= Gamedata::getInstance().getXmlInt("player/startLoc/y")){
            velocityY( Gamedata::getInstance().getXmlInt("player/speedY"));
            isJumping=false;
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

  }
}

void Player::resetPlayer(){

    Vector2f pos(Gamedata::getInstance().getXmlInt(getName()+"/startLoc/x"),
                Gamedata::getInstance().getXmlInt(getName()+"/startLoc/y"));
    Vector2f vel(Gamedata::getInstance().getXmlInt(getName()+"/speedX"),
            Gamedata::getInstance().getXmlInt(getName()+"/speedY"));
    setPosition(pos);
    setVelocity(vel);

    setResurval(true);
    setOver(false);
    deathCount =0;

}
