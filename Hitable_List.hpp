#pragma once

#include "Hitable.hpp"

class Hitable_List : public Hitable
{
public:
	Hitable_List() {}

	Hitable_List(Hitable **l, int n) { m_list = l; m_list_length = n; }

	virtual bool Hit(const Camera& cam, const Ray& r, float parameter_min, float parameter_max, Hit_Record& record) const;

	Hitable** m_list;
	int m_list_length;
};

bool Hitable_List::Hit(const Camera& cam, const Ray& r, float parameter_min, float parameter_max, Hit_Record& record) const
{
	Hit_Record current_record;
	bool hit_anything = false;
	double closest_so_far = parameter_max;

	for (int i = 0; i < m_list_length; i++)
	{
		if(m_list[i]->Hit(cam, r, parameter_min, parameter_max, current_record))
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