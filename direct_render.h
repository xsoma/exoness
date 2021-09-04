#pragma once

#include "singleton.h"
#include <array>
#include "vector_2d.h"
#include "i_surface.h"
#include "vector.h"

#include <d3d9.h>
#include <d3dx9.h>

struct vertice_t
{
	float x, y, z, rhw;
	int c;
};

class d3d_render
{
public:

	d3d_render(IDirect3DDevice9* Device)
	{
		this->device = Device;
		this->vertex_shader = vertex_shader;

		Device->GetViewport(&screen_size);
	}

	void setup_render_states();
	void save_state();
	void restore_state();

	int get_text_width(const char* text, LPD3DXFONT font);
	void text(float x, float y, LPD3DXFONT font, std::string text, Color color, bool centered = false, int drop_shadow = 0, Color shadow_color = Color());
	void vertical_gradient(int x, int y, int w, int h, Color c1, Color c2);
	void rect(Vector2D a, Vector2D b, Color c);
	void rect_(float x, float y, float w, float h, Color c);
	void rect_fill(Vector2D a, Vector2D b, Color c);
	void draw_filled_triangle(Vector2D p1, Vector2D p2, Vector2D p3, Color c);
	void rect_fill_(float x, float y, float w, float h, Color c);
	void gradient_v(Vector2D a, Vector2D b, Color c_a, Color c_b);
	void gradient_v_(float x, float y, float w, float h, Color c_a, Color c_b);
	void gradient_h(Vector2D a, Vector2D b, Color c_a, Color c_b);
	void gradient_h_(float x, float y, float w, float h, Color c_a, Color c_b);
	void line(Vector2D a, Vector2D b, Color c);
	void line_(float x, float y, float x1, float x2, Color c);
	RECT text_size_to_rect(LPD3DXFONT font, const char* text);
	void horizontal_gradient(int x, int y, int w, int h, Color c1, Color c2);
	void vertical_gradient_2(int x, int y, int w, int h, Color c1, Color c2);

	LPD3DXFONT main_font;
	LPD3DXFONT pixel_font;
	LPD3DXFONT gang;
	LPD3DXFONT console_font;
	LPD3DXFONT visualz_font;

	//int get_text_width(const char* text, LPD3DXFONT font);
	//void text(float x, float y, LPD3DXFONT font, std::string text, D3DCOLOR color, bool centered = false, bool drop_shadow = false, D3DCOLOR shadow_color = D3DCOLOR());

	IDirect3DVertexShader9* vertex_shader;
	IDirect3DDevice9* device;
	IDirect3DVertexDeclaration9* vertext_declaration;
	IDirect3DVertexShader9* vertex_shader2;
	IDirect3DPixelShader9* pixel_shader;
	DWORD dwold_d3drs_colorwriteenable;
	DWORD dwold_d3dtexturestagestate;
	DWORD dwold_d3drs_antialiasedlineenable;
	DWORD dwold_d3drs_multisampleantialias;
	RECT rect_old_scissor_rect;
	D3DVIEWPORT9 screen_size;

}; extern d3d_render* setuped;