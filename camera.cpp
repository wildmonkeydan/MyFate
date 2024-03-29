#include "camera.h"

#include <inline_c.h>

#define CENTERX			SCREEN_XRES>>1
#define CENTERY			SCREEN_YRES>>1

Camera::Camera(VECTOR pos, VECTOR rot) {
	position = pos;
	rotation = rot;

	//setVector(&position, 3401, 1468, 9608);
	//setVector(&rotation, -6136, 0, 0);

	
	

	// Initialize the GTE
	InitGeom();

	// Set GTE offset (recommended method  of centering)
	gte_SetGeomOffset(CENTERX, CENTERY);

	// Set screen depth (basically FOV control, W/2 works best)
	gte_SetGeomScreen(CENTERX);

	// Set light ambient color and light color matrix
	gte_SetBackColor(0, 0, 0);
	gte_SetColorMatrix(&color_mtx);
}

void Camera::Update(Pad& pad, RenderContext& ctx) {
	

	// Divide out fractions of camera rotation
	trot.vx = rotation.vx >> 12;
	trot.vy = rotation.vy >> 12;
	trot.vz = rotation.vz >> 12;


	// Look controls
	if (pad.IsButtonDown(PAD_UP)) {

		// Look up
		rotation.vx -= ONE * 8;

	}
	else if (pad.IsButtonDown(PAD_DOWN)) {

		// Look down
		rotation.vx += ONE * 8;

	}

	if (pad.IsButtonDown(PAD_LEFT)) {

		// Look left
		rotation.vy += ONE * 8;

	}
	else if (pad.IsButtonDown(PAD_RIGHT)) {

		// Look right
		rotation.vy -= ONE * 8;

	}

	// Movement controls
	if (pad.IsButtonDown(PAD_TRIANGLE)) {

		// Move forward
		position.vx -= ((isin(trot.vy) * icos(trot.vx)) >> 12) << 2;
		position.vy += isin(trot.vx) << 2;
		position.vz += ((icos(trot.vy) * icos(trot.vx)) >> 12) << 2;

	}
	else if (pad.IsButtonDown(PAD_CROSS)) {

		// Move backward
		position.vx += ((isin(trot.vy) * icos(trot.vx)) >> 12) << 2;
		position.vy -= isin(trot.vx) << 2;
		position.vz -= ((icos(trot.vy) * icos(trot.vx)) >> 12) << 2;

	}

	if (pad.IsButtonDown(PAD_SQUARE)) {

		// Slide left
		position.vx -= icos(trot.vy) << 2;
		position.vz -= isin(trot.vy) << 2;

	}
	else if (pad.IsButtonDown(PAD_CIRCLE)) {

		// Slide right
		position.vx += icos(trot.vy) << 2;
		position.vz += isin(trot.vy) << 2;

	}

	if (pad.IsButtonDown(PAD_R1)) {

		// Slide up
		position.vx -= ((isin(trot.vy) * isin(trot.vx)) >> 12) << 2;
		position.vy -= icos(trot.vx) << 2;
		position.vz += ((icos(trot.vy) * isin(trot.vx)) >> 12) << 2;

	}

	if (pad.IsButtonDown(PAD_R2)) {

		// Slide down
		position.vx += ((isin(trot.vy) * isin(trot.vx)) >> 12) << 2;
		position.vy += icos(trot.vx) << 2;
		position.vz -= ((icos(trot.vy) * isin(trot.vx)) >> 12) << 2;

	}

	FntPrint(-1, "X=%d Y=%d Z=%d\n",
		position.vx >> 12,
		position.vy >> 12,
		position.vz >> 12);
	/*FntPrint(-1, "RX=%d RY=%d\n",
		rotation.vx >> 12,
		rotation.vy >> 12);*/

	// Flush text to drawing area
	FntFlush(-1);

	// Set rotation to the matrix
	RotMatrix(&trot, &mtx);

	// Divide out the fractions of camera coordinates and invert
	// the sign, so camera coordinates will line up to world
	// (or geometry) coordinates
	tpos.vx = -position.vx >> 12;
	tpos.vy = -position.vy >> 12;
	tpos.vz = -position.vz >> 12;

	// Apply rotation of matrix to translation value to achieve a
	// first person perspective
	ApplyMatrixLV(&mtx, &tpos, &tpos);

	// Set translation matrix
	TransMatrix(&mtx, &tpos);

	// Set rotation and translation matrix
	gte_SetRotMatrix(&mtx);
	gte_SetTransMatrix(&mtx);
}

MATRIX* Camera::GetMatrix() {
	return &mtx;
}