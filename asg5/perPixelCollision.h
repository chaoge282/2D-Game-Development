#include <cmath>
#include "drawable.h"

class RectangularCollision{
public:
    RectangularCollision() {}
  bool execute(const Drawable&, const Drawable&) const;
};

class PerPixelCollision{
public:
    PerPixelCollision() {}
    bool execute(const Drawable&, const Drawable&) const;
private:
   bool isVisible(Uint32, SDL_Surface*) const;
};

