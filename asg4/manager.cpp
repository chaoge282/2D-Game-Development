#include <iostream>
#include <string>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include "multisprite.h"
#include "twowaysprite.h"
#include "sprite.h"
#include "gamedata.h"
#include "manager.h"

class ScaledSpriteCompare{
public:
    bool operator()(const ScaledSprite* lhs, const ScaledSprite* rhs){
        return lhs->getScale() < rhs->getScale();
    }
};

Manager::~Manager() {
  // These deletions eliminate "definitely lost" and
  // "still reachable"s in Valgrind.
  SDL_FreeSurface(orbSurface);
 // SDL_FreeSurface(buildingSurface);
  //SDL_FreeSurface(mountainSurface);
  for (unsigned i = 0; i < sprites.size(); ++i) {
    delete sprites[i];
  }
  for (unsigned i = 0; i < orbs.size(); ++i) {
      delete orbs[i];
  }

}

Manager::Manager() :
  env( SDL_putenv(const_cast<char*>("SDL_VIDEO_CENTERED=center")) ),
  io( IOManager::getInstance() ),
  clock( Clock::getInstance() ),
  screen( io.getScreen() ),
  orbSurface( io.loadAndSet(Gamedata::getInstance().getXmlStr("orbs/file"),
                Gamedata::getInstance().getXmlBool("orbs/transparency")) ),
  //buildingSurface( io.loadAndSet(Gamedata::getInstance().getXmlStr("building/file"),
        //        Gamedata::getInstance().getXmlBool("building/transparency")) ),
   // mountainSurface( io.loadAndSet(Gamedata::getInstance().getXmlStr("mountain/file"),
     //           Gamedata::getInstance().getXmlBool("mountain/transparency")) ),
  world("back", Gamedata::getInstance().getXmlInt("back/factor") ),
  ground("ground", Gamedata::getInstance().getXmlInt("ground/factor") ),
  building("building", Gamedata::getInstance().getXmlInt("building/factor") ),
  mountain("mountain", Gamedata::getInstance().getXmlInt("mountain/factor") ),
  viewport( Viewport::getInstance() ),
  orbs(),
  sprites(),
    player(new Player("player")),
  currentSprite(0),
  makeVideo( false ),
  frameCount( 0 ),
  numberOfOrbs (Gamedata::getInstance().getXmlInt("numberOfOrbs")),
  username(  Gamedata::getInstance().getXmlStr("username") ),
  title( Gamedata::getInstance().getXmlStr("screenTitle") ),
  frameMax( Gamedata::getInstance().getXmlInt("frameMax") ),
    huds("huds")
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    throw string("Unable to initialize SDL: ");
  }
  SDL_WM_SetCaption(title.c_str(), NULL);
  atexit(SDL_Quit);
  makeOrbs();
  printOrbs();
//  sprites.push_back( new Player("player") );
  sprites.push_back( new twoWaySp("run") );
  sprites.push_back( new MultiSprite("wolf") );
  sprites.push_back( new Sprite("monster1") );
  sprites.push_back( new Sprite("monster2") );
  sprites.push_back( new Sprite("monster3") );
  sprites.push_back( new Sprite("monster4") );
  viewport.setObjectToTrack(player);
}


void Manager::makeOrbs() {
  orbs.reserve( numberOfOrbs );
  for (unsigned i = 0; i < numberOfOrbs; ++i) {
      orbs.push_back( new ScaledSprite("orbs",orbSurface) );
  }
  sort(orbs.begin(), orbs.end(), ScaledSpriteCompare());
}
void Manager::printOrbs() const {
  for (unsigned i = 0; i < orbs.size(); ++i) {
    std::cout << orbs[i]->getScale() << std::endl;
  }
}

void Manager::draw() const {
  world.draw();
    for(unsigned int i=0;i<numberOfOrbs/3;i++)
      orbs[i]->draw();
  mountain.draw();
    for(unsigned int i=numberOfOrbs/3 +1;i<numberOfOrbs-10 ;i++)
      orbs[i]->draw();
  building.draw();
  ground.draw();
    for(unsigned int i=numberOfOrbs-9;i<orbs.size();i++)
        orbs[i]->draw();
  for (unsigned i = 0; i < sprites.size(); ++i) {
    sprites[i]->draw();
  }
  player->draw();
  huds.draw();
  io.printMessageAt(title, 10, 450);
  viewport.draw();

  SDL_Flip(screen);
}

void Manager::makeFrame() {
  std::stringstream strm;
  strm << "frames/" << username<< '.'
       << std::setfill('0') << std::setw(4)
       << frameCount++ << ".bmp";
  std::string filename( strm.str() );
  std::cout << "Making frame: " << filename << std::endl;
  SDL_SaveBMP(screen, filename.c_str());
}

void Manager::switchSprite() {
    unsigned int current;
   current = (currentSprite++) % sprites.size();
   if(current == sprites.size()-1){
      viewport.setObjectToTrack(player);
  }
  else viewport.setObjectToTrack(sprites[current]);
}

void Manager::update() {
  ++clock;
  Uint32 ticks = clock.getElapsedTicks();

  static unsigned int lastSeconds = clock.getSeconds();
  if ( clock.getSeconds() - lastSeconds == 5 ) {
    lastSeconds = clock.getSeconds();
    //switchSprite();
  }
  for (unsigned int i = 0; i < sprites.size(); ++i) {
    sprites[i]->update(ticks);
  }
  player->update(ticks);
  if ( makeVideo && frameCount < frameMax ) {
    makeFrame();
  }
  for (unsigned i = 0; i < orbs.size(); ++i) {
    orbs[i]->update(ticks);
  }
  huds.update(ticks);
  world.update();
  mountain.update();
  building.update();
  ground.update();
  viewport.update(); // always update viewport last
}

void Manager::play() {
  SDL_Event event;
  bool done = false;
  bool keyCatch = false;
  ++clock;
  Uint32 ticks = clock.getElapsedTicks();
  static unsigned int lastSeconds = clock.getSeconds();
  if ( clock.getSeconds() - lastSeconds == 5 ) {
    lastSeconds = clock.getSeconds();
    //switchSprite();
  }
  while ( not done ) {
      SDL_PollEvent(&event);
      Uint8 *keystate = SDL_GetKeyState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
    if(event.type == SDL_KEYUP) {
      keyCatch = false;
    }
      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDLK_ESCAPE] || keystate[SDLK_q]) {
          done = true;
          break;
        }
        if ( keystate[SDLK_t] && !keyCatch ) {
            keyCatch = true;
          switchSprite();
        }
        if ( keystate[SDLK_o] && !keyCatch ) {
            keyCatch = true;
          clock.toggleSloMo();
        }
        if ( keystate[SDLK_p] && !keyCatch) {
            keyCatch = true;
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }
        if (keystate[SDLK_F4] && !makeVideo && !keyCatch) {
            keyCatch = true;
          std::cout << "Making video frames" << std::endl;
          makeVideo = true;
        }
        if (keystate[SDLK_F1] && !keyCatch) {
            keyCatch = true;
            huds.display();
        }
      }
        if ( keystate[SDLK_a] ) {
           // sprites[0]->velocityX(-abs(sprites[0]->velocityX()));
           // sprites[0]->updateMove(ticks,1);
            player->velocityX(-abs(player->velocityX()));
            player->updateMove(ticks,1);
        }
        if ( keystate[SDLK_d] ) {
            //sprites[0]->velocityX(abs(sprites[0]->velocityX()));
            //sprites[0]->updateMove(ticks,1);
            player->velocityX(abs(player->velocityX()));
            player->updateMove(ticks,1);

        }
        if ( keystate[SDLK_s] ) {
            //sprites[0]->velocityY(abs(sprites[0]->velocityY()));
            //sprites[0]->updateMove(ticks,0);
            player->velocityY(abs(player->velocityY()));
            player->updateMove(ticks,0);
        }
        if ( keystate[SDLK_w] ) {
            //sprites[0]->velocityY(-abs(sprites[0]->velocityY()));
            //sprites[0]->updateMove(ticks,0);
            player->velocityY(-abs(player->velocityY()));
            player->updateMove(ticks,0);
        }
    draw();
    update();
  }
}
