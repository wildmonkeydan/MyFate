#pragma once
#include "pad.h"
#include "render.h"
#include "camera.h"

#include <psxgte.h>

static constexpr int PLAYER_SPEED = 1;

class Player {
public:
	Player();
	void Update(Pad& pad, Camera& cam);
	void Draw(RenderContext& ctx, Camera& cam, RECT& screen_clip);
private:
	VECTOR position;
	SVECTOR rotation;

	SVECTOR verts[4];
};