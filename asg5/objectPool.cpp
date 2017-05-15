#include "objectPool.h"


ObjectPool& ObjectPool::getInstance(){
    static ObjectPool objectPool("bullet");
    return objectPool;
}

ObjectPool::ObjectPool(const std::string& n):
                        name(n),
                perPixelColl( PerPixelCollision()),
                        bulletList(),
                        freeList()
{
}
void ObjectPool::draw() const {
    std::list<Bullet>::const_iterator ptr = bulletList.begin();
    while(ptr != bulletList.end()) {
        ptr->draw();
        ++ptr;
    }
}


void ObjectPool:: acquireBullet(const  Vector2f& p,const Vector2f& v){
    if(freeList.empty()){
        Bullet b(name,p,v);
        bulletList.push_back(b);
    }
    else{
        freeList.begin()->reset(p,v);
        bulletList.push_back(*freeList.begin());
        freeList.erase(freeList.begin());
    }
}

void ObjectPool::releaseBullet(Uint32 ticks){
     std::list<Bullet>::iterator ptr = bulletList.begin();
    while(ptr != bulletList.end()) {
        ptr->update(ticks);
        if(ptr->getLife() >600) {
            freeList.push_back(*ptr);
            ptr = bulletList.erase(ptr);
        }
        else ++ptr;
    }
}


bool ObjectPool::collidedWith(const Drawable *obj)
{
    //static PerPixelCollisionStrategy pcs;
    std::list<Bullet>::iterator ptr = bulletList.begin();
   // Smartsprite* tmp = dynamic_cast<Smartsprite*>(obj);
    while (ptr != bulletList.end()) {
    //    if (tmp) {
      //      tmp->setAvoid((*ptr).X(), (*ptr).Y());
        //    tmp->avoidFlag = true;
       // }
        if (perPixelColl.execute(*ptr, *obj)) {
            freeList.push_back(*ptr);
            ptr = bulletList.erase(ptr);
           // tmp->avoidFlag = false;
            return true;
        }
        else ++ptr;
    }


    return false;
}


