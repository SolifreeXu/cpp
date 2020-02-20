#include "Spaceship.h"

std::ostream& operator<<(std::ostream& ostream, const Spaceship &spaceship)
{
#define string(x) #x
	ostream << string(Spaceship);
	return ostream;
}

Spaceship::Spaceship()
{
}

Spaceship::~Spaceship()
{
}
