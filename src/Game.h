#pragma once
#include "Cli.h"
#include "Engine.h"
class Game
{
public:
	Game();
	~Game();

	void start();
    void guessLoop();

private:
	Cli m_cli;
	Engine m_engine;
};

