#pragma once

#include <ostream>

class GameObject
{
	friend std::ostream& operator<<(std::ostream& ostream, const GameObject& gameObject);
public:
	GameObject();
	virtual ~GameObject();
};
