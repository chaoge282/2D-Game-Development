#include <iostream>
#include <string>
#include <iomanip>
#include "multisprite.h"
#include "twowaysprite.h"
#include "sprite.h"
#include "gamedata.h"
#include "manager.h"

Manager::~Manager() {
  // These deletions eliminate "definitely lost" and
  // "still reachable"s in Valgrind.
  for (unsigned i = 0; i < sprites.size(); ++i) {
    delete sprites[i];
  }
}

Manager::Manager() :
  env( SDL_putenv(const_cast<char*>("SDL_VIDEO_CENTERED=center")) ),
  io( IOManager::getInstance() ),
  clock( Clock::getInstance() ),
  screen( io.getScreen() ),
  world("back", Gamedata::getInstance().getXmlInt("back/factor") ),
  ground("ground", Gamedata::getInstance().getXmlInt("ground/factor") ),
  building("building", Gamedata::getInstance().getXmlInt("building/factor") ),
  mountain("mountain", Gamedata::getInstance().getXmlInt("mountain/factor") ),
  viewport( Viewport::getInstance() ),
  sprites(),
  currentSprite(0),
  makeVideo( false ),
  frameCount( 0 ),
  username(  Gamedata::getInstance().getXmlStr("username") ),
  title( Gamedata::getInstance().getXmlStr("screenTitle") ),
  frameMax( Gamedata::getInstance().getXmlInt("frameMax") )
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    throw string("Unable to initialize SDL: ");
  }
  SDL_WM_SetCaption(title.c_str(), NULL);
  atexit(SDL_Quit);
  sprites.push_back( new twoWaySp("run") );
  sprites.push_back( new MultiSprite("wolf") );
  sprites.push_back( new Sprite("monster1") );
  sprites.push_back( new Sprite("monster2") );
  sprites.push_back( new Sprite("monster3") );
  sprites.push_back( new Sprite("monster4") );
  viewport.setObjectToTrack(sprites[currentSprite]);
}

void Manager::draw() const {
  world.draw();
  mountain.draw();
  building.draw();
  ground.draw();
  for (unsigned i = 0; i < sprites.size(); ++i) {
    sprites[i]->draw();
  }

  io.printMessageValueAt("Seconds: ", clock.getSeconds(), 10, 30);
  clock.getFps();
  io.printMessageAt("Press T to switch sprites", 10, 45);
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
  currentSprite = (currentSprite+1) % sprites.size();
  viewport.setObjectToTrack(sprites[currentSprite]);
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
  if ( makeVideo && frameCount < frameMax ) {
    makeFrame();
  }
  world.update();
  mountain.update();
  building.update();
  ground.update();
  viewport.update(); // always update viewport last
}

void Manager::play() {
  SDL_Event event;
  bool done = false;

  while ( not done ) {
    while ( SDL_PollEvent(&event) ) {
      Uint8 *keystate = SDL_GetKeyState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDLK_ESCAPE] || keystate[SDLK_q]) {
          done = true;
          break;
        }
        if ( keystate[SDLK_t] ) {
          switchSprite();
        }
        if ( keystate[SDLK_s] ) {
          clock.toggleSloMo();
        }
        if ( keystate[SDLK_a] ) {
            sprites[0]->velocityX(-abs(sprites[0]->velocityX()));
        }
        if ( keystate[SDLK_d] ) {
            sprites[0]->velocityX(abs(sprites[0]->velocityX()));
        }
        if ( keystate[SDLK_p] ) {
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }
        if (keystate[SDLK_F4] && !makeVideo) {
          std::cout << "Making video frames" << std::endl;
          makeVideo = true;
        }
      }
    }
    draw();
    update();
  }
}
