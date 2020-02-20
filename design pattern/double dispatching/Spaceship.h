#pragma once

#include "GameObject.h"

#include <ostream>

class Spaceship :
	public GameObject
{
	friend std::ostream& operator<<(std::ostream& os, const Spaceship &spaceship);
public:
	Spaceship();
	virtual ~Spaceship();
};
