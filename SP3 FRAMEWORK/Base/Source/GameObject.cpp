#include "GameObject.h"

GameObject::GameObject()
{
	pos.SetZero();
	dir.SetZero();
	scale.Set(1, 1, 1);

	active = true;
	speed = 0.0f;
}

GameObject::~GameObject()
{
}