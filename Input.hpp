#pragma once

#include <memory>

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

void E_KeyDown(int scancode);
void E_KeyUp(int scancode);
void E_Quit();

