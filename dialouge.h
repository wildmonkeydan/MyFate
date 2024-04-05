#pragma once

#include "render.h"
#include "pad.h"

enum class DialougeItem : unsigned char {
	None,
	Ring,
	Bracelet,
	Coin
};

class Dialouge {
public:
	Dialouge(char* table);
	DialougeItem Update(Pad& pad);
	void Draw(RenderContext& ctx);
	DialougeItem Talk(unsigned short offset);
	bool talking = false;

private:
	char* strTable;

	
	bool lastSentence = false;
	int progressTimer = 60;
	unsigned short nextSentence = 0;
	char dialouge[128];
};