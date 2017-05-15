
#ifndef __OBJECTPOOL_H__
#define __OBJECTPOOL_H__

#include <iostream>
#include "bullet.h"
#include <list>
#include "perPixelCollision.h"

class ObjectPool{
public:
    static ObjectPool& getInstance();
    ~ObjectPool(){};
    void draw() const;
    bool collidedWith(const Drawable*) ;
    void acquireBullet(const Vector2f& p,const Vector2f& v);
    void releaseBullet(Uint32 ticks);
    unsigned int bulletCount() const { return bulletList.size(); }
    unsigned int freeCount() const { return freeList.size(); }
private:
    ObjectPool(const std::string& n);
    ObjectPool(const ObjectPool&);
    ObjectPool& operator=(const ObjectPool&);
     std::string name;
    PerPixelCollision perPixelColl;
    std::list<Bullet> bulletList;
    std::list<Bullet> freeList;
};

#endif
