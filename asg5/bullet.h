#ifndef _BULLET_H_
#define _BULLET_H_

#include <iostream>
#include <string>
#include "multisprite.h"

class Bullet : public MultiSprite{
public:
    Bullet(const std::string& name, const Vector2f& pos, const Vector2f& vel);
    Bullet(const Bullet& );
    Bullet& operator=(const Bullet& );
    virtual ~Bullet(){}
    virtual void update(Uint32 ticks);
    void reset(const Vector2f& pos,const Vector2f& vel);
    int getLife() const;
    void setLife(const int n);
private:
    int lifetime;
};

#endif


