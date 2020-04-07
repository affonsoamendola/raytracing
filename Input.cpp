/*  
  Copyright Affonso Amendola 2019

  Fofonso's SDL Engine

  This is part of my SDL Game/Software Engine,

  Distributed under GPLv3, use it to your hearts content,
  just remember the number one rule:

  Be Excellent to Each Other.
*/
#include "SDL2/SDL.h"

#include "Input.hpp"

//INPUT SYSTEM CLASS MEMBER FUNCTIONS:
//Creates Input Subsystem.
Input::Input()
{}

//Called once every frame, updates key states, and does polling on keyboard events.
void Input::update()
{
	memset(m_key_down, 0, 284);
	memset(m_key_up, 0, 284);

	this->m_keyboard_state 	=	SDL_GetKeyboardState(NULL); 
	this->m_mouse_state 	= 	SDL_GetMouseState(&this->m_mouse_x, &this->m_mouse_y);

	while(SDL_PollEvent(&(this->m_event)))
	{
		if(	this->m_event.type == SDL_KEYDOWN)
		{
			if(m_event.key.repeat == 0) m_key_down[m_event.key.keysym.scancode] = true;

			E_KeyDown(m_event.key.keysym.scancode);
		}

		if(	this->m_event.type == SDL_KEYUP)
		{
			if(m_event.key.repeat == 0) m_key_up[m_event.key.keysym.scancode] = true;

			E_KeyUp(m_event.key.keysym.scancode);
		}

		if(	(this->m_event.type == SDL_QUIT) ||
			(this->m_event.type == SDL_KEYDOWN && this->m_event.key.keysym.scancode == SDL_SCANCODE_ESCAPE))
		{
			E_Quit();
			break;
		}
	}
}
//-------------------------