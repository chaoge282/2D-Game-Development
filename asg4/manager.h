#include <vector>
#include <SDL.h>
#include "ioManager.h"
#include "world.h"
#include "huds.h"
#include "viewport.h"
#include "scaledSprite.h"
#include "player.h"

class Manager {
public:
  Manager ();
  ~Manager ();
  void play();
  void switchSprite();

private:
  const bool env;
  const IOManager& io;
  Clock& clock;

  SDL_Surface * const screen;
  SDL_Surface * const orbSurface;
//  SDL_Surface * const buildingSurface;
 // SDL_Surface * const mountainSurface;
  World world;
  World ground;
  World building;
  World mountain;
  Viewport& viewport;
  std::vector<ScaledSprite*> orbs;
  std::vector<Drawable*> sprites;
  Player* player;
  unsigned int currentSprite;
  bool makeVideo;
  int frameCount;
  unsigned int numberOfOrbs;
  const std::string username;
  const std::string title;
  const int frameMax;
  Huds  huds;

  void draw() const;
  void update();
  void makeOrbs();
  void printOrbs() const;

  Manager(const Manager&);
  Manager& operator=(const Manager&);
  void makeFrame();
};
