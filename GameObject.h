#pragma once
#include <SFML/Graphics.hpp>
#include <array>

class GameObject : public sf::Sprite
{
public:
	GameObject();
	GameObject(const sf::Texture& pTex, const sf::Vector2f& position);
	virtual ~GameObject() = 0;
	virtual void update_entities();
	void setIndex(const int& index);
	int getIndex();
protected:
	sf::Texture texture_;
private:
	int index_;
};

class Weapon : public GameObject
{
public:
	Weapon();
	~Weapon();
	bool isOut();
	void changeState();
	sf::Clock getFatigue();
private:
	int power_ = 1;
	sf::Vector2f owner_position;
	bool active_ = false;
	sf::Clock fatigue_;
};

class MovableObject : public GameObject 
{
public:
	using GameObject::GameObject;
	~MovableObject();

	virtual sf::Vector2f getVelocity();
	virtual void setVelocity(const float& x, const float& y);
	virtual void getDamage();
protected:
	int hp_ = 3;
	sf::Clock damageClock_;
	int immunityTime_ = 4;
private:
	float hVelocity = 10; //horizontal velocity
	float vVelocity = 10; //vertical velocity
};

class StationaryObject : public GameObject 
{
public:
	using GameObject::GameObject;
	~StationaryObject();
};

class PlayerObject : public MovableObject
{
public:
	using MovableObject::MovableObject;
	~PlayerObject();

	void attack();

	int getHP();
	void setHP(int hp);
	//void getDamage();
	Weapon getWeapon();

	void update_entities();
	bool rel_button_attack_ = true;
private:
	
	sf::Clock attackClock_;
	
	
	Weapon weapon_;
};

class TerrainObject : public StationaryObject
{
public:
	using StationaryObject::StationaryObject;
	~TerrainObject();
};

class EnemyObject : public MovableObject
{
public:
	using MovableObject::MovableObject;

	void setIdle();
	void setActive();
	bool isDead();
	int getHP();
	sf::RectangleShape getActivityRect();
protected:
	sf::RectangleShape activityRect;
	bool idle_ = true;
	void attack();
};

enum class DIRECTION { RIGHT, LEFT, UP, DOWN };

