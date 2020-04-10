#pragma once

#include "SDL2/SDL.h"
#include "Color.hpp"

class Texture
{
private:

	Color m_color_mod = {255, 255, 255, 255}; //Color and alpha tint of the texture.

public:
	SDL_Surface* m_surface = nullptr; //Pixel surface of the Texture.
	SDL_Texture* m_texture = nullptr; //The actual texture.
	SDL_Renderer* m_renderer = nullptr;

	int m_x = 0;
	int m_y = 0;
	int m_size_x = 0;
	int m_size_y = 0;

	inline void set_opacity(unsigned char opacity)
	{
		m_color_mod.m_a = opacity;
		SDL_SetTextureAlphaMod(m_texture, m_color_mod.m_a);
	}
	inline double get_opacity() const { return m_color_mod.m_a; }

	inline void set_color_mod(Color color)
	{
		m_color_mod.m_r = color.m_r;
		m_color_mod.m_g = color.m_g;
		m_color_mod.m_b = color.m_b;
		SDL_SetTextureColorMod(m_texture, color.m_r, color.m_g, color.m_b);
	}
	inline Color get_color_mod() const { return m_color_mod; }

	Texture(const char* file_location, SDL_Renderer* renderer);
	Texture(const Texture& _texture);
	~Texture();
};