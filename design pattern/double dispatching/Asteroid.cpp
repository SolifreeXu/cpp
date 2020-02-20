#include "Asteroid.h"

std::ostream& operator<<(std::ostream& ostream, const Asteroid& asteroid)
{
#define string(x) #x
	ostream << string(Asteroid);
	return ostream;
}

Asteroid::Asteroid()
{
}

Asteroid::~Asteroid()
{
}
