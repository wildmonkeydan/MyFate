#include "npc.h"

#include <inline_c.h>

NPC::NPC(NPCData* datPtr, SMD* smd) {
	data = datPtr;
	model = smd;

	setVector(&position, data->position.vx, data->position.vy + 256, data->position.vz);
	setVector(&rotation, -1024, 0, 0);

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
	ot.zdiv = 0;
	ot.zoff = 0;

	ctx._next_packet = smdSortModel(&ot, ctx._next_packet, model);

	// Restore matrix
	PopMatrix();
}