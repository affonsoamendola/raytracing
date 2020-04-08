#pragma once

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
