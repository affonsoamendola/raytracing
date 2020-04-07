#include <iostream>

#include "Vector3f.hpp"
#include "Ray.hpp"

#include "Graphics.hpp"
#include "Input.hpp"

bool hit_sphere(const Vector3f& center, float radius, const Ray& r)
{
	Vector3f oc = r.origin() - center;

	float a = dot(r.direction(), r.direction());
	float b = 2.0f * dot(oc, r.direction());
	float c = dot(oc, oc) - radius *  radius;
	return ((b*b - 4*a*c) > 0);
}

Vector3f color_gradient(const Ray& r)
{
	if(hit_sphere(Vector3f(0., 0., -1.),  0.5, r)) return Vector3f(1, 0, 0);

	float t = 0.5f*(r.direction().y() + 1.0f);
	return (1.0f - t) * Vector3f(1.0f, 1.0f, 1.0f) + t * Vector3f(0.5f, 0.7f, 1.0f);
}

inline Color Vector3f_to_Color (const Vector3f& t, const unsigned char a)
{
	return Color(	static_cast<unsigned char>(t[0] * 255.0f), 
					static_cast<unsigned char>(t[1] * 255.0f),
					static_cast<unsigned char>(t[2] * 255.0f),
					a);
} 

char is_running = 1;

Graphics graphics;
Input input;

int main(int argc, char const *argv[])
{
	while(is_running)
	{
		input.update();
		graphics.render();
	}

	return 0;
}

int nx = 320;
int ny = 200;

Vector3f top_left_corner(-3.2, 2.0, -1.0);
Vector3f horizontal(6.4, 0.0, 0.0);
Vector3f vertical(0.0, 4.0, 0.0);

Vector3f origin(0.0, 0.0, 0.0);

void E_Draw()
{
	for(int y = 0; y < ny; y++)
	{
		for(int x = 0; x < nx; x++)
		{
			float u = float(x) / float(nx);
			float v = float(y) / float(ny);

			Ray r(origin, (top_left_corner + u * horizontal - v * vertical).unit());
			Vector3f col = color_gradient(r);

			graphics.set_pixel(x, y, Vector3f_to_Color(col, 255));
		}
	}
}

void E_KeyDown(int scancode){}
void E_KeyUp(int scancode){}

void E_Quit()
{
	is_running = 0;
}