#include "dialouge.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

Dialouge::Dialouge(char* table) {
	strTable = table;
}

DialougeItem Dialouge::Update(Pad& pad) {
	if (talking) {
		if (pad.IsButtonDown(PAD_CROSS) && progressTimer == 0) {
			progressTimer = 60;

			if (!lastSentence) {
				return Talk(nextSentence);
			}
			else {
				talking = false;
			}
		}

		if (progressTimer != 0) {
			progressTimer--;
		}
	}

	return DialougeItem::None;
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

DialougeItem Dialouge::Talk(unsigned short offset) {
	talking = true;
	
	printf("%d\n", offset);

	char* start = strTable + offset;
	char* iterator = start;

	int sentenceLength = 0;

	while (*iterator != '%' && *iterator != '@') {
		sentenceLength++;
		iterator++;
	}

	lastSentence = *iterator == '%';

	strncpy(dialouge, start, sentenceLength);
	dialouge[sentenceLength] = '\0';

	nextSentence = offset + (sentenceLength + 1);

	DialougeItem item;

	switch (*start) {
	case '^':
		item = DialougeItem::Ring;
		break;
	case '#':
		item = DialougeItem::Bracelet;
		break;
	case '~':
		item = DialougeItem::Coin;
		break;
	default:
		item = DialougeItem::None;
		break;
	}

	return item;
}