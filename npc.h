#pragma once
#include <psxgte.h>

struct NPCData {
	SVECTOR position;
	unsigned short general;
	unsigned short ring;
	unsigned short bracelet;
	unsigned short coin;
	unsigned short continued;
};

class NPC {
public:

private:
	NPCData* data;
};