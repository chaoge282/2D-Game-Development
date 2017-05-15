#include <string>
#include <vector>
#include "drawable.h"

class Player : public Drawable {
public:
    Player(const std::string&);
    Player(const Player&);
    virtual ~Player() { }

  virtual void draw() const;
  virtual void update(Uint32 ticks);
  virtual void updateMove(Uint32 ticks,int i);
  virtual const Frame* getFrame() const {
    return frames[currentFrame];
  }

protected:
  const std::vector<Frame *> frames;
 // PerPixelCollision * perPixelColl;
  int worldWidth;
  int worldHeight;
  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int frameWidth;
  int frameHeight;

  void advanceFrame(Uint32 ticks);
};
