#pragma once

#include "Vector3f.hpp"

class Ray
{
public:
	Ray() {}
	Ray(const Vector3f& _origin, const Vector3f& _direction) { v3f_origin = _origin; v3f_direction = _direction; }
	//Always use unit vectors for _direction.

	inline Vector3f origin() const 		{ return v3f_origin; } 
	inline Vector3f direction() const 	{ return v3f_direction; } 

	inline Vector3f propagate(float t) const { return v3f_origin + t * v3f_direction; }

	Vector3f v3f_origin;
	Vector3f v3f_direction;
};