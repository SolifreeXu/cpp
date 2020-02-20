#include "CollisionMap.h"

#include <map>
#include <memory>

namespace {
	typedef std::pair<const type_info*, const type_info*> TypePair;
	typedef std::map<TypePair, CollisionMap::Functor> FunctorMap;
	FunctorMap *functorMap;
	void reverse(GameObject& left, GameObject& right)
	{
		auto functor = functorMap->find(std::make_pair(&typeid(right), &typeid(left)));
		if (functor != functorMap->end())
			functor->second(right, left);
	}
}

CollisionMap::CollisionMap()
{
	functorMap = new FunctorMap;
}

CollisionMap::~CollisionMap()
{
	delete functorMap;
}

CollisionMap& CollisionMap::getCollisionMap()
{
	static CollisionMap collisionMap;
	return collisionMap;
}

void CollisionMap::addFunctor(const type_info *left,
	const type_info* right,
	Functor functor,
	bool symmetrical)
{
	(*functorMap)[std::make_pair(left, right)] = functor;
	if (symmetrical)
		(*functorMap)[std::make_pair(right, left)] = reverse;
}

void CollisionMap::removeFunctor(const type_info *left,
	const type_info *right)
{
	functorMap->erase(std::make_pair(left, right));
	functorMap->erase(std::make_pair(right, left));
}

CollisionMap::Functor CollisionMap::search(const type_info *left,
	const type_info *right)
{
	auto functor = functorMap->find(std::make_pair(left, right));
	if (functor != functorMap->end())
		return functor->second;
	return nullptr;
}
