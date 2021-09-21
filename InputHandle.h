#pragma once

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "Command.h"

class InputHandle
{
public:
	InputHandle(Command* keyW, Command* keyA, Command* keyS, Command* keyD, Command* keySpace);
	std::vector<Command*> readInput();
	~InputHandle();
private:
	Command* keyW_;
	Command* keyA_;
	Command* keyS_;
	Command* keyD_;
	Command* keySpace_;
	Command* stop_ = new ObjectStop();

	bool space_ready = true;
	std::vector<Command*> commandsToExecute_;
};

