#pragma once

#include <math.h>

class Camera
{
public:
	Camera(float vfov, float aspect_ratio) 
	{
		float theta = vfov * M_PI / 180.;

		float half_height = tan(theta / 2.);
		float half_width = aspect_ratio * half_height;

		m_top_left_corner = Vector3f(-half_width, half_height, -1.);
		m_horizontal_size = Vector3f(2 * half_width, 0., 0.);
		m_vertical_size = Vector3f(0.0, 2 * half_height, 0.0);

		m_position = Vector3f(0., 0., 1.);
		m_origin = Vector3f(0., 0., 0.);
		m_rotation = Vector3f(0.0, 0.0, 0.);
	}

	Ray Ray_Cast(float u, float v)
	{
		return Ray(m_origin, m_top_left_corner + u * m_horizontal_size - v * m_vertical_size - m_origin);
	}

	Vector3f m_origin;
	Vector3f m_position;

	Vector3f m_rotation;

	Vector3f m_top_left_corner;
	Vector3f m_horizontal_size;
	Vector3f m_vertical_size;
};