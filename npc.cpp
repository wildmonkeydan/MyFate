#include "npc.h"

#include <inline_c.h>
#include <stdio.h>
#include <stdlib.h>

NPC::NPC(NPCData* datPtr, SMD* smd, SMD* quest) {
	data = datPtr;
	model = smd;
	question = quest;

	printf("NPC: G-%d, R-%d, B-%d, C-%d, CN-%d\n", data->general, data->ring, data->bracelet, data->coin, data->continued);

	setVector(&position, data->position.vx, data->position.vy + 256, data->position.vz);
	setVector(&rotation, -1024, 0, 0);
	setRECT(&collsion, position.vx - 1024, position.vz - 1024, 2048, 2048);
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
	if (playerNear) {
		ctx._next_packet = smdSortModel(&ot, ctx._next_packet, question);
	}

	// Restore matrix
	PopMatrix();
}

bool NPC::IsNear(RECT& col) {
	playerNear = CheckRecs(col, collsion);
	return playerNear;
}

unsigned short NPC::GetDialougeOffset(Pad& pad) {
	if (pad.IsButtonDown(PAD_CROSS)) {
		return data->general;
	}
	else if (pad.IsButtonDown(PAD_CIRCLE)) {
		return data->ring;
	}
	else if (pad.IsButtonDown(PAD_SQUARE)) {
		return data->bracelet;
	}
	else {
		return data->coin;
	}
}