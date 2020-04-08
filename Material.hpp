#pragma once

#include "Hitable.hpp"
#include "Random_Generator.hpp"

inline Vector3f Reflect(const Vector3f& v, const Vector3f& n)
{
	return v - 2*(dot(v, n) * n);
}

inline bool Refract(const Vector3f& v, const Vector3f& n, float ni_over_nt, Vector3f& refracted)
{
	Vector3f uv = v.unit();

	float dt = dot(uv, n);

	float discriminant = 1.0 - ni_over_nt*ni_over_nt * (1.-dt*dt);

	if(discriminant > 0.)
	{
		refracted = ni_over_nt*(uv - n*dt) - n* sqrt(discriminant);
		return true;
	}
	else return false;
}

inline float Schlick(float cosine, float refraction_index)
{
	float r0 = (1. - refraction_index) / (1. + refraction_index);
	r0 = r0*r0;

	return r0 + (1. - r0) * pow((1 - cosine), 5);
}

class Material
{
public:
	virtual bool Scatter(const Ray& ray_in, const Hit_Record& record, Vector3f& attenuation, Ray& ray_out) const = 0;	
};

class Lambertian : public Material
{
public:
	Lambertian(const Vector3f& a) : m_albedo(a) {}

	virtual bool Scatter(const Ray& ray_in, const Hit_Record& record, Vector3f& attenuation, Ray& ray_out) const
	{
		Vector3f target = record.m_hitpoint + record.m_normal + random_unit_sphere_point();
		ray_out = Ray(record.m_hitpoint, (target - record.m_hitpoint).unit());
		attenuation = m_albedo;
		return true;
	}

	Vector3f m_albedo;
};

class Metal : public Material
{
public:
	Metal(const Vector3f& a, float fuzziness) : m_albedo(a), m_fuzziness(fuzziness) {}

	virtual bool Scatter(const Ray& ray_in, const Hit_Record& record, Vector3f& attenuation, Ray& ray_out) const
	{
		Vector3f reflected = Reflect(ray_in.direction().unit(), record.m_normal);
		ray_out = Ray(record.m_hitpoint, reflected + m_fuzziness * random_unit_sphere_point());
		attenuation = m_albedo;

		return (dot(ray_out.direction(), record.m_normal) > 0);
	}

	Vector3f m_albedo;
	float m_fuzziness;
};

class Dieletric : public Material
{
public:
	Dieletric(float refraction_index) : m_refraction_index(refraction_index) {}

	virtual bool Scatter(const Ray& ray_in, const Hit_Record& record, Vector3f& attenuation, Ray& ray_out) const
	{
		Vector3f outward_normal;
		Vector3f reflected = Reflect(ray_in.direction(), record.m_normal);

		float ni_over_nt;

		attenuation = Vector3f(1.0, 1.0, 1.0);

		Vector3f refracted;

		float reflect_prob;
		float cosine;

		if(dot(ray_in.direction(), record.m_normal) > 0.)
		{
			outward_normal = -record.m_normal;
			ni_over_nt = m_refraction_index;
			cosine = m_refraction_index * dot(ray_in.direction(), record.m_normal) / ray_in.direction().length();
		}
		else
		{
			outward_normal = record.m_normal;
			ni_over_nt = 1.0 / m_refraction_index;
			cosine = -dot(ray_in.direction(), record.m_normal) / ray_in.direction().length();
		}

		if(Refract(ray_in.direction(), outward_normal, ni_over_nt, refracted))
		{
			reflect_prob = Schlick(cosine, m_refraction_index);
		}
		else
		{
			ray_out = Ray(record.m_hitpoint, reflected.unit());
			reflect_prob = 1.0f;
		}

		if(randf() < reflect_prob)
		{
			ray_out = Ray(record.m_hitpoint, reflected.unit());
		}
		else
		{
			ray_out = Ray(record.m_hitpoint, refracted.unit());
		}

		return true;
	}

	float m_refraction_index;
};
