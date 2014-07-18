#pragma once
#include "mapr_basic_type.h"
#include "pixel_formats.h"
#include "platform/agg_platform_support.h"

class PngProductor;
class AggFrame : public agg::platform_support
{
	typedef agg::renderer_base<pixfmt> renderer_base;
	typedef agg::renderer_scanline_aa_solid<renderer_base> renderer_solid;
public:
	AggFrame(agg::pix_format_e format, bool flip_y);
	virtual ~AggFrame();
	virtual void on_resize(int cx, int cy);
	virtual void on_draw();

	void drawGrid( GridId gridId, int left, int top );
	void addBuffer(int left, int top, const char* buffers, int width, int height);
	void addBuffer(int left, int top, const wchar_t* pngName);

private:
	PngProductor* m_pngProductor;
};