#include "item.h"
#include "utils.h"

Item::Item(DialougeItem type, CD& cd) {
	TIM_IMAGE img;

	switch (type) {
	case DialougeItem::Ring:
		dataPtr = cd.LoadFile("\\RING.TIM;1");
		LoadTexture((u_long*)dataPtr, &img);

		itemScreenCoord.vx = 62; itemScreenCoord.vy = 108;
		setRECT(&itemUV, 0, 0, 44, 22);

		backgroundScreenCoord.vx = 0; backgroundScreenCoord.vy = 40;
		setRECT(&backgroundUV, 0, 40, 255, 216);
		
		break;
	}

	if (dataPtr != nullptr) {
		free(dataPtr);
	}

	inventorySlot = (unsigned char)type - 1;
}

bool Item::Update(Pad& pad) {
	if (!itemGot) {
		if (pad.IsButtonDown(PAD_CROSS) && inputTimer == 0) {
			itemGot = true;
			inputTimer = 60;
		}
	}
	else {
		if (pad.IsButtonDown(PAD_CROSS) && inputTimer == 0) {
			return true;
		}
	}

	if (inputTimer > 0) {
		inputTimer--;
	}

	return false;
}

void Item::Draw(RenderContext& ctx) {
	if (!itemGot) {
		SPRT* item = ctx.new_primitive<SPRT>();

		setSprt(item);
		setUV0(item, itemUV.x, itemUV.y);
		setWH(item, itemUV.w, itemUV.h);
		setXY0(item, itemScreenCoord.vx, itemScreenCoord.vy);
		setRGB0(item, 128, 128, 128);
		setClut(item, 320, 464);
	}

	SPRT* background = ctx.new_primitive<SPRT>();

	setSprt(background);
	setUV0(background, backgroundUV.x, backgroundUV.y);
	setWH(background, backgroundUV.w, backgroundUV.h);
	setXY0(background, backgroundScreenCoord.vx, backgroundScreenCoord.vy);
	setRGB0(background, 128, 128, 128);
	setClut(background, 320, 464);
	
	

	DR_TPAGE* tpage = ctx.new_primitive<DR_TPAGE>();

	setDrawTPage(tpage, 0, 1, getTPage(1, 0, 448, 0));
}

unsigned char Item::Give() {
	return inventorySlot;
}