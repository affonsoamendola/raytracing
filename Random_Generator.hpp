#pragma once

#include <random>

#include "Vector3f.hpp"

std::random_device g_rd;
std::mt19937 g_gen(g_rd());
std::uniform_real_distribution<float> g_dist(0.0f, 1.0f);

inline float randf()
{
	return g_dist(g_gen);
}

Vector3f random_unit_sphere_point()
{
	Vector3f p;
	do
	{
		p = 2.0f * Vector3f(randf(), randf(), randf()) - Vector3f(1., 1., 1.);
	}while (p.squared_length() >= 1.);

	return p;
}