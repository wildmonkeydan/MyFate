#pragma once
#include <psxgte.h>
#include <smd/smd.h>

#include "render.h"
#include "camera.h"

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
	NPC(NPCData* datPtr, SMD* smd);
	void Draw(RenderContext& ctx, Camera& cam);
private:
	NPCData* data;
	SMD* model;
	SC_OT ot;

	SVECTOR rotation;
	VECTOR position;
};