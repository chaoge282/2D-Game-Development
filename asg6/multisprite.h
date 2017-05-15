#ifndef MULTISPRITE__H
#define MULTISPRITE__H
#include <string>
#include <vector>
#include "drawable.h"
#include "explodingSprite.h"
#include "frameFactory.h"

class MultiSprite : public Drawable {
public:
  MultiSprite(const std::string&);
  MultiSprite(const std::string&, const Vector2f& pos,const Vector2f& vel);
  MultiSprite(const MultiSprite&);
  MultiSprite& operator=(const MultiSprite&);
  virtual ~MultiSprite();

  virtual void draw() const;
  virtual void update(Uint32 ticks);
  virtual void explode();
  virtual const Frame* getFrame() const {
    return frames[currentFrame];
  }
 bool getExplosion() const {return explosion;}
 void setExplosion(bool e) {explosion =e;}
 virtual void reset();
protected:
  const std::vector<Frame *> frames;
  ExplodingSprite * explodingSprite;
  int worldWidth;
  int worldHeight;

  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int frameWidth;
  int frameHeight;
  bool explosion;

int getRandom(int,int);
    virtual void advanceFrame(Uint32 ticks);
};
#endif
