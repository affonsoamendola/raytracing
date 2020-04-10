#pragma once

#include "Hitable.hpp"
#include "Material.hpp"

class Sphere : public Hitable
{
public:
	Sphere() {}
	Sphere(Vector3f center, float r, Material* material) : m_radius(r)
	{ 
		m_world_position = center;
		m_material = material; 
	};

	virtual bool Hit(const Ray& r, float parameter_min, float parameter_max, Hit_Record& record) const;

	float m_radius;
};

bool Sphere::Hit(const Ray& r, float parameter_min, float parameter_max, Hit_Record& record) const
{
	Vector3f oc = r.m_origin - m_local_position;

	float a = dot(r.m_direction, r.m_direction);
	float b = dot(oc, r.m_direction);
	float c = dot(oc, oc) - m_radius * m_radius;

	float discriminant = b*b - a*c;

	if(discriminant > 0)
	{
		float temp = (-b -sqrt(b*b - a*c)) / a;

		if(temp < parameter_max && temp > parameter_min)
		{
			record.m_propagation_parameter = temp;
			record.m_hitpoint = r.propagate(temp);
			record.m_normal = (record.m_hitpoint - m_local_position) / m_radius;
			record.m_material = m_material;
			return true;
		}

		temp = (-b + sqrt(b*b - a*c)) / a;

		if(temp < parameter_max && temp > parameter_min)
		{
			record.m_propagation_parameter = temp;
			record.m_hitpoint = r.propagate(temp);
			record.m_normal = (record.m_hitpoint - m_local_position) / m_radius;
			record.m_material = m_material;
			return true;
		}
	}

	return false;
}

