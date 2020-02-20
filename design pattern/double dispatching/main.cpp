#include "Spaceship.h"
#include "SpaceStation.h"
#include "Asteroid.h"
#include "CollisionMap.h"
#include "UnknownCollision.h"

#include <iostream>

namespace {
	void spaceshipSpaceStation(GameObject& left, GameObject& right)
	{
		auto spaceship = dynamic_cast<Spaceship&>(left);
		auto spaceStation = dynamic_cast<SpaceStation&>(right);
		std::cout << spaceship << ' ' << spaceStation << std::endl;
		std::cout << &spaceship << ' ' << &spaceStation << std::endl;
	}

	void spaceshipAsteroid(GameObject& left, GameObject& right)
	{
		auto spaceship = dynamic_cast<Spaceship&>(left);
		auto asteroid = dynamic_cast<Asteroid&>(right);
	}

	void spaceStationAsteroid(GameObject& left, GameObject& right)
	{
		auto spaceStation = dynamic_cast<SpaceStation&>(left);
		auto asteroid = dynamic_cast<Asteroid&>(right);
	}
}

namespace {
	void registerFunctor()
	{
		auto& collisionMap = CollisionMap::getCollisionMap();
		collisionMap.addFunctor(&typeid(Spaceship), &typeid(SpaceStation), spaceshipSpaceStation);
		collisionMap.addFunctor(&typeid(Spaceship), &typeid(Asteroid), spaceshipAsteroid);
		collisionMap.addFunctor(&typeid(SpaceStation), &typeid(Asteroid), spaceStationAsteroid);
	}

	void collide(GameObject& left, GameObject& right)
	{
		auto& collisionMap = CollisionMap::getCollisionMap();
		auto functor = collisionMap.search(&typeid(left), &typeid(right));
		if (functor)
			functor(left, right);
		else
			throw UnknownCollision(left, right);
	}
}

int main()
{
	std::cout << std::boolalpha << (typeid(Spaceship) == typeid(Spaceship())) << std::endl;
	Spaceship spaceship;
	std::cout << std::boolalpha << (typeid(Spaceship) == typeid(spaceship)) << std::endl;

	registerFunctor();
	SpaceStation spaceStation;
	collide(spaceship, spaceStation);
	collide(spaceStation, spaceship);
	return 0;
}
