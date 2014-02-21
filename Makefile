Localization:
	g++ Main.cpp Math.cpp Robot.cpp World.cpp -o Localization -I. `pkg-config --cflags --libs bullet sdl gl`
