#pragma once
#include <cstdint>
#include <psxgte.h>
#include <psxgpu.h>

#include "render.h"
#include "pad.h"

class Camera {
public:
	Camera(VECTOR pos, VECTOR rot);
	void Update(Pad& pad, RenderContext& ctx);
	MATRIX* GetMatrix();
	VECTOR position;

private:
	
	VECTOR rotation;

	VECTOR	tpos;			// Translation value for matrix calculations
	SVECTOR	trot;			// Rotation value for matrix calculations
	MATRIX	mtx, lmtx;		// Rotation matrices for geometry and lighting

	MATRIX color_mtx = {
	ONE, 0, 0,	// Red
	ONE, 0, 0,	// Green
	ONE, 0, 0	// Blue
	};
};