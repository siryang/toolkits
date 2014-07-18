#pragma once
#include "pixel_formats.h"
#include "platform/agg_platform_support.h"
enum
{
	frame_width = 320,
	frame_height = 200
};

class AggFrame : public agg::platform_support
{
	typedef agg::renderer_base<pixfmt> renderer_base;
	typedef agg::renderer_scanline_aa_solid<renderer_base> renderer_solid;
public:
	AggFrame(agg::pix_format_e format, bool flip_y);
	virtual void on_resize(int cx, int cy);
	virtual void on_draw();
	void addBuffer(int left, int top, const char* buffers, size_t width, size_t height);
	void addBuffer(int left, int top, const wchar_t* pngName);
private:

};