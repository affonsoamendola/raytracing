#include <iostream>

#include "SDL2/SDL.h"

#include "Material.hpp"
#include "Random_Generator.hpp"
#include "Vector3f.hpp"
#include "Ray.hpp"
#include "Graphics.hpp"
#include "Camera.hpp"
#include "Input.hpp"
#include "Timer.hpp"
#include "Sphere.hpp"
#include "Hitable.hpp"
#include "Hitable_List.hpp"
#include "Raytrace.hpp"

bool is_running = 1;
bool show_help = 1;
int multi_sample_enabled = 1;
int multi_sample = 4;
int selected_sphere = 0;

bool mouse_movement = true;

Graphics graphics;
Input input;
Timer timer;
Camera camera(45, graphics.m_aspect_ratio);

Hitable* hitable_list[4];
Hitable_List* world;

int main()
{
	hitable_list[0] = new Sphere(  Vector3f(-1., 0., -2.), 			0.45f, 	new Dieletric(1.2));
	hitable_list[1] = new Sphere(  Vector3f(0., 0, -2.),			0.45f, 	new Lambertian(Vector3f(0.95, 0.1, 0.1)) );
	hitable_list[2] = new Sphere(  Vector3f(1., 0, -2.), 			0.45f, 	new Metal(Vector3f(0.8, 0.9, 0.9), 0.01) );
	hitable_list[3] = new Sphere(  Vector3f(0., -100.5, -2.), 		100.f, 	new Lambertian(Vector3f(0.65, 0.9, 0.0)) );
	
	world = new Hitable_List(hitable_list, 4);

	while(is_running)
	{
		timer.start_Timer();
		input.update();
		graphics.render();
		timer.stop_Timer();
	}

	return 0;
}

void e_draw()
{
	world->update_local_positions(camera);
	
	for(unsigned int y = 0; y < graphics.m_screen_height; y++)
	{
		for(unsigned int x = 0; x < graphics.m_screen_width; x++)
		{
			Vector3f color(0., 0., 0.);

			for(int s = 0; s <= ((multi_sample - 1) * multi_sample_enabled); s++)
			{
				float u = (float(x) + (randf() * multi_sample_enabled)) / float(graphics.m_screen_width);
				float v = (float(y) + (randf() * multi_sample_enabled)) / float(graphics.m_screen_height);

				Ray r(camera.m_origin, (camera.m_top_left_corner + u * camera.m_horizontal_size - v * camera.m_vertical_size).unit());
				color += ray_trace(r, world);
			}

			if(multi_sample_enabled) color /= multi_sample;

			Vector3f gamma_corrected_color = Vector3f(sqrt(color[0]), sqrt(color[1]), sqrt(color[2]));

			graphics.set_pixel(x, y, Vector3f_to_Color(gamma_corrected_color, 255));

		}
	}

	graphics.update_screen_surface();

	if(show_help)
	{
		graphics.draw_string(2, 2, graphics.m_debug_font_tiny, Color(255, 100, 0), "Foffonso's Magnificent Raytracing Experiment");
		graphics.draw_string(2, 9, graphics.m_debug_font_tiny, Color(255, 100, 0), "Copyright Affonso Amendola, 2020");
		graphics.draw_string(2, 16, graphics.m_debug_font_tiny, Color(255, 100, 0), "FPS = %f",	0.001/timer.delta_Time());
		
		graphics.draw_string(190, 2, graphics.m_debug_font_tiny, Color(255, 100, 0), "F1      : Show/Hide Info");
		graphics.draw_string(190, 9, graphics.m_debug_font_tiny, Color(255, 100, 0), "WASD    : Move Camera");
		graphics.draw_string(190, 16, graphics.m_debug_font_tiny, Color(255, 100, 0), "Mouse   : Rotate Camera");	
		graphics.draw_string(190, 23, graphics.m_debug_font_tiny, Color(255, 100, 0), "M/N     : Inc/Dec Rays Per Pixel");	
		graphics.draw_string(190, 30, graphics.m_debug_font_tiny, Color(255, 100, 0), "1,2,3,4 : Select Sphere");
		graphics.draw_string(190, 37, graphics.m_debug_font_tiny, Color(255, 100, 0), "1 = Glass 2 = Lambertian");		
		graphics.draw_string(190, 44, graphics.m_debug_font_tiny, Color(255, 100, 0), "3 = Metal 4 = Ground");	
		graphics.draw_string(190, 51, graphics.m_debug_font_tiny, Color(255, 100, 0), "V       : Toggle Multi-Sampling");	
		graphics.draw_string(190, 58, graphics.m_debug_font_tiny, Color(255, 100, 0), "B       : Toggle Mouse");	
		graphics.draw_string(190, 65, graphics.m_debug_font_tiny, Color(255, 100, 0), "Esc     : Quit @");	

		if(multi_sample_enabled) graphics.draw_string(2, 178, graphics.m_debug_font_tiny, Color(255, 100, 0), "Rays Per Pixel = %d", multi_sample);
		graphics.draw_string(2, 185, graphics.m_debug_font_tiny, Color(255, 100, 0), "X = %f", camera.m_position.v[0]);
		graphics.draw_string(2, 192, graphics.m_debug_font_tiny, Color(255, 100, 0), "Z = %f", camera.m_position.v[2]);

		if(selected_sphere == 0)
		{
			graphics.draw_string(190, 100, graphics.m_debug_font_tiny, Color(255, 100, 0), "Glass Sphere Selected");
			graphics.draw_string(190, 107, graphics.m_debug_font_tiny, Color(255, 100, 0), "R,T  : Inc/Dec Refraction Index");
			graphics.draw_string(190, 114, graphics.m_debug_font_tiny, Color(255, 100, 0), "R_I = %f", hitable_list[selected_sphere]->m_material->m_refraction_index);	
		}
		else
		if(selected_sphere == 1 || selected_sphere == 3)
		{
			graphics.draw_string(190, 100, graphics.m_debug_font_tiny, Color(255, 100, 0), "Lambertian Sphere Selected");
			graphics.draw_string(190, 107, graphics.m_debug_font_tiny, Color(255, 100, 0), "Y,U,P   : Inc RGB of Albedo");
			graphics.draw_string(190, 114, graphics.m_debug_font_tiny, Color(255, 100, 0), "J,K,L   : Dec RGB of Albedo");
			graphics.draw_string(190, 121, graphics.m_debug_font_tiny, Color(255, 100, 0), "R = %f", hitable_list[selected_sphere]->m_material->m_albedo.v[0]);	
			graphics.draw_string(190, 128, graphics.m_debug_font_tiny, Color(255, 100, 0), "G = %f", hitable_list[selected_sphere]->m_material->m_albedo.v[1]);	
			graphics.draw_string(190, 135, graphics.m_debug_font_tiny, Color(255, 100, 0), "B = %f", hitable_list[selected_sphere]->m_material->m_albedo.v[2]);	
		}
		else
		if(selected_sphere == 2)
		{
			graphics.draw_string(190, 100, graphics.m_debug_font_tiny, Color(255, 100, 0), "Metal Sphere Selected");
			graphics.draw_string(190, 107, graphics.m_debug_font_tiny, Color(255, 100, 0), "Y,U,P   : Inc RGB of Albedo");
			graphics.draw_string(190, 114, graphics.m_debug_font_tiny, Color(255, 100, 0), "J,K,L   : Dec RGB of Albedo");
			graphics.draw_string(190, 121, graphics.m_debug_font_tiny, Color(255, 100, 0), "R = %f", hitable_list[selected_sphere]->m_material->m_albedo.v[0]);	
			graphics.draw_string(190, 128, graphics.m_debug_font_tiny, Color(255, 100, 0), "G = %f", hitable_list[selected_sphere]->m_material->m_albedo.v[1]);	
			graphics.draw_string(190, 135, graphics.m_debug_font_tiny, Color(255, 100, 0), "B = %f", hitable_list[selected_sphere]->m_material->m_albedo.v[2]);	
			graphics.draw_string(190, 142, graphics.m_debug_font_tiny, Color(255, 100, 0), "O,P     : Inc/Dec Fuzzyness");
			graphics.draw_string(190, 149, graphics.m_debug_font_tiny, Color(255, 100, 0), "Fuzz = %f", hitable_list[selected_sphere]->m_material->m_fuzziness);	
		}
	}
}

void e_on_key_down(int scancode)
{
	switch(scancode)
	{
		case SDL_SCANCODE_M:
			multi_sample++;
			break;
		case SDL_SCANCODE_N:
			if(multi_sample > 1) multi_sample--;
			break;
		case SDL_SCANCODE_B:
			mouse_movement = !mouse_movement;
			break;

		case SDL_SCANCODE_V:
			multi_sample_enabled = !multi_sample_enabled;
			break;

		case SDL_SCANCODE_F1:
			show_help = !show_help;
			break;

		case SDL_SCANCODE_1:
			selected_sphere = 0;
			break;
		case SDL_SCANCODE_2:
			selected_sphere = 1;
			break;
		case SDL_SCANCODE_3:
			selected_sphere = 2;
			break;
		case SDL_SCANCODE_4:
			selected_sphere = 3;
			break;

		case SDL_SCANCODE_R:
			if(selected_sphere == 0)
			{
				hitable_list[selected_sphere]->m_material->m_refraction_index += 0.025;
			}
			break;

		case SDL_SCANCODE_T:
			if(selected_sphere == 0)
			{
				hitable_list[selected_sphere]->m_material->m_refraction_index -= 0.025;
			}
			break;

		case SDL_SCANCODE_O:
			if(selected_sphere == 2)
			{
				hitable_list[selected_sphere]->m_material->m_fuzziness += 0.01;
			}
			break;

		case SDL_SCANCODE_P:
			if(selected_sphere == 2)
			{
				hitable_list[selected_sphere]->m_material->m_fuzziness -= 0.01;
			}
			break;

		case SDL_SCANCODE_Y:
			if(selected_sphere != 0)
			{
				hitable_list[selected_sphere]->m_material->m_albedo += Vector3f(0.025, 0., 0.);
			}
			break;

		case SDL_SCANCODE_H:
			if(selected_sphere != 0)
			{
				hitable_list[selected_sphere]->m_material->m_albedo -= Vector3f(0.025, 0., 0.);
			}
			break;

		case SDL_SCANCODE_U:
			if(selected_sphere != 0)
			{
				hitable_list[selected_sphere]->m_material->m_albedo += Vector3f(0., 0.025, 0.);
			}
			break;

		case SDL_SCANCODE_J:
			if(selected_sphere != 0)
			{
				hitable_list[selected_sphere]->m_material->m_albedo -= Vector3f(0., 0.025, 0.);
			}
			break;

		case SDL_SCANCODE_I:
			if(selected_sphere != 0)
			{
				hitable_list[selected_sphere]->m_material->m_albedo += Vector3f(0., 0., 0.025);
			}
			break;

		case SDL_SCANCODE_K:
			if(selected_sphere != 0)
			{
				hitable_list[selected_sphere]->m_material->m_albedo -= Vector3f(0., 0., 0.025);
			}
			break;
	}
}

void e_on_key_up(int scancode) 
{}

void e_on_key_held(const bool keys_down[284]) 
{
	if(keys_down[SDL_SCANCODE_D])
	{
		camera.m_position += RotateY(Vector3f(3000., 0., 0.), camera.m_rotation.v[1]) * timer.delta_Time();
	}

	if(keys_down[SDL_SCANCODE_A])
	{
		camera.m_position += RotateY(Vector3f(-3000., 0., 0.), camera.m_rotation.v[1]) * timer.delta_Time();
	}

	if(keys_down[SDL_SCANCODE_W])
	{
		camera.m_position += RotateY(Vector3f(0., 0., -3000.), camera.m_rotation.v[1]) * timer.delta_Time();
	}

	if(keys_down[SDL_SCANCODE_S])
	{
		camera.m_position += RotateY(Vector3f(0., 0., 3000.), camera.m_rotation.v[1]) * timer.delta_Time();
	}
}

void e_on_mouse_movement(int mouse_x, int mouse_y)
{

	if(mouse_movement)	camera.m_rotation += Vector3f(-mouse_y * 70., mouse_x * 70., 0.) * timer.delta_Time();
}

void e_quit()
{

	is_running = 0;
}