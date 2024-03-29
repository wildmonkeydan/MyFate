#include "dialouge.h"

#include <string.h>

Dialouge::Dialouge(char* table) {
	strTable = table;
}

void Dialouge::Update(Pad& pad) {
	if (talking) {
		if (pad.IsButtonDown(PAD_CROSS) && progressTimer == 0) {
			progressTimer = 30;

			if (!lastSentence) {
				Talk(nextSentence);
			}
			else {
				talking = false;
			}
		}

		if (progressTimer != 0) {
			progressTimer--;
		}
	}
}

void Dialouge::Draw(RenderContext& ctx) {
	if (talking) {
		TILE* back = ctx.new_primitive<TILE>(1);

		setTile(back);
		setRGB0(back, 0, 0, 0);
		setXY0(back, 6, 14);
		setWH(back, 308, 36);
		
		ctx.draw_text(8, 16, 0, dialouge);
	}
}

void Dialouge::Talk(unsigned short offset) {
	talking = true;

	char* start = strTable + offset;
	char* iterator = start;

	int sentenceLength = 0;

	while (*iterator != '%' && *iterator != '@') {
		sentenceLength++;
		iterator++;
	}

	if (*iterator == '%')
		lastSentence = true;

	strncpy(dialouge, start, sentenceLength);
	dialouge[sentenceLength] = '\0';

	nextSentence = offset + (sentenceLength + 1);
}