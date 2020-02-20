#pragma once

#include "GameObject.h"

#include <ostream>

class SpaceStation :
	public GameObject
{
	friend std::ostream& operator<<(std::ostream& ostream, const SpaceStation &spaceStation);
public:
	SpaceStation();
	virtual ~SpaceStation();
};
