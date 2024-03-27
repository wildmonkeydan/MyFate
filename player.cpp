#include "player.h"
#include "utils.h"

#include <inline_c.h>

Player::Player() {
	setVector(&position, 0, 0, 0);
	setVector(&rotation, 0, 0, 0);

	setVector(&verts[0], 0, 512, 0);
	setVector(&verts[3], 512, 0, 0);
	setVector(&verts[1], 0, 0, 0);
	setVector(&verts[2], 512, 512, 0);
}

void Player::Update(Pad& pad) {
	DVECTOR leftStick = pad.GetStick(LEFT_STICK);

	position.vx += leftStick.vx >> PLAYER_SPEED;
	position.vz += leftStick.vy >> PLAYER_SPEED;
}

void Player::Draw(RenderContext& ctx, Camera& cam, RECT& screen_clip) {
	int p;

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

	for (int i = 0; i < 1; i++) {
		auto poly = ctx.new_primitive<POLY_F4>(0);

		setPolyF4(poly);
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
	}

	// Restore matrix
	PopMatrix();
}