#include "bullet.h"
#include "gamedata.h"
#include "frameFactory.h"

Bullet::Bullet(const std::string& name,const Vector2f& pos,const Vector2f& vel):
    MultiSprite(name,pos,vel),
    lifetime(Gamedata::getInstance().getXmlInt(name+"/lifetime"))
    {}

Bullet::Bullet(const Bullet& b):
    MultiSprite(b.getName(),b.getPosition(),b.getVelocity()),
    lifetime(b.lifetime)
{}

Bullet& Bullet::operator=(const Bullet& rhs){
    setName(rhs.getName());
    setPosition(rhs.getPosition());
    setVelocity(rhs.getVelocity());
    return *this;
}

void Bullet::reset(const Vector2f& pos,const Vector2f& vel){
    this->setPosition(pos);
    this->setVelocity(vel);
    this->setLife(0);
}

void Bullet::update(Uint32 ticks) {
    advanceFrame(ticks);
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.002;
  setPosition(getPosition() + incr);
  lifetime = lifetime+ticks;
}

int Bullet::getLife()const {return lifetime;}
void Bullet::setLife(const int n){lifetime = n;}

