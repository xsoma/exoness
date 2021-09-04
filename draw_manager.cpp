#include "draw_manager.h"
#include "i_interfaces.h"
#include <vector>

void render::invalidate_objects()
{
	this->m_device = nullptr;
}

void render::restore_objects(LPDIRECT3DDEVICE9 m_device)
{
	this->m_device = m_device;
	this->m_device->GetViewport(&m_viewport);
}

void render::initalize_objects(LPDIRECT3DDEVICE9 m_device)
{
	this->m_device = m_device;
	this->m_device->GetViewport(&m_viewport);
}

void render::setup_states() const
{
	D3DVIEWPORT9 d3d_viewport;
	this->m_device->GetViewport(&d3d_viewport);

	this->m_device->SetVertexShader(nullptr);
	this->m_device->SetPixelShader(nullptr);
	this->m_device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);

	this->m_device->SetRenderState(D3DRS_LIGHTING, FALSE);
	this->m_device->SetRenderState(D3DRS_FOGENABLE, FALSE);
	this->m_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	this->m_device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	this->m_device->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	this->m_device->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);
	this->m_device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	this->m_device->SetRenderState(D3DRS_STENCILENABLE, FALSE);

	this->m_device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE);
	this->m_device->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, TRUE);

	this->m_device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	this->m_device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	this->m_device->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, TRUE);
	this->m_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	this->m_device->SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_INVDESTALPHA);
	this->m_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	this->m_device->SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_ONE);

	this->m_device->SetRenderState(D3DRS_SRGBWRITEENABLE, FALSE);
	this->m_device->SetRenderState(D3DRS_COLORWRITEENABLE,
		D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);
}

void render::draw_text(int X, int Y, Color Color, int Font, bool Center, const char* _Input, ...)
{
	int apple = 0;
	char Buffer[2048] = { '\0' };
	va_list Args;

	va_start(Args, _Input);
	vsprintf_s(Buffer, _Input, Args);
	va_end(Args);

	size_t Size = strlen(Buffer) + 1;
	wchar_t* WideBuffer = new wchar_t[Size];

	mbstowcs_s(0, WideBuffer, Size, Buffer, Size - 1);

	int Width = 0, Height = 0;

	if (Center)
		interfaces::surface->GetTextSize(Font, WideBuffer, Width, Height);

	interfaces::surface->DrawSetTextColor(Color.red, Color.green, Color.blue, Color.alpha);
	interfaces::surface->DrawSetTextFont(Font);
	interfaces::surface->DrawSetTextPos(X - (Width / 2), Y);
	interfaces::surface->DrawPrintText(WideBuffer, wcslen(WideBuffer));
}

void render::outlined_rect(int x, int y, int w, int h, Color color)
{
	interfaces::surface->DrawSetColor(color.red, color.green, color.blue, color.alpha);
	interfaces::surface->DrawOutlinedRect(x, y, x + w, y + h);
}

void render::filled_rect(int x, int y, int w, int h, Color color)
{
	interfaces::surface->DrawSetColor(color.red, color.green, color.blue, color.alpha);
	interfaces::surface->DrawFilledRect(x, y, x + w, y + h);
}

Vector2D render::get_mouse_pos()
{
	POINT mousePosition;
	GetCursorPos(&mousePosition);
	ScreenToClient(FindWindow(0, "Counter-Strike: Global Offensive"), &mousePosition);
	return { static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y) };
}

void render::simple_vertical_gradient(int x, int y, int w, int h, Color c1, Color c2)
{
	this->filled_rect(x, y, w, h, c1);
	BYTE first = c2.red;
	BYTE second = c2.green;
	BYTE third = c2.blue;
	for (int i = 0; i < h / 1.08; i++)
	{
		float fi = i, fh = h;
		float a = fi / fh;
		DWORD ia = a * 255;
		this->filled_rect(x, y + i, w, 6, Color(first, second, third, ia));
	}
}

void render::simple_horizontal_gradient(int x, int y, int w, int h, Color c1, Color c2)
{
	this->filled_rect(x, y, w, h, c1);
	BYTE first = c2.red;
	BYTE second = c2.green;
	BYTE third = c2.blue;
	for (int i = 0; i < w; i++)
	{
		float fi = i, fw = w;
		float a = fi / fw;
		DWORD ia = a * 255;
		this->filled_rect(x + i, y, 2, h, Color(first, second, third, ia));
	}
}

RECT render::get_text_size_rect(DWORD font, const char* text)
{
	size_t origsize = strlen(text) + 1;
	const size_t newsize = 100;
	size_t convertedChars = 0;
	wchar_t wcstring[newsize];
	mbstowcs_s(&convertedChars, wcstring, origsize, text, _TRUNCATE);

	RECT rect; int x, y;
	interfaces::surface->GetTextSize(font, wcstring, x, y);
	rect.left = x; rect.bottom = y;
	rect.right = x;
	return rect;
}

bool render::mouse_in_region(int x, int y, int x2, int y2)
{
	if (get_mouse_pos().x > x&& get_mouse_pos().y > y&& get_mouse_pos().x < x2 + x && get_mouse_pos().y < y2 + y)
		return true;

	return false;
}

void render::simple_vertical_gradient_2(int x, int y, int w, int h, Color c1, Color c2)
{
	this->filled_rect(x, y, w, h, c1);
	BYTE first = c2.red;
	BYTE second = c2.green;
	BYTE third = c2.blue;
	for (int i = 0; i < h; i++)
	{
		float fi = i, fh = h;
		float a = fi / fh;
		DWORD ia = a * 255;
		this->filled_rect(x, y + i, w, 1, Color(first, second, third, ia));
	}
}

void render::circle_3d(Vector position, float points, float radius, Color color)
{
	float step = (float)M_PI * 2.0f / points;

	std::vector <Vector> points3d;

	for (float a = 0; a < (M_PI * 2.0f); a += step)
	{
		Vector start(radius * cosf(a) + position.x, radius * sinf(a) + position.y, position.z);
		Vector end(radius * cosf(a + step) + position.x, radius * sinf(a + step) + position.y, position.z);

		Vector start2d, end2d;
		if (interfaces::debugoverlay->ScreenPosition(start, start2d) || interfaces::debugoverlay->ScreenPosition(end, end2d))
			return;

		render::get().line(start2d.x, start2d.y, end2d.x, end2d.y, color);
	}
}

void render::line(int x, int y, int x2, int y2, Color color) {
	interfaces::surface->DrawSetColor(color.red, color.green, color.blue, color.alpha);
	interfaces::surface->DrawLine(x, y, x2, y2);
}

void render::add_textured_polygon(int n, Vertex_t* vertice, int r, int g, int b, int a) {
	static int texture_id = interfaces::surface->CreateNewTextureID(true);
	static unsigned char buf[4] = { 255, 255, 255, 255 };
	interfaces::surface->DrawSetColor(r, g, b, a);
	interfaces::surface->DrawSetTexture(texture_id);
	interfaces::surface->DrawTexturedPolygon(n, vertice);
}

void render::draw_filled_triangle(std::array< Vector2D, 3 >points, Color colour) {
	std::array< Vertex_t, 3 >vertices{ Vertex_t(points.at(0)), Vertex_t(points.at(1)), Vertex_t(points.at(2)) };
	add_textured_polygon(3, vertices.data(), colour.red, colour.green, colour.blue, colour.alpha);
}