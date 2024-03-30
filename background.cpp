#include "background.h"

Background::Background() {
	current = Type::Void;
}

void Background::Update() {

}

void Background::Draw(RenderContext& ctx) {
	POLY_FT4* texQuad;
	SPRT* sprite;
	DR_TPAGE* tpage;

	switch (current) {
	case Type::Void:
		break;
	case Type::Sky:
		texQuad = (POLY_FT4*)ctx._next_packet;

		setPolyFT4(texQuad);
		setRGB0(texQuad, 128, 128, 128);
		setTPage(texQuad, 1, 1, 320, 0);
		setClut(texQuad, 320, 480);
		setUV4(texQuad, 0, 0, 256, 0, 0, 256, 256, 256);
		setXY4(texQuad, 0, 0, 320, 0, 0, 256, 320, 256);

		addPrim(ctx._buffers[ctx._active_buffer]._ot + 1024, texQuad);

		texQuad++;
		ctx._next_packet = (uint8_t*)texQuad;
		break;
	case Type::Vapour:
		sprite = (SPRT*)ctx._next_packet;
		setSprt(sprite);
		setRGB0(sprite, 128, 128, 128);
		setUV0(sprite, 0, 0);
		setWH(sprite, 256, 256);
		setXY0(sprite, 0, 0);
		setClut(sprite, 320, 488);

		addPrim(ctx._buffers[ctx._active_buffer]._ot + 1024, sprite);
		sprite++;

		tpage = (DR_TPAGE*)sprite;
		setDrawTPage(tpage, 0, 1, getTPage(1, 0, 640, 256));
		addPrim(ctx._buffers[ctx._active_buffer]._ot + 1024, tpage);
		tpage++;

		sprite = (SPRT*)tpage;
		setSprt(sprite);
		setRGB0(sprite, 128, 128, 128);
		setUV0(sprite, 0, 0);
		setWH(sprite, 64, 256);
		setXY0(sprite, 256, 0);
		setClut(sprite, 320, 488);

		addPrim(ctx._buffers[ctx._active_buffer]._ot + 1024, sprite);
		sprite++;

		tpage = (DR_TPAGE*)sprite;
		setDrawTPage(tpage, 0, 1, getTPage(1, 0, 768, 256));
		addPrim(ctx._buffers[ctx._active_buffer]._ot + 1024, tpage);
		tpage++;

		ctx._next_packet = (uint8_t*)tpage;
		break;
	}
}

void Background::SwitchType(Type swit) {
	current = swit;
}