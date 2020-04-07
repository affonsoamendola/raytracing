#pragma once

#include <vector>

#include "SDL2/SDL.h"
#include "SDL2/SDL_render.h"
#include "SDL2/SDL_image.h"

class Color
{
public:
	unsigned char r = 0;
	unsigned char g = 0;
	unsigned char b = 0;
	unsigned char a = 255;

	Color(unsigned char t_r = 0, unsigned char t_g = 0, unsigned char t_b = 0) : r(t_r), g(t_g), b(t_b) {}
	Color(unsigned char t_r, unsigned char t_g, unsigned char t_b, unsigned char t_a) : r(t_r), g(t_g), b(t_b), a(t_a) {}
};

//Some default colors
const Color COLOR_RED = {255, 0, 0, 255};
const Color COLOR_GREEN = {0, 255, 0, 255};
const Color COLOR_BLUE = {0, 0, 255, 255};
const Color COLOR_BLACK = {0, 0, 0, 255};
const Color COLOR_WHITE = {255, 255, 255, 255};

class Graphics
{
private:
	std::vector<unsigned char> m_screen_pixels; //APPARENTLY stored in BGRA, take that into account always
public:
	const unsigned int m_screen_width = 320;
	const unsigned int m_screen_height = 200;

	const unsigned int m_pixel_scale = 4; 	//When rendering, will draw a square of m_pixel_scale by m_pixel_scale instead of 1x1 pixels
											//Basically zooms in on pixel art and is pixel perfect, side effect, 320x200 game will be on a MUCH bigger screen.	

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
