#include "direct_render.h"

d3d_render* setuped = NULL;

void d3d_render::rect(Vector2D a, Vector2D b, Color c)
{
	b += a;

	b.x -= 1;
	b.y -= 1;

	vertice_t verts[5] = {
		{ float(a.x), float(a.y), 0.01f, 0.01f, D3DCOLOR_RGBA(c.red, c.green, c.blue, c.alpha) },
		{ float(b.x), float(a.y), 0.01f, 0.01f, D3DCOLOR_RGBA(c.red, c.green, c.blue, c.alpha) },
		{ float(b.x), float(b.y), 0.01f, 0.01f, D3DCOLOR_RGBA(c.red, c.green, c.blue, c.alpha) },
		{ float(a.x), float(b.y), 0.01f, 0.01f, D3DCOLOR_RGBA(c.red, c.green, c.blue, c.alpha) },
		{ float(a.x), float(a.y), 0.01f, 0.01f, D3DCOLOR_RGBA(c.red, c.green, c.blue, c.alpha) }
	};

	device->SetTexture(0, nullptr);
	device->DrawPrimitiveUP(D3DPT_LINESTRIP, 4, &verts, 20);
}

void d3d_render::rect_(float x, float y, float w, float h, Color c)
{
	rect({ x,y }, { w + 1,h + 1 }, c);
}

void d3d_render::rect_fill(Vector2D a, Vector2D b, Color c)
{
	b += a;

	vertice_t verts[4] = {
		{a.x, a.y, 0.01f, 0.01f, D3DCOLOR_RGBA(c.red, c.green, c.blue, c.alpha)},
		{b.x, a.y, 0.01f, 0.01f, D3DCOLOR_RGBA(c.red, c.green, c.blue, c.alpha)},
		{a.x, b.y, 0.01f, 0.01f, D3DCOLOR_RGBA(c.red, c.green, c.blue, c.alpha)},
		{b.x, b.y, 0.01f, 0.01f, D3DCOLOR_RGBA(c.red, c.green, c.blue, c.alpha)}
	};

	device->SetTexture(0, nullptr);
	device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, &verts, 20);
}

void d3d_render::draw_filled_triangle(Vector2D p1, Vector2D p2, Vector2D p3, Color c)
{
	vertice_t verts[] = {
		{ p1.x, p1.y, 0.00f, 0.00f, D3DCOLOR_RGBA(c.red, c.green, c.blue, c.alpha) },
		{ p2.x, p2.y, 0.00f, 0.00f, D3DCOLOR_RGBA(c.red, c.green, c.blue, c.alpha) },
		{ p3.x, p3.y, 0.00f, 0.00f, D3DCOLOR_RGBA(c.red, c.green, c.blue, c.alpha) }
	};

	device->SetTexture(0, nullptr);
	device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 1, &verts, 20);
}

void d3d_render::rect_fill_(float x, float y, float w, float h, Color c)
{
	rect_fill({ x, y }, { w, h }, c);
}

void d3d_render::line_(float x, float y, float x1, float x2, Color c)
{
	line({ x, y }, { x1, x2 }, c);
}

void d3d_render::gradient_h_(float x, float y, float w, float h, Color c_a, Color c_b)
{
	gradient_h({ x,y }, { w, h }, c_a, c_b);
}

void d3d_render::gradient_v_(float x, float y, float w, float h, Color c_a, Color c_b)
{
	gradient_v({ x,y }, { w, h }, c_a, c_b);
}

void d3d_render::gradient_h(Vector2D a, Vector2D b, Color c_a, Color c_b)
{
	b += a;

	vertice_t verts[4] = {
	{a.x, a.y, 0.01f, 0.01f, D3DCOLOR_RGBA(c_a.red, c_a.green, c_a.blue, c_a.alpha)},
	{b.x, a.y, 0.01f, 0.01f, D3DCOLOR_RGBA(c_b.red, c_b.green, c_b.blue, c_b.alpha)},
	{a.x, b.y, 0.01f, 0.01f, D3DCOLOR_RGBA(c_a.red, c_a.green, c_a.blue, c_a.alpha)},
	{b.x, b.y, 0.01f, 0.01f, D3DCOLOR_RGBA(c_b.red, c_b.green, c_b.blue, c_b.alpha)}
	};

	device->SetTexture(0, nullptr);
	device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, &verts, 20);
}

void d3d_render::line(Vector2D a, Vector2D b, Color c)
{
	vertice_t verts[2] = {
		{a.x, a.y, 0.01f, 0.01f, D3DCOLOR_RGBA(c.red, c.green, c.blue, c.alpha)},
		{b.x, b.y, 0.01f, 0.01f, D3DCOLOR_RGBA(c.red, c.green, c.blue, c.alpha)}
	};

	device->SetTexture(0, nullptr);
	device->DrawPrimitiveUP(D3DPT_LINELIST, 1, &verts, 20);
}

void d3d_render::gradient_v(Vector2D a, Vector2D b, Color c_a, Color c_b)
{
	b += a;

	vertice_t verts[4] = {
		{a.x, a.y, 0.01f, 0.01f, D3DCOLOR_RGBA(c_a.red, c_a.green, c_a.blue, c_a.alpha)},
		{b.x, a.y, 0.01f, 0.01f, D3DCOLOR_RGBA(c_a.red, c_a.green, c_a.blue, c_a.alpha)},
		{a.x, b.y, 0.01f, 0.01f, D3DCOLOR_RGBA(c_a.red, c_a.green, c_a.blue, c_a.alpha)},
		{b.x, b.y, 0.01f, 0.01f, D3DCOLOR_RGBA(c_a.red, c_a.green, c_a.blue, c_a.alpha)}
	};

	device->SetTexture(0, nullptr);
	device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, &verts, 20);
}

int d3d_render::get_text_width(const char* text, LPD3DXFONT font)
{
	RECT rcRect = { 0,0,0,0 };
	if (font)
	{
		font->DrawText(NULL, text, strlen(text), &rcRect, DT_CALCRECT,
			D3DCOLOR_XRGB(0, 0, 0));
	}

	return rcRect.right - rcRect.left;
}

RECT d3d_render::text_size_to_rect(LPD3DXFONT font, const char* text)
{
	size_t origsize = strlen(text) + 1;
	const size_t newsize = 100;
	size_t convertedChars = 0;
	wchar_t wcstring[newsize];
	mbstowcs_s(&convertedChars, wcstring, origsize, text, _TRUNCATE);

	RECT rcRect = { 0,0,0,0 };
	if (font)
	{
		font->DrawText(NULL, text, strlen(text), &rcRect, DT_CALCRECT,
			D3DCOLOR_XRGB(0, 0, 0));
	}

	return rcRect;
}

void d3d_render::horizontal_gradient(int x, int y, int w, int h, Color c1, Color c2)
{
	BYTE first = c2.red;
	BYTE second = c2.green;
	BYTE third = c2.blue;
	for (int i = 0; i < w; i++)
	{
		float fi = i, fw = w;
		float a = fi / fw;
		DWORD ia = a * 255;
		this->rect_fill_(x + i, y, 2, h, Color(first, second, third, ia));
	}
}

void d3d_render::text(float x, float y, LPD3DXFONT font, std::string text, Color color, bool centered, int drop_shadow, Color shadow_color)
{
	if (centered)
	{
		x -= (get_text_width(text.c_str(), font) / 2);
	}

	RECT DrawArea = { x, y, 1920, DrawArea.top + 200 };

	if (drop_shadow == 1)
	{
		RECT DrawArea2 = { x + 1.f, y, 1920, DrawArea.top + 200 };
		//RECT DrawArea3 = { x - 1.f, y, 1920, DrawArea.top + 200 };
		RECT DrawArea4 = { x, y + 1.f, 1920, DrawArea.top + 200 };
		//RECT DrawArea5 = { x, y - 1.f, 1920, DrawArea.top + 200 };
		font->DrawTextA(NULL, text.c_str(), -1, &DrawArea2, 0, D3DCOLOR_RGBA(shadow_color.red, shadow_color.green, shadow_color.blue, shadow_color.alpha));
		//font->DrawTextA(NULL, text.c_str(), -1, &DrawArea3, 0, D3DCOLOR_RGBA(shadow_color.red, shadow_color.green, shadow_color.blue, shadow_color.alpha));
		font->DrawTextA(NULL, text.c_str(), -1, &DrawArea4, 0, D3DCOLOR_RGBA(shadow_color.red, shadow_color.green, shadow_color.blue, shadow_color.alpha));
		//font->DrawTextA(NULL, text.c_str(), -1, &DrawArea5, 0, D3DCOLOR_RGBA(shadow_color.red, shadow_color.green, shadow_color.blue, shadow_color.alpha));
	}
	else if (drop_shadow == 2)
	{
		RECT DrawArea2 = { x + 1.f, y, 1920, DrawArea.top + 200 };
		RECT DrawArea3 = { x - 1.f, y, 1920, DrawArea.top + 200 };
		RECT DrawArea4 = { x, y + 1.f, 1920, DrawArea.top + 200 };
		RECT DrawArea5 = { x, y - 1.f, 1920, DrawArea.top + 200 };
		font->DrawTextA(NULL, text.c_str(), -1, &DrawArea2, 0, D3DCOLOR_RGBA(shadow_color.red, shadow_color.green, shadow_color.blue, shadow_color.alpha));
		font->DrawTextA(NULL, text.c_str(), -1, &DrawArea3, 0, D3DCOLOR_RGBA(shadow_color.red, shadow_color.green, shadow_color.blue, shadow_color.alpha));
		font->DrawTextA(NULL, text.c_str(), -1, &DrawArea4, 0, D3DCOLOR_RGBA(shadow_color.red, shadow_color.green, shadow_color.blue, shadow_color.alpha));
		font->DrawTextA(NULL, text.c_str(), -1, &DrawArea5, 0, D3DCOLOR_RGBA(shadow_color.red, shadow_color.green, shadow_color.blue, shadow_color.alpha));
	}

	font->DrawTextA(NULL, text.c_str(), -1, &DrawArea, 0, D3DCOLOR_RGBA(color.red, color.green, color.blue, color.alpha));
}

void d3d_render::vertical_gradient_2(int x, int y, int w, int h, Color c1, Color c2)
{
	this->rect_fill_(x, y, w, h, c1);
	BYTE first = c2.red;
	BYTE second = c2.green;
	BYTE third = c2.blue;
	for (int i = 0; i < h; i++)
	{
		float fi = i, fh = h;
		float a = fi / fh;
		DWORD ia = a * 255;
		this->rect_fill_(x, y + i, w, 1, Color(first, second, third, ia));
	}
}

void d3d_render::vertical_gradient(int x, int y, int w, int h, Color c1, Color c2)
{
	this->rect_fill_(x, y, w, h, c1);
	BYTE first = c2.red;
	BYTE second = c2.green;
	BYTE third = c2.blue;
	for (int i = 0; i < h / 1.08; i++)
	{
		float fi = i, fh = h;
		float a = fi / fh;
		DWORD ia = a * 255;
		this->rect_fill_(x, y + i, w, 6, Color(first, second, third, ia));
	}
}


void d3d_render::setup_render_states()
{
	this->device->SetVertexShader(nullptr);
	this->device->SetPixelShader(nullptr);
	this->device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	this->device->SetRenderState(D3DRS_FOGENABLE, FALSE);
	this->device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	this->device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	this->device->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	this->device->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);
	this->device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	this->device->SetRenderState(D3DRS_STENCILENABLE, FALSE);

	this->device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE);
	this->device->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, TRUE);

	this->device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	this->device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	this->device->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, TRUE);
	this->device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	this->device->SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_INVDESTALPHA);
	this->device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	this->device->SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_ONE);

	this->device->SetRenderState(D3DRS_SRGBWRITEENABLE, FALSE);
	this->device->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN |
		D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);
}

void d3d_render::save_state()
{
	this->device->GetRenderState(D3DRS_COLORWRITEENABLE, &dwold_d3drs_colorwriteenable);
	this->device->GetRenderState(D3DRS_ANTIALIASEDLINEENABLE, &dwold_d3drs_antialiasedlineenable);
	this->device->GetRenderState(D3DRS_MULTISAMPLEANTIALIAS, &dwold_d3drs_multisampleantialias);
	this->device->GetTextureStageState(0, D3DTSS_COLORARG0, &dwold_d3dtexturestagestate);
	this->device->GetScissorRect(&rect_old_scissor_rect);
	this->device->GetVertexDeclaration(&vertext_declaration);
	this->device->GetVertexShader(&vertex_shader2);
	this->device->GetPixelShader(&pixel_shader);
	this->device->SetRenderState(D3DRS_COLORWRITEENABLE, 0xffffffff);
	this->device->SetRenderState(D3DRS_SRGBWRITEENABLE, false);
	this->device->SetSamplerState(NULL, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	this->device->SetSamplerState(NULL, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	this->device->SetSamplerState(NULL, D3DSAMP_ADDRESSW, D3DTADDRESS_WRAP);
	this->device->SetSamplerState(NULL, D3DSAMP_SRGBTEXTURE, NULL);
	this->device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
}

void d3d_render::restore_state()
{
	device->SetTextureStageState(0, D3DTSS_COLORARG0, dwold_d3dtexturestagestate);
	device->SetRenderState(D3DRS_COLORWRITEENABLE, dwold_d3drs_colorwriteenable);
	device->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, dwold_d3drs_antialiasedlineenable);
	device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, dwold_d3drs_multisampleantialias);
	device->SetScissorRect(&rect_old_scissor_rect);
	this->device->SetRenderState(D3DRS_COLORWRITEENABLE, dwold_d3drs_colorwriteenable);

	device->SetRenderState(D3DRS_SRGBWRITEENABLE, true);
	device->SetVertexDeclaration(vertext_declaration);
	device->SetVertexShader(vertex_shader2);
	device->SetPixelShader(pixel_shader);
}