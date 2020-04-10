#pragma once

#include "Vector3f.hpp"

class Ray;
class Material;

struct Hit_Record
{
	float m_propagation_parameter;
	Vector3f m_hitpoint;
	Vector3f m_normal;
	Material * m_material;
};

class Hitable
{
public:
	virtual bool Hit(const Ray& r, float parameter_min, float parameter_max, Hit_Record& record) const = 0;

	Vector3f m_world_position;
	Vector3f m_local_position;
	Material * m_material;
};

