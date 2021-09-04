#pragma once

#include "singleton.h"
#include <array>
#include "vector_2d.h"
#include "i_surface.h"
#include "vector.h"

#include <d3d9.h>
#include <d3dx9.h>

class render : public singleton< render > {

public:

	bool initalized = false;
	IDirect3DDevice9* m_device;
	D3DVIEWPORT9 m_viewport;

	void invalidate_objects();
	void restore_objects(LPDIRECT3DDEVICE9 m_device);
	void setup_states() const;
	void initalize_objects(LPDIRECT3DDEVICE9 m_device);

public:

	void draw_text(int X, int Y, Color Color, int Font, bool Center, const char* _Input, ...);
	void outlined_rect(int x, int y, int w, int h, Color color);
	void filled_rect(int x, int y, int w, int h, Color color);
	void line(int x, int y, int x2, int y2, Color color);

	void circle_3d(Vector position, float points, float radius, Color color);

	void simple_vertical_gradient(int x, int y, int w, int h, Color c1, Color c2);
	void simple_horizontal_gradient(int x, int y, int w, int h, Color c1, Color c2);
	void simple_vertical_gradient_2(int x, int y, int w, int h, Color c1, Color c2);

	void add_textured_polygon(int n, Vertex_t* vertice, int r, int g, int b, int a);
	void draw_filled_triangle(std::array< Vector2D, 3 >points, Color colour);

	Vector2D get_mouse_pos();
	RECT get_text_size_rect(DWORD font, const char* text);
	bool mouse_in_region(int x, int y, int x2, int y2);

public:

	DWORD alternative_pixel_font;
	DWORD cool_font;
	DWORD small_pixel_font;
	DWORD small_font;
	DWORD no_shadow;

	DWORD visuals_font;
	DWORD visuals_font_name;
	DWORD console_font;
};
