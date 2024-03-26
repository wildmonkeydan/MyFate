#include "data.h"

#include <stdio.h>
#include <stdlib.h>

Data::Data(uint32_t* basePointer) {
	base = (uint8_t*)basePointer;
	
	levels[0] = base + 12;

	for (int i = 0; i < 8; i++) {
		unsigned char numNPC, numObject;
		int offset = 320;
		uint8_t* tempPtr = levels[i];

		numNPC = *(tempPtr + offset);
		printf("Level %d - NPCs: %d", i, numNPC);

		offset += 1 + (numNPC);

		numObject = *(tempPtr + offset);


		offset += 1 + (numObject);

		levels[i + 1] = levels[i] + offset;
		printf("  Objs: %d  Offset: %d\n", numObject, offset);
	}

	npcs = base + *(basePointer);
	objects = base + *(basePointer + 1);
	strTable = base + *(basePointer + 2);
}

uint8_t* Data::GetBase() {
	return base;
}

uint8_t* Data::GetLevel(int index) {
	return levels[index];
}

NPCData* Data::GetNpc(int index) {
	return (NPCData*)(npcs + (18 * index));
}

ObjectData* Data::GetObject(int index) {
	return (ObjectData*)(objects + (9 * index));
}

char* Data::GetString(int offset) {
	return (char*)(strTable + offset);
}