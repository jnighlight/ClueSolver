#include <iostream>
#include "Main.h"
#include "Game.h"



Main::Main()
{
}


Main::~Main()
{
}

int main(int argc, char * argv[])
{
	std::cout << "printf...I mean, Hello world\n";
	system("pause");
	Game game;
	game.start();
}
