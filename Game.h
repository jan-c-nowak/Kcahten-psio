#pragma once

#include "GameObject.h"
#include "InputHandle.h"
#include "AllCommands.h"
#include <vector>
#include <array>
#include <queue>
#include <string>

#include "AIControl.h"
#include "TextureManager.h"
#include "ObjectNumbering.h"



class Game
{
public:
	Game();
	~Game();

	//External Functions
	void run(); //Event Check and Game loop
	void update(); //Update entities
	void render(); //Render and display graphics
private:
	//Consts

	const int windowWidth = 1024;
	const int windowHeight = 720;

	//Entities
	sf::Clock dtclock_;
	sf::RenderWindow* window_;
	std::vector<GameObject*> objects_;
	std::vector<EnemyObject*> enemies_;
	PlayerObject* player_;
	InputHandle* input_;
	AIControl* ai_;
	TextureManager texturemanager_;
	ObjectNumbering objIndex_;
	sf::View view_;
	
	
	std::queue<Command*> commq_;
	MoveObject* last_move_command;
	std::array<Command*,10> playercomms_;
	std::array<std::map<DIRECTION, Command*>, 10> enemiescomms_;
	std::array<MoveObject*, 10> undo_enemy;

	//Usables
	float dt_;
	sf::Font font_;
	sf::Text enemyinfo_;
	sf::Text playerinfo_;
	sf::Text endgame_;

	//Internal Functions
	void initCommands();
	void initWindow();
	void initInputHandler();
	void initAIC();
	void initView();
	void initHUD();

	void checkCollision();
	void executeCommands();
	void executeAICommands(const std::vector<Command*>& commands);
	void updateView();
	void updateHUD();

	void addObject(GameObject* gameobject);
	void fillMovementCommands(MovableObject& movableobject);
	void loadLevel();

	//Macros
	void inputToQueue();

	void win_lose_condition_loop();

	//Temporary hacks
	void drawPlayer();
};