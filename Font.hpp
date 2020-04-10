#pragma once

class Texture;
class SDL_Renderer;

//Defines what is a font, which is a collection of a texture and a size of character.
class Font
{
public:
	const char* m_file_location;
	Texture* m_texture;
	int m_size_x;
	int m_size_y;

	Font(const char *, int size_x, int size_y, SDL_Renderer* renderer);
	Font(const Font& font_);
	~Font();
};