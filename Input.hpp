#pragma once

#include "SDL2/SDL.h"

class Engine;

class Input
{
private:
	SDL_Event m_event;

public:
	const unsigned char * m_keyboard_state;
	
	bool m_key_down[284];
	bool m_key_up[284];

	int m_mouse_state;

	int m_mouse_x;
	int m_mouse_y;

	Input();

	void update();
};

void e_on_mouse_movement(int mouse_x, int mouse_y);
void e_on_key_held(const bool keys_down[284]);
void e_on_key_down(int scancode);
void e_on_key_up(int scancode);
void e_quit();

