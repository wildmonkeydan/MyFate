#pragma once
#include <psxgpu.h>
#include <psxgte.h>
#include <stdlib.h>

#include "cd.h"
#include "pad.h"
#include "render.h"
#include "dialouge.h"


class Item {
public:
	Item(DialougeItem type, CD& cd);
	bool Update(Pad& pad);
	void Draw(RenderContext& ctx);
	unsigned char Give();
private:
	DVECTOR itemScreenCoord;
	RECT itemUV;

	DVECTOR backgroundScreenCoord;
	RECT backgroundUV;

	uint32_t* dataPtr = nullptr;

	bool itemGot = false;
	int inputTimer = 60;
	unsigned char inventorySlot = 0;
};