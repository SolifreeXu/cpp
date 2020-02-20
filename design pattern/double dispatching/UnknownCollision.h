#pragma once

#include "GameObject.h"

class UnknownCollision
{
public:
	UnknownCollision(const GameObject& left, const GameObject& right);
	~UnknownCollision();
};
