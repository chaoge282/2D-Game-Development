#include "ioManager.h"
#include "aaline.h"
#include "time.h"
#include "objectPool.h"

class Huds {
public:
  Huds(const std::string & name);
  ~Huds();
  void draw() const;
  void update(Uint32);
  void display();
private:
  const IOManager & io;
  Clock& clock;
  SDL_Surface* screen;
  Vector2f position;
  int length;
  int width;
  float thick;
  int seconds;
  const Uint32 color;
  bool appear;
  ObjectPool& objectPool;

  void drawBox() const;
  Huds(const Huds&);
  Huds& operator=(const Huds&);
};
