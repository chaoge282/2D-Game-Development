#include <string>
#include <vector>
#include "multisprite.h"

class twoWaySp : public MultiSprite {
public:
  twoWaySp(const std::string&);
  twoWaySp(const twoWaySp&);
  virtual ~twoWaySp() { }

 // virtual void draw() const;
 // virtual void update(Uint32 ticks);
  virtual void explode();
 virtual void resetVP();

protected:

 virtual  void advanceFrame(Uint32 ticks);

int getRandom(int a, int b) {
  return a + (rand() % b + 1);
}

float getRandFloat(float min, float max) {
    return min + (rand() / (RAND_MAX + 1.0f)) * (max - min);
}
};
