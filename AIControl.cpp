#include "AIControl.h"
#include <iostream>
#include <cmath>

AIControl::AIControl()
{

}

AIControl::AIControl(const PlayerObject& player, std::vector<EnemyObject*>& enemies, 
	std::array<std::map<DIRECTION, Command*>, 10>& ecomms) : player_(&player), enemies_(&enemies), enemiescomms_(&ecomms){}



AIControl::~AIControl()
{

}

std::vector<Command*> AIControl::AICupdate()
{
	bool vert_axis_set = false;
	std::vector<Command*> retComms;
	for (auto& el : *enemies_)
	{
		if (abs(el->getPosition().y - player_->getPosition().y) < 1) vert_axis_set = true;
		
		DIRECTION temp;
		if (el->getPosition().x < player_->getPosition().x) temp = DIRECTION::RIGHT;
		else temp = DIRECTION::LEFT;
		if (!vert_axis_set)
		{
			if (el->getPosition().y < player_->getPosition().y) temp = DIRECTION::DOWN;
			else temp = DIRECTION::UP;
			std::cout << el->getPosition().y << "/////" << player_->getPosition().y << "\n\n";
		}
		retComms.push_back(enemiescomms_->at(el->getIndex())[temp]);
	}
	return retComms;
}

