#include "huds.h"

Huds::~Huds(){
    SDL_FreeSurface(screen);
}

Huds::Huds(const std::string & name) :
  io(IOManager::getInstance()),
  clock( Clock::getInstance() ),
  screen(io.getScreen()),
  position(Vector2f(Gamedata::getInstance().getXmlInt(name+"/x"),
  		Gamedata::getInstance().getXmlInt(name+"/y"))),
  length(Gamedata::getInstance().getXmlInt(name+"/length")),
  width(Gamedata::getInstance().getXmlInt(name+"/width")),
  thick(Gamedata::getInstance().getXmlFloat(name+"/thick")),
    seconds(0),
  color(SDL_MapRGB(screen->format, 0xff, 0x00, 0x00)),
    appear(true),
    objectPool(ObjectPool::getInstance())
{
}

void Huds::drawBox() const {
  // Two Horizontal lines
  Draw_AALine(screen, position[0], position[1],
                      position[0]+length, position[1],
                      thick, color);
  Draw_AALine(screen, position[0], position[1]+width,
                      position[0]+length, position[1]+width,
                      thick, color);
  // Two Vertical lines
  Draw_AALine(screen, position[0], position[1],
                      position[0], position[1]+width,
                      thick, color);
  Draw_AALine(screen, position[0]+length, position[1],
                      position[0]+length, position[1]+width,
                      thick, color);
  // Padding
  Draw_AALine(screen, position[0], position[1]+width/2,
                      position[0]+length, position[1]+width/2,
                      width, 0x00, 0xff, 0xff, 0xc8);
}

void Huds::draw() const {
    if(appear) {
    drawBox();
    io.printMessageValueAt("Seconds: ", clock.getSeconds(), 10, 20);
    io.printMessageValueAt("fps: ", Clock::getInstance().getFps(), 10, 35);
    io.printMessageAt("a - move left", 10, 50);
    io.printMessageAt("d - move right", 10, 65);
    io.printMessageAt("k - jump", 10, 80);
    io.printMessageAt("j - shoot", 10, 95);
    io.printMessageAt("r - reset ", 10, 110);
    io.printMessageAt("g - godMode ", 10, 125);
    io.printMessageAt("o - slow ", 10, 145);
    io.printMessageAt("p - pasue", 10, 160);
    io.printMessageAt("Press T to switch sprites", 10, 175);
    io.printMessageValueAt("freeList", objectPool.freeCount(),10, 190);
    io.printMessageValueAt("bulletList", objectPool.bulletCount(),10, 205);
  } else {
  }
}

void Huds::update(Uint32 ticks) {
    seconds += ticks;
    if(appear && seconds > 3000)
        display();
}

void Huds::display() {
   appear = !appear;
   seconds = 0;
}
