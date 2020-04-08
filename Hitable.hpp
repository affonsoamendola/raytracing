#pragma once

#include "Camera.hpp"
#include "Ray.hpp"

class Material;

struct Hit_Record
{
	float m_propagation_parameter;
	Vector3f m_hitpoint;
	Vector3f m_normal;
	Material * m_material;
};

inline Vector3f RotateY(Vector3f point, const float& theta)
{
	return Vector3f(point[0] * cos(theta) - point[2] * sin(theta), point[1], point[2] * cos(theta) + point[0] * sin(theta));
}

inline Vector3f RotateX(Vector3f point, const float& theta)
{
	return Vector3f(point[0], point[1] * cos(theta) - point[2] * sin(theta), point[2] * cos(theta) + point[1] * sin(theta));
}

class Hitable
{
public:
	virtual bool Hit(const Camera& cam, const Ray& r, float parameter_min, float parameter_max, Hit_Record& record) const = 0;
	
	inline Vector3f Translate(const Vector3f& value, const Vector3f& amount) const
	{
		return value + amount;
	}

	inline Vector3f Rotate(const Vector3f& value, const float& thetaY, const float& thetaX) const
	{
		Vector3f holder;

		holder = RotateY(value, thetaY);
		holder = RotateX(holder, thetaX);

		return holder;
	}

	Vector3f m_position;
};

