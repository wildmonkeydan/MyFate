#include "npc.h"

#include <inline_c.h>

NPC::NPC(NPCData* datPtr, SMD* smd, SMD* quest) {
	data = datPtr;
	model = smd;
	question = quest;

	setVector(&position, data->position.vx, data->position.vy + 256, data->position.vz);
	setVector(&rotation, -1024, 0, 0);
	setRECT(&collsion, position.vx - 1024, position.vz - 1024, 2048, 2048);

	printf("NPC %d \n", data->general);
}

void NPC::Draw(RenderContext& ctx, Camera& cam) {
	// Object matrix for player
	MATRIX omtx;

	// Set object rotation and position
	RotMatrix(&rotation, &omtx);
	TransMatrix(&omtx, &position);

	// Composite coordinate matrix transform, so object will be rotated and
	// positioned relative to camera matrix (mtx), so it'll appear as
	// world-space relative.
	CompMatrixLV(cam.GetMatrix(), &omtx, &omtx);

	// Save matrix
	PushMatrix();

	// Set matrices
	gte_SetRotMatrix(&omtx);
	gte_SetTransMatrix(&omtx);

	ot.ot = ctx._buffers[ctx._active_buffer]._ot;
	ot.otlen = DEFAULT_OT_LENGTH;
	ot.zdiv = 1;
	ot.zoff = 0;

	ctx._next_packet = smdSortModel(&ot, ctx._next_packet, model);
	if (playerNear) {
		ctx._next_packet = smdSortModelFlat(ot.ot, ctx._next_packet, question);
	}

	// Restore matrix
	PopMatrix();
}

bool NPC::IsNear(RECT& col) {
	playerNear = CheckRecs(col, collsion);
	return playerNear;
}