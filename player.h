#pragma once
#include "pad.h"
#include "render.h"
#include "camera.h"

#include <psxgte.h>

static constexpr int PLAYER_SPEED = 1;

class Player {
public:
	Player(Pad* ppad);
	void Update(Camera& cam);
	void Draw(RenderContext& ctx, Camera& cam, RECT& screen_clip);
	
	VECTOR position;
	RECT collision;
	Pad* pad;
private:	
	SVECTOR rotation;
	SVECTOR verts[4];
};