#pragma once
#include<map>
#include "GameObject.h"

class ObjectNumbering
{
public:
	void setIndex(GameObject& gameobject)
	{
		index_[&gameobject] = nextIndex_;
		gameobject.setIndex(nextIndex_);
		nextIndex_++;
	}
	int getIndex(const GameObject& gameobject)
	{
		return index_[&gameobject];
	}
private:
	int nextIndex_ = 0;
	std::map<const GameObject*, int> index_;
};