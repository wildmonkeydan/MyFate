#include "background.h"

Background::Background() {
	current = Type::Void;
}

void Background::Update() {
	switch (current) {
	case Type::Sky:
		if (scrollTime == 0) {
			scrollTime = 2;

			skyScroll++;

			if (skyScroll >= 256) {
				skyScroll = 0;
			}
		}
		else {
			scrollTime--;
		}
	}
}

void Background::Draw(RenderContext& ctx) {
	SPRT* sprite;
	DR_TPAGE* tpage;

	switch (current) {
	case Type::Void:
		break;
	case Type::Sky:
		sprite = (SPRT*)ctx._next_packet;
		setSprt(sprite);
		setRGB0(sprite, 128, 128, 128);
		setUV0(sprite, skyScroll, 0);
		setWH(sprite, 256, 256);
		setXY0(sprite, 0, 0);
		setClut(sprite, 320, 480);

		addPrim(ctx._buffers[ctx._active_buffer]._ot + 4093, sprite);
		sprite++;

		setSprt(sprite);
		setRGB0(sprite, 128, 128, 128);
		setUV0(sprite, skyScroll, 0);
		setWH(sprite, 64, 256);
		setXY0(sprite, 256, 0);
		setClut(sprite, 320, 480);

		addPrim(ctx._buffers[ctx._active_buffer]._ot + 4093, sprite);
		sprite++;

		tpage = (DR_TPAGE*)sprite;
		setDrawTPage(tpage, 0, 1, getTPage(1, 0, 320, 0));
		addPrim(ctx._buffers[ctx._active_buffer]._ot + 4093, tpage);
		tpage++;

		ctx._next_packet = (uint8_t*)tpage;
		break;
	case Type::Vapour:
		sprite = (SPRT*)ctx._next_packet;
		setSprt(sprite);
		setRGB0(sprite, 128, 128, 128);
		setUV0(sprite, 0, 0);
		setWH(sprite, 256, 256);
		setXY0(sprite, 0, 0);
		setClut(sprite, 320, 488);

		addPrim(ctx._buffers[ctx._active_buffer]._ot + 4093, sprite);
		sprite++;

		tpage = (DR_TPAGE*)sprite;
		setDrawTPage(tpage, 0, 1, getTPage(1, 0, 640, 256));
		addPrim(ctx._buffers[ctx._active_buffer]._ot + 4093, tpage);
		tpage++;

		sprite = (SPRT*)tpage;
		setSprt(sprite);
		setRGB0(sprite, 128, 128, 128);
		setUV0(sprite, 0, 0);
		setWH(sprite, 64, 256);
		setXY0(sprite, 256, 0);
		setClut(sprite, 320, 488);

		addPrim(ctx._buffers[ctx._active_buffer]._ot + 4093, sprite);
		sprite++;

		tpage = (DR_TPAGE*)sprite;
		setDrawTPage(tpage, 0, 1, getTPage(1, 0, 768, 256));
		addPrim(ctx._buffers[ctx._active_buffer]._ot + 4093, tpage);
		tpage++;

		ctx._next_packet = (uint8_t*)tpage;
		break;
	}
}

void Background::SwitchType(Type swit) {
	current = swit;
}