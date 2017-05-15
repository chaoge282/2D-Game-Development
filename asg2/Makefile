# Change the following values to suit your system.

CFLAGS=`sdl-config --cflags` -g -W -Wall -Weffc++ -Wextra -pedantic -O0
SDL_LIB=`sdl-config --libs` -lSDL_ttf -lSDL_image
CCC=g++

OBJECTS = Manager.o Sprite.o generateFrames.o

run: main.o $(OBJECTS)
	$(CCC) $(CFLAGS) main.cpp -o run $(OBJECTS) $(SDL_LIB)

main.o: main.cpp generateFrames.h Manager.h Sprite.h
	$(CCC) $(CFLAGS) -c main.cpp

Manager.o: Manager.cpp Manager.h generateFrames.h Sprite.h
	$(CCC) $(CFLAGS) -c Manager.cpp

Sprite.o: Sprite.cpp Sprite.h
	$(CCC) $(CFLAGS) -c Sprite.cpp

generateFrames.o: generateFrames.cpp generateFrames.h
	$(CCC) $(CFLAGS) -c generateFrames.cpp

clean:
	rm -f run *.o
	rm -f *~
	rm -f frames/*.bmp
