#include "InputHandle.h"

InputHandle::InputHandle(Command* keyW, Command* keyA, Command* keyS, Command* keyD, Command* keySpace)
{
	this->keyW_ = keyW;
	this->keyA_ = keyA;
	this->keyS_ = keyS;
	this->keyD_ = keyD;
	this->keySpace_ = keySpace;
}

std::vector<Command*> InputHandle::readInput()
{
	commandsToExecute_.clear();
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) commandsToExecute_.push_back(this->keyA_);
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) commandsToExecute_.push_back(this->keyD_);
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) commandsToExecute_.push_back(this->keyS_);
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) commandsToExecute_.push_back(this->keyW_);
	else commandsToExecute_.push_back(this->stop_);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) commandsToExecute_.push_back((this->keySpace_));
	

	return commandsToExecute_;
}

InputHandle::~InputHandle()
{
	delete stop_;
}



