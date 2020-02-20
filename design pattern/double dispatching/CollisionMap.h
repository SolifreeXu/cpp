#pragma once

#include <typeinfo>

#include "GameObject.h"

class CollisionMap
{
	CollisionMap();
public:
	typedef void(*Functor)(GameObject&, GameObject&);
	CollisionMap(const CollisionMap&) = delete;
	~CollisionMap();
	CollisionMap& operator=(const CollisionMap&) = delete;
	static CollisionMap& getCollisionMap();
	void addFunctor(const type_info *left,
		const type_info *right,
		Functor functor,
		bool symmetrical = true);
	void removeFunctor(const type_info *left,
		const type_info *right);
	Functor search(const type_info *left,
		const type_info *right);
};
