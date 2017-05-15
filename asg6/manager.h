#include <vector>
#include <SDL.h>
#include "ioManager.h"
#include "world.h"
#include "huds.h"
#include "viewport.h"
#include "scaledSprite.h"
#include "player.h"
#include "sound.h"

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
  World world;
  World ground;
  World building;
  World mountain;
  Viewport& viewport;
  std::vector<ScaledSprite*> orbs;
  std::vector<Drawable*> sprites;
  std::vector<Player*> player;
  //Player* player;
  int currentSprite;
  bool makeVideo;
  int frameCount;
  int score;
  int time;
  unsigned int numberOfEnermy;
  unsigned int numberOfOrbs;
  const std::string username;
  const std::string title;
  const int frameMax;
  Huds  huds;
  bool godMode;

  void draw() const;
  void update();
  void makeSprite();
  void creatPlayer();
  void makeOrbs();
  void printOrbs() const;
  void checkForCollision(Uint32 ticks) ;
  void deleteSprite();
  void resetCollidedSprite();
  void resetGame();
  Manager(const Manager&);
  Manager& operator=(const Manager&);
  void makeFrame();
};
 static  SDLSound sound;
