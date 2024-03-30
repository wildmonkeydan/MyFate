#pragma once
#include <cstdint>
#include <psxgte.h>

#include "render.h"
#include "data.h"
#include "player.h"
#include "background.h"

struct Tile {
	unsigned char u;
	unsigned char v;
	bool exit;
	unsigned char exitLevel;
};

class Ground {
public:
	Ground(Data& dat, SMD* npcModel);
	void Update(Player& ply, SMD* npcModel, Data& dat, Background& back);
	void Draw(RenderContext& ctx, RECT& screen_clip, Camera& cam);
	void SwitchLevel(Data& dat, SMD* npcModel, int level);
private:
	Tile* mapPtr;
	SVECTOR grid[9][11];

	NPC* npcs = nullptr;
	unsigned char numNpcs = 0;
};