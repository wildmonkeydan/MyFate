#pragma once
#include "pad.h"
#include "render.h"
#include "camera.h"

#include <psxgte.h>
#include <smd/smd.h>

static constexpr int PLAYER_SPEED = 1;

class Player {
public:
	Player(Pad* ppad, SMD* mdl);
	void Update(Camera& cam);
	void Draw(RenderContext& ctx, Camera& cam, RECT& screen_clip);
	void AddItem(unsigned char index);
	
	VECTOR position;
	RECT collision;
	Pad* pad;
private:	
	SVECTOR rotation;
	SVECTOR verts[4];

	SMD* model;
	SC_OT ot;

	bool items[3] = { false };
};