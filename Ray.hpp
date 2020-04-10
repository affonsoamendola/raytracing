#pragma once

#include "Vector3f.hpp"

class Ray
{
public:
	Ray() {}
	Ray(const Vector3f& _origin, const Vector3f& _direction) :
		m_origin(_origin), m_direction(_direction) {}
	//Always use unit vectors for _direction.

	inline Vector3f propagate(float t) const { return m_origin + t * m_direction; }

	Vector3f m_origin;
	Vector3f m_direction;
};