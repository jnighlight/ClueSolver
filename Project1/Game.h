#pragma once
#include "Cli.h"
#include "Engine.h"
class Game
{
public:
	Game();
	~Game();

	void start();

private:
	Cli m_cli;
	Engine m_engine;
};

