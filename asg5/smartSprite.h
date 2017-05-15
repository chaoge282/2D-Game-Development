#ifndef __SMARTSPRITE_H__
#define __SMARTSPRITE_H__

#include "twowaysprite.h"

enum MODE{
    NORMAL=0,
    EVADE
};

class SmartSprite: public twoWaySp{
public:
    SmartSprite(const std::string& name);
    virtual void draw() const;
    virtual void update(Uint32 ticks);


    int getDistance(float &x1, float &y1, float &x2, float &y2) const;

    void avoidSprite(float x1, float y1) {targetx = x1; targety = y1; }
    bool getAvoidFlag() const {return avoidFlag;}
    void setAvoidFlag(bool a){avoidFlag = a;}
private:

    SmartSprite(const SmartSprite&);
    SmartSprite& operator=(const SmartSprite&);
    float safeDistance;
    bool avoidFlag;
    MODE currentMode;
    float targetx;
    float targety;

    void goLeft();
    void goRight();
    void goUp();
    void goDown();
};



#endif
