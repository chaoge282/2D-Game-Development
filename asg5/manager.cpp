#include <iostream>
#include <string>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include "smartSprite.h"
#include "multisprite.h"
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
    SDL_FreeSurface(screen);
  for (unsigned i = 0; i < sprites.size(); ++i) {
    delete sprites[i];
  }
  for (unsigned i = 0; i < orbs.size(); ++i) {
      delete orbs[i];
  }
  for (unsigned i = 0; i < player.size(); ++i) {
      delete player[i];
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
  orbs(),
  sprites(),
    player(),
  currentSprite(0),
  makeVideo( false ),
  frameCount( 0 ),
  score(0),
  time(0),
    numberOfEnermy(Gamedata::getInstance().getXmlInt("numberOfEnermy")),
  numberOfOrbs (Gamedata::getInstance().getXmlInt("numberOfOrbs")),
  username(  Gamedata::getInstance().getXmlStr("username") ),
  title( Gamedata::getInstance().getXmlStr("screenTitle") ),
  frameMax( Gamedata::getInstance().getXmlInt("frameMax") ),
    huds("huds"),
    godMode(false)
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    throw string("Unable to initialize SDL: ");
  }
  SDL_WM_SetCaption(title.c_str(), NULL);
  atexit(SDL_Quit);
  creatPlayer();
  makeOrbs();
  //printOrbs();
  makeSprite();
  viewport.setObjectToTrack(player[0]);
}

void Manager::makeSprite(){
    sprites.reserve(numberOfEnermy);
  for(unsigned int i=0;i<numberOfEnermy;i++){
      sprites.push_back( new SmartSprite("smart") );
  }
}

void Manager::creatPlayer(){
    player.reserve(1);
    player.push_back(new Player("player"));
}

void Manager::makeOrbs() {
  orbs.reserve( numberOfOrbs );
  for (unsigned i = 0; i < numberOfOrbs; ++i) {
      orbs.push_back( new ScaledSprite("orbs") );
  }
  sort(orbs.begin(), orbs.end(), ScaledSpriteCompare());
}
void Manager::printOrbs() const {
  for (unsigned i = 0; i < orbs.size(); ++i) {
    std::cout << orbs[i]->getScale() << std::endl;
  }
}

void Manager::checkForCollision(Uint32 ticks)  {
    if(!player[0]->getOver()){
        time += ticks;
    }
    std::vector<Drawable*>::iterator sprite = sprites.begin();
  std::vector<ScaledSprite*>::iterator orb = orbs.end()-8;
  while ( sprite != sprites.end() ) {
      if (player[0]->beatSprite(*sprite)){
          SmartSprite *tem = dynamic_cast<SmartSprite*> (*sprite);
          tem->explode();
          sound[1];
          tem->setAvoidFlag(true);
          tem->avoidSprite(player[0]->X(),player[0]->Y());
        //  tem->setReDisp(false);
          score += 20;
      }
      if(!godMode && !player[0]->getResurval()){
        if ( player[0]->collidedWith(*sprite) ){
            SmartSprite *tmp = dynamic_cast<SmartSprite*> (*sprite);
            player[0]->explode();
            sound[3];
            tmp->explode();
            player[0]->setAppear(false);
        }
      }
    ++sprite;
  }
  while ( orb != orbs.end() ) {
        if (time%400 ==0){
            if((*orb)->getScale()<2 && !player[0]->getResurval()){
                (*orb)->setScale((*orb)->getScale()+0.1);
                score +=10;
            }
        }
        if (player[0]->beatSprite(*orb)){
            score +=50;
            sound[2];
            (*orb)->setScale((*orb)->getScale()+0.1);
        }
      if(!godMode && !player[0]->getResurval()){
        if ( player[0]->collidedWith(*orb) ) {
            player[0]->explode();
            sound[3];
            player[0]->setAppear(false);
        }
      }
      ++orb;
  }
}

/*
void Manager::deleteSprite()
{
    std::vector<Drawable*>::iterator ptr = sprites.begin();
    for(unsigned int i=0; i<numberOfEnermy;i++){
        if ((dynamic_cast<SmartSprite*>(*ptr))->getClean()) {
            delete (*ptr);
            ptr = sprites.erase(ptr);
            numberOfEnermy = numberOfEnermy-1;
        }
        else
            ++ptr;
    }
}
*/

void Manager::resetCollidedSprite()
{
    for (unsigned int i =0; i<numberOfEnermy; i++) {
        SmartSprite* tmp = dynamic_cast<SmartSprite*>(sprites[i]);
        if (tmp->getExplosion()) {
            tmp->reset();
        }
    }
}

void Manager::resetGame(){
    orbs.clear();
    makeOrbs();
    sprites.clear();
    makeSprite();
    player.clear();
    creatPlayer();
    player[0]->resetPlayer();
    score = 0;
  viewport.setObjectToTrack(player[0]);
}

void Manager::draw() const {
  world.draw();
    for(unsigned int i=0;i<numberOfOrbs/5;i++)
      orbs[i]->draw();
  mountain.draw();
    for(unsigned int i=numberOfOrbs/5;i<numberOfOrbs-8;i++)
      orbs[i]->draw();
  building.draw();
  ground.draw();
    for(unsigned int i=numberOfOrbs-8;i<orbs.size();i++)
    orbs[i]->draw();
  for (unsigned i = 0; i < sprites.size(); ++i) {
    sprites[i]->draw();
  }
  player[0]->draw();
  huds.draw();
  if(!player[0]->getOver()) io.printMessageValueAt(" scores:",score,600,30);
  else {
        sound[4];
      io.printMessageValueAt("Congratulations! Your score is:",score,300,200);
    }
  if(godMode) io.printMessageAt("YOU ARE GOD NOW!", 340, 30);
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
      viewport.setObjectToTrack(player[0]);
  }
    else  viewport.setObjectToTrack(sprites[current]);
}

void Manager::update() {
    resetCollidedSprite();
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
  player[0]->update(ticks);
  if ( makeVideo && frameCount < frameMax ) {
    makeFrame();
  }
  for (unsigned i = 0; i < orbs.size(); ++i) {
    orbs[i]->update(ticks);
  }
  huds.update(ticks);
  checkForCollision(ticks);
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
        if ( keystate[SDLK_m] && !keyCatch ) {
            keyCatch = true;
            sound.toggleMusic();
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
        if ( keystate[SDLK_r] && !keyCatch) {
            keyCatch = true;
            resetGame();
        }
        if ( keystate[SDLK_g] && !keyCatch) {
            keyCatch = true;
            godMode = !godMode;
        }
        if ( keystate[SDLK_j] && !keyCatch ) {
            keyCatch = true;
            player[0]->shoot();
            sound[0];
        }
        if ( keystate[SDLK_k] && !keyCatch) {
            keyCatch = true;
            player[0]->jump();
        }
      }
        if ( keystate[SDLK_a] ) {
            player[0]->left();
        }
        if ( keystate[SDLK_d] ) {
            player[0]->right();

        }
    draw();
    update();
  }
}
