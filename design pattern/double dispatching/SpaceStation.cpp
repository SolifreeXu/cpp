#include "SpaceStation.h"

std::ostream& operator<<(std::ostream& ostream, const SpaceStation& spaceStation)
{
#define string(x) #x
	ostream << string(SpaceStation);
	return ostream;
}

SpaceStation::SpaceStation()
{
}

SpaceStation::~SpaceStation()
{
}
