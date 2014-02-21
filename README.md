To compile it you need Boost, Bullet Physics Library, SDL, and OpenGL.

If you have g++ and pkg-config the following should compile the code.

g++ Main.cpp Math.cpp Robot.cpp World.cpp -I. `pkg-config --cflags --libs bullet sdl gl`