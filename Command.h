#pragma once
#include "GameObject.h"

class Command
{
public:
	virtual ~Command() = 0;
	virtual void execute() = 0;
};

//MOVE COMMANDS
class ObjectStop : public Command
{
public:
	void execute();
};

class ObjectCommand : public Command
{
public:
	ObjectCommand(MovableObject& object);
	~ObjectCommand() = 0;
	virtual void execute() = 0;
	virtual void undo() = 0;

	MovableObject* getObjectPtr() { return object_; }
protected:
	MovableObject* object_;
};

class MoveObject : public ObjectCommand
{
public:
	MoveObject(MovableObject& object, float& dtime);
	~MoveObject();
	virtual void execute() = 0;
	virtual void undo();
protected:
	sf::Vector2f prev_pos = {0,0};
	float* dtime_;
};

class MoveObjectUp : public MoveObject
{
public:
	using MoveObject::MoveObject;
	void execute();
};

class MoveObjectDown : public MoveObject
{
public:
	using MoveObject::MoveObject;
	void execute();
};

class MoveObjectRight : public MoveObject
{
public:
	using MoveObject::MoveObject;
	void execute();
};

class MoveObjectLeft : public MoveObject
{
public:
	using MoveObject::MoveObject;
	void execute();
};



//OTHER COMMANDS

class PlayerAttack : public ObjectCommand
{
public:
	using ObjectCommand::ObjectCommand;
	void execute();
	void undo();
};