#include "huds.h"

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
    appear(true) {
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
    io.printMessageValueAt("fps: ", Clock::getInstance().getFps(), 10, 40);
    io.printMessageAt("a - move left", 10, 60);
    io.printMessageAt("d - move right", 10, 80);
    io.printMessageAt("s - move down", 10, 100);
    io.printMessageAt("w - move up", 10, 120);
    io.printMessageAt("l - slow ", 10, 140);
    io.printMessageAt("p - pasue", 10, 160);
    io.printMessageAt("Press T to switch sprites", 10, 180);
  } else {
  }
}

void Huds::update(Uint32 ticks) {
    seconds += ticks;
    if(appear && seconds > 2000)
        display();
}

void Huds::display() {
   appear = !appear;
   seconds = 0;
}
