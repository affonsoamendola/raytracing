#include <iostream>
#include <cstdio>
#include <cstdarg>

#include "SDL2/SDL.h"
#include "SDL2/SDL_render.h"
#include "SDL2/SDL_image.h"

#include "Color.hpp"
#include "Texture.hpp"
#include "Texture.hpp"
#include "Font.hpp"

#include "Graphics.hpp"

Graphics::Graphics() 
{
	//Reserve the memory needed for the framebuffer  (NOT REALLY the framebuffer, but its the screen 
	//step before the actual screen, so its more like a pixel scaled version of the screen)
	//Can I acctually just write to the framebuffer directly with SDL?, To get some speed?
	m_screen_pixels.reserve(m_screen_width * m_screen_height * 4); 

	std::cout << "Initting SDL Video Subsystem..." << std::flush;

	//Start SDL
	if(SDL_InitSubSystem(SDL_INIT_VIDEO) != 0)
	{
		SDL_Log("\nProblem initializing SDL Video Subsystem: %s", SDL_GetError());
		exit(1);
	}

	std::cout << "Done." << std::endl;

	std::cout << "Initting SDL_Image..." << std::flush;

	//Start SDL_Image
	int imgFlags = IMG_INIT_PNG;
 	if( !( IMG_Init(imgFlags) & imgFlags) )
 	{
		SDL_Log("\nProblem initializing SDL_Image: %s", IMG_GetError());
		exit(1);		
 	}

	std::cout << "Done." << std::endl;
	
	std::cout << "Creating Window..." << std::flush;

	//Creates window, with size m_screen_surface width x height
	m_window = SDL_CreateWindow	(	"Fofonso's SDL Engine",
											SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
											m_screen_surface_width, m_screen_surface_height,
											SDL_WINDOW_SHOWN
										);

	//Error handling
	if(m_window == NULL)
	{
		SDL_Log("\nProblem creating SDL Window: %s", SDL_GetError());
		exit(1);
	}

	std::cout << "Done." << std::endl;

	//Creates renderer (with the currently available renderer, should be opengl most of the times on linux)
	m_renderer = SDL_CreateRenderer	( 	m_window,
												-1,
												SDL_RENDERER_ACCELERATED
											);

	std::cout << "Creating SDL Renderer..." << std::flush;

	//Error handling
	if(m_renderer == NULL)
	{
		SDL_Log("Problem creating SDL Renderer: %s", SDL_GetError());
		exit(1);
	}

	std::cout << "Done." << std::endl;

	SDL_RendererInfo info;
	SDL_GetRendererInfo(m_renderer, &info);

	std::cout << "Renderer Type: " << info.name << std::endl;

	//Creates screen texture.
	m_screen_surface = SDL_CreateTexture	(	m_renderer,
													SDL_PIXELFORMAT_ARGB8888,
													SDL_TEXTUREACCESS_STREAMING,
													m_screen_width, m_screen_height
												);
	SDL_SetTextureBlendMode(m_screen_surface, SDL_BLENDMODE_BLEND);

	//Hide system cursor
	SDL_ShowCursor(0);

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");

	m_debug_font = new Font("8x8Font.png", 8, 8, m_renderer);
	m_debug_font_tiny = new Font("4x6Font.png", 4, 6, m_renderer);
}

//Destroy Graphics subsystem.
Graphics::~Graphics()
{
	delete m_debug_font;
	delete m_debug_font_tiny;

	//Quit everything.
	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);
	IMG_Quit();
}

//update before rendering
void Graphics::update(){}

//Main render function, this clears the screen and draws things to it.
void Graphics::render()
{
	e_draw();

    //Shows on screen
    SDL_RenderPresent(m_renderer);

   	//Clears screen for next frame.
	SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(m_renderer);
	clear_screen();
}

void Graphics::update_screen_surface()
{
	//Flushes screen surface data to screen texture.
	//m_screen_pixels -> m_screen_surface
    SDL_UpdateTexture	(	m_screen_surface,
    						NULL,
    						&(m_screen_pixels[0]),
    						m_screen_width * 4
    					);

    //Sends texture to renderer
    SDL_RenderCopy(m_renderer, m_screen_surface, NULL, NULL);
}

//Sets a pixel on screen of Color color
void Graphics::set_pixel(unsigned int x, unsigned int y, Color color)
{
	//Sets a pixel on screen, does screen space clipping with this if.
	if(x < m_screen_width && y < m_screen_height)
	{
		const unsigned int offset = ( m_screen_width * 4 * y) + x * 4;

		m_screen_pixels[ offset + 0 ] = color.m_b;    // b
	    m_screen_pixels[ offset + 1 ] = color.m_g;    // g
	    m_screen_pixels[ offset + 2 ] = color.m_r;    // r
	    m_screen_pixels[ offset + 3 ] = color.m_a;    // a
	}
}

void Graphics::clear_screen()
{
	//Clears screen_pixels
	fill(m_screen_pixels.begin(), m_screen_pixels.begin() + m_screen_width * m_screen_height * 4, 0);
}

void Graphics::clear_screen(Color color)
{
	//Sets a screen of a determined color (kind of slow)
	for(unsigned int x = 0; x < m_screen_width; x ++)
	{
		for(unsigned int y = 0; y < m_screen_height; y ++)
		{
			set_pixel(x, y, color);
		}
	}
}

void Graphics::blit_texture(const Texture* to_render, 	const int src_x, const int src_y, const int src_w, const int src_h, 
														const int dst_x, const int dst_y)
{
	//Get source Rect
	SDL_Rect src_sdl;
	SDL_Rect dst_sdl;

	src_sdl.x = src_x;
	src_sdl.y = src_y;
	src_sdl.w = src_w;
	src_sdl.h = src_h;

	//Create the destination rect, scaling everything thats needed.
	dst_sdl.x = dst_x * m_pixel_scale;
	dst_sdl.y = dst_y * m_pixel_scale;
	dst_sdl.w = src_sdl.w * m_pixel_scale;
	dst_sdl.h = src_sdl.h * m_pixel_scale;

	//Does the actual blitting.
	SDL_RenderCopy(m_renderer, to_render->m_texture, &src_sdl, &dst_sdl);
}

void Graphics::draw_char(unsigned int x, unsigned int y, Font* font, Color color, char character)
{
	unsigned int offset = (unsigned int)((character-32));

	unsigned int tex_x = offset % 32;
	unsigned int tex_y = offset / 32;

	Color original_mod = font->m_texture->get_color_mod();

	font->m_texture->set_color_mod(color);

	blit_texture(	font->m_texture, 
					tex_x * font->m_size_x, tex_y * font->m_size_y, font->m_size_x, font->m_size_y,
					x, y);

	font->m_texture->set_color_mod(original_mod);
}

void Graphics::draw_string(unsigned int x, unsigned int y, Font* font, Color color, const char * char_string, ...)
{
	char buffer[256] = {0};
	va_list args;
	va_start(args, char_string);
	
	vsnprintf(buffer, 256, char_string, args);

	va_end(args);

	for(int i = 0; i < 256 && buffer[i] != '\0'; i++)
	{
		draw_char(x + font->m_size_x * i, y, font, color, buffer[i]);
	}
}


