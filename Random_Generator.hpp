#pragma once

#include <random>

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<float> dist(0.0f, 1.0f);

inline float randf()
{
	return dist(gen);
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