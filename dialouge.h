#pragma once

#include "render.h"
#include "pad.h"

class Dialouge {
public:
	Dialouge(char* table);
	void Update(Pad& pad);
	void Draw(RenderContext& ctx);
	void Talk(unsigned short offset);
	bool talking = false;

private:
	char* strTable;

	
	bool lastSentence = false;
	int progressTimer = 60;
	unsigned short nextSentence = 0;
	char dialouge[128];
};