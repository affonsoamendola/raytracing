#pragma once

#include "Vector3f.hpp"
#include "Material.hpp"
#include "Ray.hpp"
#include "Hitable_List.hpp"

Vector3f ray_trace(const Ray& r, Hitable_List* hitables, int depth = 0)
{
	Hit_Record hit_record;

	if(hitables->Hit(r, 0.001, MAXFLOAT, hit_record))
	{		
		Ray scattered;
		Vector3f attenuation;

		if(depth < 50)
		{
			Vector3f color(0., 0., 0.);

			if (hit_record.m_material->Scatter(r, hit_record, attenuation, scattered))
			{
				color = ray_trace(scattered, hitables, depth + 1);
			}
			
			return attenuation * color;
		}
		else
		{
			return Vector3f(0., 0., 0.);
		}
	}
	else
	{
		float t = 0.5f*(r.m_direction.y() + 1.0f);
		return (1.0f - t) * Vector3f(1.0f, 1.0f, 1.0f) + t * Vector3f(0.5f, 0.7f, 1.0f);
	}
}
