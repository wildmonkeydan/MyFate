#pragma once
#include <psxgte.h>
#include <smd/smd.h>

#include "render.h"
#include "camera.h"
#include "utils.h"

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
	NPC(NPCData* datPtr, SMD* smd, SMD* quest);
	void Draw(RenderContext& ctx, Camera& cam);
	bool IsNear(RECT& col);
	unsigned short GetDialougeOffset(Pad& pad);
	
	NPCData* data;
private:
	SMD* model;
	SMD* question;
	SC_OT ot;

	SVECTOR rotation;
	VECTOR position;
	RECT collsion;

	bool playerNear = false;
};