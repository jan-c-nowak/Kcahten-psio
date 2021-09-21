#pragma once
#include <vector>
#include "Command.h"
#include <map>


class AIControl
{
public:
	AIControl();
	AIControl(const PlayerObject& player, std::vector<EnemyObject*>& enemies, std::array<std::map<DIRECTION, Command*>, 10>& ecomms);
	~AIControl();

	std::vector<Command*> AICupdate();
private:
	const PlayerObject* player_;
	std::vector<EnemyObject*>* enemies_;
	std::array<std::map<DIRECTION, Command*>,10>* enemiescomms_;
};

