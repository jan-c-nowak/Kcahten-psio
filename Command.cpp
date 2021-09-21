#include "Command.h"
#include <iostream>
//MOVE COMMANDS

MoveObject::MoveObject(MovableObject& object, float& dtime) : ObjectCommand(object)
{
	this->dtime_ = &dtime;
}

MoveObject::~MoveObject()
{
}

void MoveObject::undo()
{
	this->object_->setPosition(this->prev_pos);
	this->object_->update_entities();
}


void ObjectStop::execute()
{
	return;
}

void MoveObjectUp::execute()
{
	prev_pos = this->object_->getPosition();
	this->object_->move(0, (*this->dtime_) * -this->object_->getVelocity().y);
	this->object_->setRotation(0);
	this->object_->update_entities();
}

void MoveObjectDown::execute()
{
	prev_pos = this->object_->getPosition();
	this->object_->move(0, (*this->dtime_) * this->object_->getVelocity().y);
	this->object_->setRotation(180);
	this->object_->update_entities();
}


void MoveObjectRight::execute()
{
	prev_pos = this->object_->getPosition();
	this->object_->move((*this->dtime_) * this->object_->getVelocity().y, 0);
	this->object_->setRotation(90);
	this->object_->update_entities();
}


void MoveObjectLeft::execute()
{
	prev_pos = this->object_->getPosition();
	this->object_->move((*this->dtime_) * -this->object_->getVelocity().x, 0);
	this->object_->setRotation(-90);
	this->object_->update_entities();
}



//OTHER COMMANDS

Command::~Command()
{
}

ObjectCommand::ObjectCommand(MovableObject& object)
{
	this->object_ = &object;
}

ObjectCommand::~ObjectCommand()
{
}

void PlayerAttack::execute()
{
	dynamic_cast<PlayerObject*>(this->object_)->attack();
}

void PlayerAttack::undo()
{
	return;
}
