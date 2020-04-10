
#include "Font.hpp"
#include "Texture.hpp"

class SDL_Renderer;

Font::Font(	const char* file_location, const int size_x, const int size_y, SDL_Renderer* renderer)
{
  	m_file_location = file_location;
	m_size_x = size_x;
	m_size_y = size_y;
	m_texture = new Texture(m_file_location, renderer);
}

Font::Font(const Font& font_)
{
	m_file_location = font_.m_file_location;
	m_size_x = font_.m_size_x;
	m_size_y = font_.m_size_y;
	m_texture = new Texture(*font_.m_texture);
}

Font::~Font()
{
	delete m_texture;
}