#include <iostream>

#include "Vector3f.hpp"
#include "Ray.hpp"

#include "Graphics.hpp"
#include "Input.hpp"

#include "Sphere.hpp"
#include "Hitable.hpp"
#include "Hitable_List.hpp"

#include "Material.hpp"

#include "Camera.hpp"

#include "Random_Generator.hpp"

Vector3f ray_trace(const Camera& cam, const Ray& r, Hitable_List* hitables, int depth = 0)
{
	Hit_Record hit_record;

	if(hitables->Hit(cam, r, 0.001, MAXFLOAT, hit_record))
	{		
		Ray scattered;
		Vector3f attenuation;

		if(depth < 50 && hit_record.m_material->Scatter(r, hit_record, attenuation, scattered))
		{
			return attenuation * ray_trace(cam, scattered, hitables, depth + 1);
		}
		else
		{
			return Vector3f(0., 0., 0.);
		}
	}
	else
	{
		float t = 0.5f*(r.direction().y() + 1.0f);
		return (1.0f - t) * Vector3f(1.0f, 1.0f, 1.0f) + t * Vector3f(0.5f, 0.7f, 1.0f);
	}
}

char is_running = 1;

Graphics graphics;
Input input;

Camera camera(45, graphics.m_aspect_ratio);

Hitable* hitable_list[4];
Hitable_List* world;

int main(int argc, char const *argv[])
{
	hitable_list[0] = new Sphere(  Vector3f(0., 0, -2.),			0.45f, 	new Lambertian(Vector3f(0.95, 0.1, 0.1)) );
	hitable_list[1] = new Sphere(  Vector3f(0., -100.5, -2.), 		100.f, 	new Lambertian(Vector3f(0.65, 0.9, 0.0)) );
	hitable_list[2] = new Sphere(  Vector3f(1., 0, -2.), 			0.45f, 	new Metal(Vector3f(0.8, 0.9, 0.9), 0.01) );
	hitable_list[3] = new Sphere(  Vector3f(-1., 0., -2.), 			0.45f, 	new Dieletric(1.2));
	
	world = new Hitable_List(hitable_list, 4);

	while(is_running)
	{
		input.update();
		graphics.render();
	}

	return 0;
}

int multi_sample = 64;

void E_Draw()
{
	for(int y = 0; y < graphics.m_screen_height; y++)
	{
		for(int x = 0; x < graphics.m_screen_width; x++)
		{
			Vector3f color(0., 0., 0.);

			for(int s = 0; s < multi_sample; s++)
			{
				float u = (float(x) + (randf())) / float(graphics.m_screen_width);
				float v = (float(y) + (randf())) / float(graphics.m_screen_height);

				Ray r(camera.m_origin, (camera.m_top_left_corner + u * camera.m_horizontal_size - v * camera.m_vertical_size).unit());
				color += ray_trace(camera, r, world);
			}
			color /= multi_sample;
			
			Vector3f gamma_corrected_color = Vector3f(sqrt(color[0]), sqrt(color[1]), sqrt(color[2]));

			graphics.set_pixel(x, y, Vector3f_to_Color(gamma_corrected_color, 255));
		}
	}
}

void E_KeyDown(int scancode)
{
	switch(scancode)
	{
		case SDL_SCANCODE_D:
			camera.m_position += Vector3f(0.05, 0., 0.);
			break;

		case SDL_SCANCODE_A:
			camera.m_position += Vector3f(-0.05, 0., 0.);
			break;

		case SDL_SCANCODE_W:
			camera.m_position += Vector3f(0., 0., -0.05);
			break;

		case SDL_SCANCODE_S:
			camera.m_position += Vector3f(0., 0., 0.05);
			break;

		case SDL_SCANCODE_U:
			camera.m_rotation += Vector3f(0.05, 0., 0.);
			break;

		case SDL_SCANCODE_J:
			camera.m_rotation += Vector3f(-0.05, 0., 0.);
			break;

		case SDL_SCANCODE_K:
			camera.m_rotation += Vector3f(0., -0.05, 0.);
			break;

		case SDL_SCANCODE_H:
			camera.m_rotation += Vector3f(0., 0.05, 0.);
			break;
	}
}

void E_KeyUp(int scancode){}

void E_Quit()
{
	is_running = 0;
}