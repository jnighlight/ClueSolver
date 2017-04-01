#pragma once
#include "Cli.h"
#include "Engine.h"

/**
 * @brief The "Controller" of MVC. Transfers information from the "Model" (Engine)
 *  and the "View" (CLI)
 *
 * Manages the output of the gui, and takes the user information from there and sends it to
 *  the engine to be processed. Then updates the GUI with the new information from the Engine
 */
class Game
{
public:
	Game();
	~Game();

    /**
     * @brief Grabs game start state from CLI and sets it in the Engine
     */
	void start();

    /**
     * @brief Retrieves and processes guesses until the game is completed
     */
    void guessLoop();

private:
	Cli m_cli;
	Engine m_engine;
};

