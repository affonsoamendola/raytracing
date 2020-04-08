#pragma once

#include <vector>

#include "SDL2/SDL.h"
#include "SDL2/SDL_render.h"
#include "SDL2/SDL_image.h"

#include "Color.hpp"

class Graphics
{
private:
	std::vector<unsigned char> m_screen_pixels; //APPARENTLY stored in BGRA, take that into account always
public:
	const unsigned int m_screen_width = 320;
	const unsigned int m_screen_height = 200;

	const unsigned int m_pixel_scale = 4; 	//When rendering, will draw a square of m_pixel_scale by m_pixel_scale instead of 1x1 pixels
											//Basically zooms in on pixel art and is pixel perfect, side effect, 320x200 game will be on a MUCH bigger screen.	

	const float m_aspect_ratio = static_cast<float>(m_screen_width) / static_cast<float>(m_screen_height);

	const unsigned int m_screen_surface_width = m_screen_width * m_pixel_scale;
	const unsigned int m_screen_surface_height = m_screen_height * m_pixel_scale;

	bool m_show_mouse = true;

	SDL_Window* m_window; //SDL window of the game.
	SDL_Renderer* m_renderer; //SDL renderer of the game.

	SDL_Texture* m_screen_surface;	//Texture holding the FINAL screen surface. (Dont mess with this.)

	Graphics();
	~Graphics();

	void update();
	void render();

	void set_pixel(unsigned int x, unsigned int y, Color color);
	void get_pixel(unsigned int x, unsigned int y);

	void clear_screen();
	void clear_screen(Color color);
};

void E_Draw();
