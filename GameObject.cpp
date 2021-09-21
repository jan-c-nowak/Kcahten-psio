#include "GameObject.h"

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}

GameObject::GameObject(const sf::Texture& pTex, const sf::Vector2f& position)
{
	this->texture_ = pTex;
	this->setTexture(texture_);
	this->setOrigin(this->texture_.getSize().x / 2, this->texture_.getSize().y / 2);
	this->setPosition(position);
}

void GameObject::update_entities()
{
	return;
}

void GameObject::setIndex(const int& index)
{
	index_ = index;
}

int GameObject::getIndex()
{
	return index_;
}


MovableObject::~MovableObject()
{
}



sf::Vector2f MovableObject::getVelocity()
{
	return { hVelocity, vVelocity };
}

void MovableObject::setVelocity(const float& x, const float& y)
{
	this->hVelocity = x;
	this->vVelocity = y;
}

//PlayerObject

PlayerObject::~PlayerObject()
{
}

void PlayerObject::attack()
{
	int temp_time = weapon_.getFatigue().getElapsedTime().asSeconds();
	if (rel_button_attack_ && (temp_time > 5 || temp_time == 0))
	{
		weapon_.getFatigue().restart();
		rel_button_attack_ = false;
		this->weapon_.changeState();
	}
}

int PlayerObject::getHP()
{
	return hp_;
}

void PlayerObject::setHP(int hp)
{
	hp_ = hp;
}

void MovableObject::getDamage()
{
	if (hp_ == 3)
	{
		hp_--;
		damageClock_.restart();
	}
	else if (hp_ < 3 && damageClock_.getElapsedTime().asSeconds()>immunityTime_)
	{
		hp_--;
		damageClock_.restart();
	}
}

Weapon PlayerObject::getWeapon()
{
	return weapon_;
}

void PlayerObject::update_entities()
{
	switch (static_cast<int>(this->getRotation()))
	{
	case 0:
		weapon_.setRotation(0);
		weapon_.setPosition(this->getPosition().x, this->getPosition().y - this->getLocalBounds().height / 2 - weapon_.getLocalBounds().height / 2);
		break;
	case 90:
		weapon_.setRotation(90);
		weapon_.setPosition(this->getPosition().x + this->getLocalBounds().width / 2 + weapon_.getLocalBounds().width/2, getPosition().y);
		break;
	case 180:
		weapon_.setRotation(180);
		weapon_.setPosition(this->getPosition().x, getPosition().y + this->getLocalBounds().height / 2 + weapon_.getLocalBounds().height / 2);
		break;
	case 270:
		weapon_.setRotation(270);
		weapon_.setPosition(this->getPosition().x - this->getLocalBounds().width / 2 - weapon_.getLocalBounds().width / 2, getPosition().y);
		break;
	}

	if (weapon_.getFatigue().getElapsedTime().asSeconds() > 4 && weapon_.isOut()) weapon_.changeState();
}

//TerrainObject

TerrainObject::~TerrainObject()
{

}

StationaryObject::~StationaryObject()
{
}



Weapon::Weapon()
{
	this->texture_.loadFromFile(R"(C:\dev\Cpp\games\eglview\Eagleview\wep.png)");
	this->setTexture(texture_);
	this->setOrigin(this->getLocalBounds().width / 2, this->getLocalBounds().height / 2);
}

Weapon::~Weapon()
{

}

bool Weapon::isOut()
{
	return active_;
}



void Weapon::changeState()
{
	if (active_) { active_ = false; }
	else { active_ = true; }
}

sf::Clock Weapon::getFatigue()
{
	return this->fatigue_;
}



bool EnemyObject::isDead()
{
	if (this->hp_ <= 0) return true;
	else return false;
}

int EnemyObject::getHP()
{
	return this->hp_;
}

sf::RectangleShape EnemyObject::getActivityRect()
{
	sf::Vector2f temp = { 200,200 };
	activityRect.setSize(temp);
	activityRect.setOrigin(activityRect.getSize().x / 2, activityRect.getSize().y / 2);
	activityRect.setPosition(this->getPosition());

	return activityRect;
}
