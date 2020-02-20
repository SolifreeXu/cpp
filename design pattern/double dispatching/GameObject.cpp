#include "GameObject.h"

std::ostream& operator<<(std::ostream& ostream, const GameObject& gameObject)
{
#define string(x) #x
	ostream << string(GameObject);
	return ostream;
}

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}
