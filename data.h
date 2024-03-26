#pragma once
#include <cstdint>

#include "object.h"
#include "npc.h"

class Data {
public:
	Data(uint32_t* basePointer);
	uint8_t* GetBase();
	uint8_t* GetLevel(int index);
	NPCData* GetNpc(int index);
	ObjectData* GetObject(int index);
	char* GetString(int offset);
private:
	uint8_t* base;
	uint8_t* levels[9];
	uint8_t* npcs;
	uint8_t* objects;
	uint8_t* strTable;
};