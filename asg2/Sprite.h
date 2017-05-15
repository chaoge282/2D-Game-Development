#include <SDL.h>

class Sprite{
  public:
	Sprite():START_X(427.0),Y_VELOCITY(200.0),DT(17u){}
	~Sprite(){}
	bool update(float& y);
  private:
	const float START_X;
	const float Y_VELOCITY;
	const unsigned int DT;
};
