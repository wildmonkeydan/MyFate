#include "npc.h"

#include <inline_c.h>

NPC::NPC(NPCData* datPtr, SMD* smd) {
	data = datPtr;
	model = smd;

	setVector(&position, data->position.vx, data->position.vy, data->position.vz);

	printf("NPC %d %d %d\n", data->position.vx, data->position.vy, data->position.vz);
}

void NPC::Draw(RenderContext& ctx, Camera& cam) {
	/*// Object matrix for player
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
	gte_SetTransMatrix(&omtx);*/

	ot.ot = ctx._buffers[ctx._active_buffer]._ot;
	ot.otlen = DEFAULT_OT_LENGTH;
	ot.zdiv = 1;
	ot.zoff = 0;

	ctx._next_packet = smdSortModelFlat(ot.ot, ctx._next_packet, model);

	// Restore matrix
	//PopMatrix();
}