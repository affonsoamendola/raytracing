#pragma once

#include "Hitable.hpp"
#include "Material.hpp"

class Sphere : public Hitable
{
public:
	Sphere() {}
	Sphere(Vector3f center, float r, Material* material) :  m_radius(r)
	{ 
		m_position = center;
		m_material = material; 
	};

	virtual bool Hit(const Camera& cam, const Ray& r, float parameter_min, float parameter_max, Hit_Record& record) const;

	float m_radius;
	Material* m_material;
};

bool Sphere::Hit(const Camera& cam, const Ray& r, float parameter_min, float parameter_max, Hit_Record& record) const
{
	Vector3f translated_center = Translate(m_position, -cam.m_position);
	translated_center = Rotate(translated_center, -cam.m_rotation[0], -cam.m_rotation[1]);

	Vector3f oc = r.origin() - translated_center;

	float a = dot(r.direction(), r.direction());
	float b = dot(oc, r.direction());
	float c = dot(oc, oc) - m_radius * m_radius;

	float discriminant = b*b - a*c;

	if(discriminant > 0)
	{
		float temp = (-b -sqrt(b*b - a*c)) / a;

		if(temp < parameter_max && temp > parameter_min)
		{
			record.m_propagation_parameter = temp;
			record.m_hitpoint = r.propagate(temp);
			record.m_normal = (record.m_hitpoint - translated_center) / m_radius;
			record.m_material = m_material;
			return true;
		}

		temp = (-b + sqrt(b*b - a*c)) / a;

		if(temp < parameter_max && temp > parameter_min)
		{
			record.m_propagation_parameter = temp;
			record.m_hitpoint = r.propagate(temp);
			record.m_normal = (record.m_hitpoint - translated_center) / m_radius;
			record.m_material = m_material;
			return true;
		}
	}

	return false;
}

