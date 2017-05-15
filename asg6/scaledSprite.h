#ifndef SCALEDSPRITE__H
#define SCALEDSPRITE__H

#include <vector>
#include <string>
#include <iostream>

#include "multisprite.h"

class ScaledSprite : public MultiSprite {
public:
    ScaledSprite(const std::string&);
  ScaledSprite(const ScaledSprite&);
  virtual ~ScaledSprite(){}

  ScaledSprite& operator=(const ScaledSprite&);
  bool operator<(const ScaledSprite& rhs) const {
    return scale < rhs.scale;
  }

  virtual void draw() const;
  virtual void update(Uint32 ticks);
  virtual void explode();
  unsigned getPixel(Uint32, Uint32) const;
  double getScale() const { return scale; }
  void setScale(const double s)  {scale =s; }

  Vector2f getCenter() const {
      return Vector2f( X()+frames[currentFrame]->getWidth()/2,
              Y()+frames[currentFrame]->getHeight()/2 );
  }

private:
 double scale;
 int smooth;

float getRandFloat(float,float);
};
#endif
