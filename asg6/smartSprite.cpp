#include <cmath>
#include "smartSprite.h"


int SmartSprite::getDistance(float &x1, float &y1, float &x2, float &y2) const
{
    return hypot(x1-x2, y1-y2);
}

SmartSprite::SmartSprite(const std::string& name):
    twoWaySp(name),
    safeDistance(Gamedata::getInstance().getXmlFloat(name+"/safeDis")),
    avoidFlag(false),
    currentMode(NORMAL),
    targetx(0),
    targety(0)

{

}

void SmartSprite::draw() const
{
    twoWaySp::draw();
}

void SmartSprite::update(Uint32 ticks)
{
    twoWaySp::update(ticks);
    if (avoidFlag) {
        float x=X();
        float y=Y();
        float distanceToEnemy = getDistance(x, y, targetx, targety);

        if  ( currentMode == NORMAL ) {
            if(distanceToEnemy < safeDistance)
            {   currentMode = EVADE;
            }
        }
        else if  ( currentMode == EVADE ) {
            if(distanceToEnemy > safeDistance) currentMode=NORMAL;
            else {
                if ( x < targetx ) goLeft();
                if ( x > targetx ) goRight();
                if ( y < targety ) goUp();
                if ( y > targety ) goDown();
            }
        }
    }
}


void SmartSprite::goLeft()  {
    if (X() > 0) velocityX( -abs(velocityX()) );
}
void SmartSprite::goRight() { velocityX( fabs(velocityX()) ); }
void SmartSprite::goUp()    { velocityY( -fabs(velocityY()) ); }
void SmartSprite::goDown()  { velocityY( fabs(velocityY()) ); }

