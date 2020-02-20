#pragma once

#include "GameObject.h"

#include <ostream>

class Asteroid :
	public GameObject
{
	friend std::ostream& operator<<(std::ostream& ostream, const Asteroid& asteroid);
public:
	Asteroid();
	virtual ~Asteroid();
};
