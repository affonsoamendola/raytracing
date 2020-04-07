#include <iostream>

#include "SDL2/SDL.h"
#include "SDL2/SDL_render.h"
#include "SDL2/SDL_image.h"

#include "Graphics.hpp"

Graphics::Graphics() 
{
	//Reserve the memory needed for the framebuffer  (NOT REALLY the framebuffer, but its the screen 
	//step before the actual screen, so its more like a pixel scaled version of the screen)
	//Can I acctually just write to the framebuffer directly with SDL?, To get some speed?
	this->m_screen_pixels.reserve(this->m_screen_width * this->m_screen_height * 4); 

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
	this->m_window = SDL_CreateWindow	(	"Fofonso's SDL Engine",
											SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
											this->m_screen_surface_width, this->m_screen_surface_height,
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
	this->m_renderer = SDL_CreateRenderer	( 	this->m_window,
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
	SDL_GetRendererInfo(this->m_renderer, &info);

	std::cout << "Renderer Type: " << info.name << std::endl;

	//Creates screen texture.
	this->m_screen_surface = SDL_CreateTexture	(	this->m_renderer,
													SDL_PIXELFORMAT_ARGB8888,
													SDL_TEXTUREACCESS_STREAMING,
													this->m_screen_width, this->m_screen_height
												);
	SDL_SetTextureBlendMode(this->m_screen_surface, SDL_BLENDMODE_BLEND);

	//Hide system cursor
	SDL_ShowCursor(0);
}

//Destroy Graphics subsystem.
Graphics::~Graphics()
{
	//Quit everything.
	SDL_DestroyRenderer(this->m_renderer);
	SDL_DestroyWindow(this->m_window);
	IMG_Quit();
}

//update before rendering
void Graphics::update(){};

//Main render function, this clears the screen and draws things to it.
void Graphics::render()
{
	E_Draw();

	//Flushes screen surface data to screen texture.
	//m_screen_pixels -> m_screen_surface
    SDL_UpdateTexture	(	this->m_screen_surface,
    						NULL,
    						&(this->m_screen_pixels[0]),
    						this->m_screen_width * 4
    					);

    //Sends texture to renderer
    SDL_RenderCopy(this->m_renderer, this->m_screen_surface, NULL, NULL);

    //Shows on screen
    SDL_RenderPresent(this->m_renderer);

   	//Clears screen for next frame.
	SDL_SetRenderDrawColor(this->m_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(this->m_renderer);
	this->clear_screen();
}

//Sets a pixel on screen of Color color
void Graphics::set_pixel(unsigned int x, unsigned int y, Color color)
{
	//Sets a pixel on screen, does screen space clipping with this if.
	if(x < this->m_screen_width && y < this->m_screen_height && x >= 0 && y >= 0)
	{
		const unsigned int offset = ( this->m_screen_width * 4 * y) + x * 4;

		this->m_screen_pixels[ offset + 0 ] = color.b;    // b
	    this->m_screen_pixels[ offset + 1 ] = color.g;    // g
	    this->m_screen_pixels[ offset + 2 ] = color.r;    // r
	    this->m_screen_pixels[ offset + 3 ] = color.a;    // a
	}
}

void Graphics::clear_screen()
{
	//Clears screen_pixels
	fill(this->m_screen_pixels.begin(), this->m_screen_pixels.begin() + this->m_screen_width * this->m_screen_height * 4, 0);
}

void Graphics::clear_screen(Color color)
{
	//Sets a screen of a determined color (kind of slow)
	for(int x = 0; x < this->m_screen_width; x ++)
	{
		for(int y = 0; y < this->m_screen_height; y ++)
		{
			this->set_pixel(x, y, color);
		}
	}
}