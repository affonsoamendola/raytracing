#include "SDL2/SDL.h"

#include "Input.hpp"

//INPUT SYSTEM CLASS MEMBER FUNCTIONS:
//Creates Input Subsystem.
Input::Input()
{
	SDL_SetRelativeMouseMode(SDL_TRUE);
	memset(m_key_down, 0, 284);
}

//Called once every frame, updates key states, and does polling on keyboard events.
void Input::update()
{
	m_keyboard_state 	=	SDL_GetKeyboardState(NULL); 
	m_mouse_state 	= 	SDL_GetMouseState(&m_mouse_x, &m_mouse_y);

	while(SDL_PollEvent(&(m_event)))
	{
		if( m_event.type == SDL_MOUSEMOTION )
		{
			e_on_mouse_movement(m_event.motion.xrel, m_event.motion.yrel);
		}

		if(	m_event.type == SDL_KEYDOWN )
		{
			m_key_down[m_event.key.keysym.scancode] = true;

			if(m_event.key.repeat == 0) e_on_key_down(m_event.key.keysym.scancode);
		}

		if(	m_event.type == SDL_KEYUP )
		{
			m_key_down[m_event.key.keysym.scancode] = false;

			e_on_key_up(m_event.key.keysym.scancode);
		}

		if(	(m_event.type == SDL_QUIT ) ||
			(m_event.type == SDL_KEYDOWN && m_event.key.keysym.scancode == SDL_SCANCODE_ESCAPE )    )
		{
			e_quit();
			break;
		}
	}

	e_on_key_held(m_key_down);
}
//-------------------------