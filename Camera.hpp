#pragma once

#include <cmath>

#include "Ray.hpp"
#include "Vector3f.hpp"

class Camera
{
public:
	Vector3f m_position;
	Vector3f m_origin;
	Vector3f m_rotation;

	Camera(float vfov, float aspect_ratio)
		: m_position(0., 0., 1.), m_origin(0., 0., 0.), m_rotation(0., 0., 0.) //Inits camera position and rotation
	{
		float theta = vfov * M_PI / 180.;

		float half_height = tan(theta / 2.);
		float half_width = aspect_ratio * half_height;	//Some trigonometry to figure out the size and location of the near clipping plane.

		m_top_left_corner = Vector3f(-half_width, half_height, -1.);
		m_horizontal_size = Vector3f(2 * half_width, 0., 0.);
		m_vertical_size = Vector3f(0.0, 2 * half_height, 0.0);
	}

	Ray ray_cast(float u, float v)	//Cast a ray from the camera origin to the uv coordinates on the "near plane".
	{
		return Ray(	m_origin, 
					m_top_left_corner + u * m_horizontal_size - v * m_vertical_size - m_origin);
	}

	Vector3f m_top_left_corner;
	Vector3f m_horizontal_size;
	Vector3f m_vertical_size;
};