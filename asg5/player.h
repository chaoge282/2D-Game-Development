#include <string>
#include <vector>
#include "multisprite.h"
#include "objectPool.h"

class Player : public MultiSprite {
public:
    Player(const std::string&);
    virtual ~Player() { }

  virtual void draw() const;
  virtual void update(Uint32 ticks);
 virtual  void explode();
  void right();
  void left();
  void jump();
  void shoot();
  bool beatSprite(const Drawable* d) const;
  bool collidedWith(const Drawable* d) const;
    bool  getAppear()const {return reDisp;}
  void  setAppear(bool app) {reDisp=app;}
  bool getOver() const {return gameover;}
  void setOver(bool o){gameover=o;}
  bool getResurval() const {return resurval;}
  void setResurval(bool r){resurval=r;}
  void resetPlayer();
protected:
    PerPixelCollision perPixelColl;
    ObjectPool& objectPool;
    bool isJumping;
    bool running;
    bool reDisp;
    bool gameover;
    bool resurval;
    int deathCount;
    Player(const Player& s);
    Player& operator=(const Player& s);

virtual  void advanceFrame(Uint32 ticks);
};
