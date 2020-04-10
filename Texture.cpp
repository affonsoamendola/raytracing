#include "SDL2/SDL.h"
#include "SDL2/SDL_render.h"
#include "SDL2/SDL_image.h"

#include "Texture.hpp"

Texture::Texture(const Texture& _texture)
{
	m_color_mod = _texture.m_color_mod; //Color and alpha tint of the texture.

	m_renderer = _texture.m_renderer;

	m_surface = SDL_ConvertSurface(_texture.m_surface, _texture.m_surface->format, 0); //Pixel surface of the Texture.
	m_texture = SDL_CreateTextureFromSurface(m_renderer, m_surface); //The actual texture.

	m_x = _texture.m_x;
	m_y = _texture.m_y;
	m_size_x = _texture.m_size_x;
	m_size_y = _texture.m_size_y;
}

Texture::Texture(const char* file_location, SDL_Renderer* renderer)
{
	m_renderer = renderer;

	//Load texture
	m_surface = IMG_Load(file_location);

	//Error handling
	if(m_surface == nullptr)
	{
		SDL_Log("\nFailure opening image file: %s", SDL_GetError());
		exit(1);
	}

	//Make a texture from surface, basically sends it to the GPU (I think?)
	m_texture = SDL_CreateTextureFromSurface(renderer, m_surface);

	//Error handling
	if(m_texture == nullptr)
	{
		SDL_Log("\nFailure creating texture: %s", SDL_GetError());
		exit(1);
	}
	//Define the bounds of this texture, from the surface bounds.
	m_x = 0;
	m_y = 0;
	m_size_x = m_surface->w;
	m_size_y = m_surface->h;

	//Basic setups for SDL texture for color and alpha mods and blendin, if the system supports it.
	//If the system doesnt support it, things will go very wrong.
	if(	SDL_SetTextureColorMod(m_texture, 255, 255, 255) == -1) 	SDL_Log("\nTexture Color Modulation Not Supported on this renderer");
	if(	SDL_SetTextureAlphaMod(m_texture, 255) == -1) 				SDL_Log("\nTexture Alpha Modulation Not Supported on this renderer");
		SDL_SetTextureBlendMode(m_texture, SDL_BLENDMODE_BLEND);
}

//Destroy texture.
Texture::~Texture()
{
	//Frees the surface that was in heap.
	SDL_FreeSurface(m_surface);
}
