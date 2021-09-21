#include "Game.h"
#include <iostream>

//PROTOTYPE SPACE

//***********************************8
Game::Game()
{
    //Inicjalizacja potrzebnych elementow
    initWindow();
    //Wczytanie danych
    texturemanager_.loadInitTextures();

    //Setup of Map and Entities
    addObject(new PlayerObject(texturemanager_.getTextureArray().at((int)TEXTURE::PLAYER), { 100,100 }));
    addObject(new EnemyObject(texturemanager_.getTextureArray().at((int)TEXTURE::ENEMY1), {380,260 }));


    loadLevel();

    //Korzystajace z setupu inity
    initView();
    initHUD();
    initCommands();
    initInputHandler();
    initAIC();
}
    
Game::~Game()
{
	delete window_;
    delete player_;
    delete input_;
    delete ai_;
}

void Game::run()
{
    dt_ = dtclock_.restart().asSeconds();
	while (this->window_->isOpen())
	{
        this->inputToQueue();
		this->update();
		this->render();
        
        
        sf::Event e;
        while (this->window_->pollEvent(e))
        {
            switch (e.type)
            {
            case sf::Event::KeyPressed:
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window_->close();
                break;
            case sf::Event::Closed:
                window_->close();
                break;
            case sf::Event::KeyReleased:
                if(e.key.code == sf::Keyboard::Space) this->player_->rel_button_attack_ = true;
                break;
            default:
                break;
            }
        }
	}

}

void Game::update()
{
    this->executeCommands();
    this->executeAICommands(this->ai_->AICupdate());
    this->checkCollision();
    this->updateView();
    this->updateHUD();
}

void Game::render()
{
    if (player_->getHP() > 0 && !enemies_.front()->isDead())
    {
    this->window_->clear(sf::Color::Yellow);
    drawPlayer();
    for (auto& el : this->objects_) {
        this->window_->draw(*el);
    }
    
    this->window_->draw(playerinfo_);
    this->window_->draw(enemyinfo_);
    }
    else if (enemies_.front()->isDead())
    {
        this->endgame_.setString("WYGRA£ES!");
        this->endgame_.setCharacterSize(30);
        this->endgame_.setOrigin(endgame_.getLocalBounds().width / 2, endgame_.getLocalBounds().height / 2);
        this->endgame_.setPosition(this->view_.getCenter().x, this->view_.getCenter().y);
        this->window_->clear(sf::Color::Red);
        this->window_->draw(endgame_);
        //napis
    }
    else if (player_->getHP() == 0)
    {
        this->endgame_.setString("PRZEGRALES!");
        this->endgame_.setCharacterSize(30);
        this->endgame_.setOrigin(endgame_.getLocalBounds().width / 2, endgame_.getLocalBounds().height / 2);
        this->endgame_.setPosition(this->view_.getCenter().x, this->view_.getCenter().y);
        this->window_->clear(sf::Color::Cyan);
        this->window_->draw(endgame_);
        //napis
    }
    this->window_->display();
}

void Game::initCommands()
{
    this->playercomms_[static_cast<int>(COMMAND::MOVE_UP)] = new MoveObjectUp(*this->player_, this->dt_);
    this->playercomms_[static_cast<int>(COMMAND::MOVE_DOWN)] = new MoveObjectDown(*this->player_, this->dt_);
    this->playercomms_[static_cast<int>(COMMAND::MOVE_LEFT)] = new MoveObjectLeft(*this->player_, this->dt_);
    this->playercomms_[static_cast<int>(COMMAND::MOVE_RIGHT)] = new MoveObjectRight(*this->player_, this->dt_);
    this->playercomms_[static_cast<int>(COMMAND::ATTACK)] = new PlayerAttack(*this->player_);
}

void Game::initWindow()
{
    window_ = new sf::RenderWindow(sf::VideoMode(1024, 720), "Eagleview", /*sf::Style::Fullscreen | */ sf::Style::Close | sf::Style::Titlebar);
    window_->setVerticalSyncEnabled(true);
}

void Game::initInputHandler()
{
    this->input_ = new InputHandle(this->playercomms_[int(COMMAND::MOVE_UP)],
        this->playercomms_[int(COMMAND::MOVE_LEFT)],
        this->playercomms_[int(COMMAND::MOVE_DOWN)],
        this->playercomms_[int(COMMAND::MOVE_RIGHT)],
        this->playercomms_[int(COMMAND::ATTACK)]);
}

void Game::initAIC()
{
    this->ai_ = new AIControl(*this->player_,this->enemies_,this->enemiescomms_);
}

void Game::checkCollision()
{
    //PLAYER -- OBJECT COLLISION
    for (auto& el : objects_)
    {
        //zmienic to, utrudnia (ale dziala, wiec nie takie glupie)
        if (typeid(*el) == typeid(EnemyObject) && dynamic_cast<EnemyObject*>(el)->getVelocity().x == 0) dynamic_cast<EnemyObject*>(el)->setVelocity(3, 3);
        if (player_->getGlobalBounds().intersects(el->getGlobalBounds()))
        {
            if (typeid(*el) == typeid(EnemyObject))
            {
                if(!enemies_.front()->isDead()) player_->getDamage();
                this->playerinfo_.setString("HP: " + std::to_string(this->player_->getHP()));
                undo_enemy[el->getIndex()]->undo();
                dynamic_cast<EnemyObject*>(el)->setVelocity(0, 0); //czesc tego kodu co wyzej
            }
            std::cout << player_->getHP() << "\n";
            if(last_move_command!=nullptr)
            { 
                last_move_command->undo();
            }
        }
    }

    //ENEMY -- WEAPON COLLISION
    for (auto& el : objects_)
    {
        if (typeid(*el) == typeid(EnemyObject) &&
            player_->getWeapon().getGlobalBounds().intersects(el->getGlobalBounds()) &&
            this->player_->getWeapon().isOut() &&
            !dynamic_cast<EnemyObject*>(el)->isDead() &&
            this->player_->getHP() > 0)
        {
            dynamic_cast<EnemyObject*>(el)->getDamage();
            this->enemyinfo_.setString("HP: " + std::to_string(this->enemies_.front()->getHP()));
        }
    }

    //ENEMY -- OTHER OBJECT COLLISION
    for (auto& el : objects_)
    {
        if (typeid(*el) != typeid(EnemyObject))
        {
            for (auto& elEnem : enemies_)
            {
                if(elEnem->getGlobalBounds().intersects(el->getGlobalBounds())) undo_enemy[elEnem->getIndex()]->undo();
            }
        }
    }
}

void Game::executeCommands()
{
    while (commq_.empty() != true)
    {
        sf::Vector2f pos = this->player_->getPosition(); //nasty hack
        commq_.front()->execute();
        if (this->player_->getPosition() != pos) last_move_command = dynamic_cast<MoveObject*>(commq_.front());
        commq_.pop();
    }
}

void Game::initView()
{
    this->view_.setSize(this->windowWidth / 3, this->windowHeight / 3);
    this->view_.setCenter(this->player_->getPosition());
    window_->setView(this->view_);
}

void Game::inputToQueue()
{
    std::vector<Command*> temp_ = input_->readInput();
    for (auto& el : temp_)
    {
        commq_.push(el);
    }
}

void Game::updateView()
{
    this->view_.setCenter(this->player_->getPosition());
    window_->setView(view_);
}

void Game::drawPlayer()
{
    this->window_->draw(*this->player_);
    if(this->player_->getWeapon().isOut()) this->window_->draw(this->player_->getWeapon());
}

void Game::executeAICommands(const std::vector<Command*>& commands)
{
    for (auto& el : commands)
    {
        if (dynamic_cast<EnemyObject*>(dynamic_cast<ObjectCommand*>(el)->getObjectPtr())->getActivityRect().getGlobalBounds().intersects(this->player_->getGlobalBounds()))
        {
        sf::Vector2f last_temp_pos = dynamic_cast<ObjectCommand*>(el)->getObjectPtr()->getPosition();
        el->execute();
        if (dynamic_cast<ObjectCommand*>(el)->getObjectPtr()->getPosition() != last_temp_pos)
        {
            undo_enemy[dynamic_cast<ObjectCommand*>(el)->getObjectPtr()->getIndex()] = dynamic_cast<MoveObject*>(el);
        }
        }
    }
}


void Game::addObject(GameObject* gameobject)
{
    objIndex_.setIndex(*gameobject);

    if (typeid(*gameobject) == typeid(PlayerObject)) player_ = dynamic_cast<PlayerObject*>(gameobject);

    else if (typeid(*gameobject) == typeid(EnemyObject)) 
    { 
        dynamic_cast<EnemyObject*>(gameobject)->setVelocity(3, 3);
        objects_.push_back(gameobject); 
        enemies_.push_back(dynamic_cast<EnemyObject*>(gameobject));
        fillMovementCommands(*dynamic_cast<MovableObject*>(gameobject));
    }

    else objects_.push_back(gameobject);
}

void Game::fillMovementCommands(MovableObject& movableobject)
{
    enemiescomms_[movableobject.getIndex()][DIRECTION::LEFT] = new MoveObjectLeft(movableobject, dt_);
    enemiescomms_[movableobject.getIndex()][DIRECTION::RIGHT] = new MoveObjectRight(movableobject, dt_);
    enemiescomms_[movableobject.getIndex()][DIRECTION::UP] = new MoveObjectUp(movableobject, dt_);
    enemiescomms_[movableobject.getIndex()][DIRECTION::DOWN] = new MoveObjectDown(movableobject, dt_);
}

void Game::initHUD()
{
    this->font_.loadFromFile(R"(GideonRoman-Regular.ttf)");

    this->playerinfo_.setFont(this->font_);
    this->enemyinfo_.setFont(this->font_);
    this->endgame_.setFont(this->font_);

    this->playerinfo_.setCharacterSize(12);
    this->enemyinfo_.setCharacterSize(12);

    this->playerinfo_.setFillColor(sf::Color::Black);
    this->enemyinfo_.setFillColor(sf::Color::Black);
    this->endgame_.setFillColor(sf::Color::Black);
    
    this->playerinfo_.setString("Gracz HP: " + std::to_string(this->player_->getHP()));
    this->enemyinfo_.setString("Wróg HP: " + std::to_string(this->enemies_.front()->getHP()));

    this->playerinfo_.setOrigin(playerinfo_.getLocalBounds().width/2, playerinfo_.getLocalBounds().height / 2);
    this->enemyinfo_.setOrigin(enemyinfo_.getLocalBounds().width / 2, enemyinfo_.getLocalBounds().height / 2);
    this->endgame_.setOrigin(endgame_.getLocalBounds().width / 2, endgame_.getLocalBounds().height / 2);

    this->playerinfo_.setPosition(this->view_.getCenter().x-100, this->playerinfo_.getCharacterSize() + 5 );
    this->enemyinfo_.setPosition(this->view_.getCenter().x + 100, this->enemyinfo_.getCharacterSize() + 5);
    this->endgame_.setPosition(this->view_.getCenter().x, this->view_.getCenter().y);
}



void Game::loadLevel()
{
    //VERTICAL RIVER
    addObject(new TerrainObject(texturemanager_.getTextureArray().at((int)TEXTURE::TERR2), { 164,36 }));
    addObject(new TerrainObject(texturemanager_.getTextureArray().at((int)TEXTURE::TERR2), { 164,68 }));
    addObject(new TerrainObject(texturemanager_.getTextureArray().at((int)TEXTURE::TERR2), { 164,100 }));
    addObject(new TerrainObject(texturemanager_.getTextureArray().at((int)TEXTURE::TERR2), { 164,132 }));
    addObject(new TerrainObject(texturemanager_.getTextureArray().at((int)TEXTURE::TERR2), { 164,164 }));
    addObject(new TerrainObject(texturemanager_.getTextureArray().at((int)TEXTURE::TERR2), { 164,196 }));
    addObject(new TerrainObject(texturemanager_.getTextureArray().at((int)TEXTURE::TERR2), { 164,228 }));
    addObject(new TerrainObject(texturemanager_.getTextureArray().at((int)TEXTURE::TERR2), { 164,324 }));
    addObject(new TerrainObject(texturemanager_.getTextureArray().at((int)TEXTURE::TERR2), { 164, 356 }));
    addObject(new TerrainObject(texturemanager_.getTextureArray().at((int)TEXTURE::TERR2), { 164,388 }));

    //HORIZONTAL BOXES BOTTOM
    addObject(new TerrainObject(texturemanager_.getTextureArray().at((int)TEXTURE::TERR1), { 0,420 }));
    addObject(new TerrainObject(texturemanager_.getTextureArray().at((int)TEXTURE::TERR1), { 32,420 }));
    addObject(new TerrainObject(texturemanager_.getTextureArray().at((int)TEXTURE::TERR1), { 64,420 }));
    addObject(new TerrainObject(texturemanager_.getTextureArray().at((int)TEXTURE::TERR1), { 96,420 }));
    addObject(new TerrainObject(texturemanager_.getTextureArray().at((int)TEXTURE::TERR1), { 128,420 }));
    addObject(new TerrainObject(texturemanager_.getTextureArray().at((int)TEXTURE::TERR1), { 160,420 }));
    addObject(new TerrainObject(texturemanager_.getTextureArray().at((int)TEXTURE::TERR1), { 192,420 }));
    addObject(new TerrainObject(texturemanager_.getTextureArray().at((int)TEXTURE::TERR1), { 224,420 }));
    addObject(new TerrainObject(texturemanager_.getTextureArray().at((int)TEXTURE::TERR1), { 256,420 }));
    addObject(new TerrainObject(texturemanager_.getTextureArray().at((int)TEXTURE::TERR1), { 288,420 }));
    addObject(new TerrainObject(texturemanager_.getTextureArray().at((int)TEXTURE::TERR1), { 320,420 }));
    addObject(new TerrainObject(texturemanager_.getTextureArray().at((int)TEXTURE::TERR1), { 352,420 }));
    addObject(new TerrainObject(texturemanager_.getTextureArray().at((int)TEXTURE::TERR1), { 384,420 }));
    addObject(new TerrainObject(texturemanager_.getTextureArray().at((int)TEXTURE::TERR1), { 416,420 }));
    addObject(new TerrainObject(texturemanager_.getTextureArray().at((int)TEXTURE::TERR1), { 448,420 }));

    //VERTICAL BOXES LEFT
    addObject(new TerrainObject(texturemanager_.getTextureArray().at((int)TEXTURE::TERR1), { 0,36 }));
    addObject(new TerrainObject(texturemanager_.getTextureArray().at((int)TEXTURE::TERR1), { 0,68 }));
    addObject(new TerrainObject(texturemanager_.getTextureArray().at((int)TEXTURE::TERR1), { 0,100 }));
    addObject(new TerrainObject(texturemanager_.getTextureArray().at((int)TEXTURE::TERR1), { 0,132 }));
    addObject(new TerrainObject(texturemanager_.getTextureArray().at((int)TEXTURE::TERR1), { 0,164 }));
    addObject(new TerrainObject(texturemanager_.getTextureArray().at((int)TEXTURE::TERR1), { 0,196 }));
    addObject(new TerrainObject(texturemanager_.getTextureArray().at((int)TEXTURE::TERR1), { 0,228 }));
    addObject(new TerrainObject(texturemanager_.getTextureArray().at((int)TEXTURE::TERR1), { 0,260 }));
    addObject(new TerrainObject(texturemanager_.getTextureArray().at((int)TEXTURE::TERR1), { 0,292 }));
    addObject(new TerrainObject(texturemanager_.getTextureArray().at((int)TEXTURE::TERR1), { 0,324 }));
    addObject(new TerrainObject(texturemanager_.getTextureArray().at((int)TEXTURE::TERR1), { 0, 356 }));
    addObject(new TerrainObject(texturemanager_.getTextureArray().at((int)TEXTURE::TERR1), { 0,388 }));

    //horizontal boxes top
    addObject(new TerrainObject(texturemanager_.getTextureArray().at((int)TEXTURE::TERR1), { 0,4 }));
    addObject(new TerrainObject(texturemanager_.getTextureArray().at((int)TEXTURE::TERR1), { 32,4 }));
    addObject(new TerrainObject(texturemanager_.getTextureArray().at((int)TEXTURE::TERR1), { 64,4 }));
    addObject(new TerrainObject(texturemanager_.getTextureArray().at((int)TEXTURE::TERR1), { 96,4 }));
    addObject(new TerrainObject(texturemanager_.getTextureArray().at((int)TEXTURE::TERR1), { 128,4 }));
    addObject(new TerrainObject(texturemanager_.getTextureArray().at((int)TEXTURE::TERR1), { 160,4 }));
    addObject(new TerrainObject(texturemanager_.getTextureArray().at((int)TEXTURE::TERR1), { 192,4 }));
    addObject(new TerrainObject(texturemanager_.getTextureArray().at((int)TEXTURE::TERR1), { 224,4 }));
    addObject(new TerrainObject(texturemanager_.getTextureArray().at((int)TEXTURE::TERR1), { 256,4 }));
    addObject(new TerrainObject(texturemanager_.getTextureArray().at((int)TEXTURE::TERR1), { 288,4 }));
    addObject(new TerrainObject(texturemanager_.getTextureArray().at((int)TEXTURE::TERR1), { 320,4 }));
    addObject(new TerrainObject(texturemanager_.getTextureArray().at((int)TEXTURE::TERR1), { 352,4 }));
    addObject(new TerrainObject(texturemanager_.getTextureArray().at((int)TEXTURE::TERR1), { 384,4 }));
    addObject(new TerrainObject(texturemanager_.getTextureArray().at((int)TEXTURE::TERR1), { 416,4 }));
    addObject(new TerrainObject(texturemanager_.getTextureArray().at((int)TEXTURE::TERR1), { 448,4 }));

    //VERTICAL BOXES RIGHT
    addObject(new TerrainObject(texturemanager_.getTextureArray().at((int)TEXTURE::TERR1), { 448,36 }));
    addObject(new TerrainObject(texturemanager_.getTextureArray().at((int)TEXTURE::TERR1), { 448,68 }));
    addObject(new TerrainObject(texturemanager_.getTextureArray().at((int)TEXTURE::TERR1), { 448,100 }));
    addObject(new TerrainObject(texturemanager_.getTextureArray().at((int)TEXTURE::TERR1), { 448,132 }));
    addObject(new TerrainObject(texturemanager_.getTextureArray().at((int)TEXTURE::TERR1), { 448,164 }));
    addObject(new TerrainObject(texturemanager_.getTextureArray().at((int)TEXTURE::TERR1), { 448,196 }));
    addObject(new TerrainObject(texturemanager_.getTextureArray().at((int)TEXTURE::TERR1), { 448,228 }));
    addObject(new TerrainObject(texturemanager_.getTextureArray().at((int)TEXTURE::TERR1), { 448,260 }));
    addObject(new TerrainObject(texturemanager_.getTextureArray().at((int)TEXTURE::TERR1), { 448,292 }));
    addObject(new TerrainObject(texturemanager_.getTextureArray().at((int)TEXTURE::TERR1), { 448,324 }));
    addObject(new TerrainObject(texturemanager_.getTextureArray().at((int)TEXTURE::TERR1), { 448, 356 }));
    addObject(new TerrainObject(texturemanager_.getTextureArray().at((int)TEXTURE::TERR1), { 448,388 }));


    addObject(new TerrainObject(texturemanager_.getTextureArray().at((int)TEXTURE::TERR2), { 300,300 }));
}

void Game::updateHUD()
{
    this->playerinfo_.setPosition(this->view_.getCenter().x - 100, this->view_.getCenter().y - 100);
    this->enemyinfo_.setPosition(this->view_.getCenter().x + 100, this->view_.getCenter().y - 100);
}
