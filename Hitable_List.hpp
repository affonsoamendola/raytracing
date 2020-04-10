#pragma once

#include "Hitable.hpp"
#include "Camera.hpp"

class Hitable_List : public Hitable
{
public:
	Hitable_List() {}

	Hitable_List(Hitable **l, int n) { m_list = l; m_list_length = n; }

	virtual bool Hit(const Ray& r, float parameter_min, float parameter_max, Hit_Record& record) const;

	void update_local_positions(const Camera& cam);

	Hitable** m_list;
	int m_list_length;
};

void Hitable_List::update_local_positions(const Camera& cam)
{
	for (int i = 0; i < m_list_length; i++)
	{
		m_list[i]->m_local_position = Translate(m_list[i]->m_world_position, -cam.m_position);
		m_list[i]->m_local_position = RotateY(m_list[i]->m_local_position, -cam.m_rotation.v[1]);
		m_list[i]->m_local_position = RotateX(m_list[i]->m_local_position, -cam.m_rotation.v[0]);
	}
}

bool Hitable_List::Hit(const Ray& r, float parameter_min, float parameter_max, Hit_Record& record) const
{
	Hit_Record current_record;
	bool hit_anything = false;
	double closest_so_far = parameter_max;

	for (int i = 0; i < m_list_length; i++)
	{
		if(m_list[i]->Hit(r, parameter_min, parameter_max, current_record))
		{
			if(current_record.m_propagation_parameter < closest_so_far)
			{
				hit_anything = true;
				closest_so_far = current_record.m_propagation_parameter;
				record = current_record;
			}
		}
	}

	return hit_anything;
}