#pragma once

class Color
{
public:
	unsigned char m_r = 0;
	unsigned char m_g = 0;
	unsigned char m_b = 0;
	unsigned char m_a = 255;

	Color(unsigned char r = 0, unsigned char g = 0, unsigned char b = 0) 
		: m_r(r), m_g(g), m_b(b) {}
		
	Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a) 
		: m_r(r), m_g(g), m_b(b), m_a(a) {}
};

//Some default colors
const Color COLOR_RED = {255, 0, 0, 255};
const Color COLOR_GREEN = {0, 255, 0, 255};
const Color COLOR_BLUE = {0, 0, 255, 255};
const Color COLOR_BLACK = {0, 0, 0, 255};
const Color COLOR_WHITE = {255, 255, 255, 255};
