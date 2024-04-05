#include "player.h"
#include "utils.h"

#include <inline_c.h>

Player::Player(Pad* ppad, SMD* mdl) {
	setVector(&position, 4098, 512, 4098);
	setVector(&rotation, -1024, 0, 0);

	setVector(&verts[0], 0, 512, 0);
	setVector(&verts[3], 512, 0, 0);
	setVector(&verts[1], 0, 0, 0);
	setVector(&verts[2], 512, 512, 0);

	setRECT(&collision, position.vx - 256, position.vz - 256, 512, 512);

	pad = ppad;
	model = mdl;
}

void Player::Update(Camera& cam) {
	DVECTOR leftStick = pad->GetStick(LEFT_STICK);

	position.vx += (leftStick.vx >> PLAYER_SPEED) + ((((unsigned char)pad->IsButtonDown(PAD_LEFT) << 7) >> PLAYER_SPEED) * -1) + (((unsigned char)pad->IsButtonDown(PAD_RIGHT) << 7) >> PLAYER_SPEED);
	position.vz += (leftStick.vy >> PLAYER_SPEED) + ((((unsigned char)pad->IsButtonDown(PAD_UP) << 7) >> PLAYER_SPEED) * -1) + (((unsigned char)pad->IsButtonDown(PAD_DOWN) << 7) >> PLAYER_SPEED);

	if (position.vx < 0) {
		position.vx = 0;
	}
	if (position.vx > 9728) {
		position.vx = 9728;
	}

	if (position.vz < 0) {
		position.vz = 0;
	}
	if (position.vz > 8192) {
		position.vz = 8192;
	}

	setRECT(&collision, position.vx - 256, position.vz - 256, 512, 512);
	cam.position.vx = position.vx << 12;
}

void Player::Draw(RenderContext& ctx, Camera& cam, RECT& screen_clip) {
	int p;
	POLY_F4* poly;

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

	/*poly = (POLY_F4*)ctx._next_packet;

	for (int i = 0; i < 1; i++) {

		
		// Load first three vertices to GTE
		gte_ldv3(
			&verts[0],
			&verts[1],
			&verts[2]);

		gte_rtpt();

		gte_avsz3();
		gte_stotz(&p);

		if (((p >> 2) >= DEFAULT_OT_LENGTH) || ((p >> 2) <= 0))
			continue;
		
		setPolyF4(poly);

		// Set the projected vertices to the primitive
		gte_stsxy0(&poly->x0);
		gte_stsxy1(&poly->x1);
		gte_stsxy2(&poly->x2);

		// Compute the last vertex and set the result
		gte_ldv0(&verts[3]);
		gte_rtps();
		gte_stsxy(&poly->x3);

		if (quad_clip(&screen_clip,
			(DVECTOR*)&poly->x0, (DVECTOR*)&poly->x1,
			(DVECTOR*)&poly->x2, (DVECTOR*)&poly->x3))
			continue;

		gte_avsz4();
		gte_stotz(&p);

		setRGB0(poly, 255, 128, 128);

		addPrim(ctx._buffers[ctx._active_buffer]._ot + p, poly);
		poly++;
	}

	ctx._next_packet = (uint8_t*)poly;*/

	// Restore matrix
	PopMatrix();
}

void Player::AddItem(unsigned char index) {
	items[index] = true;
}