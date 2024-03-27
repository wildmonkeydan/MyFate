#pragma once
#include <cstdint>
#include <psxgte.h>

#include "render.h"

struct Tile {
	unsigned char u;
	unsigned char v;
	bool exit;
	unsigned char exitLevel;
};

class Ground {
public:
	Ground(uint8_t* map);
	void Draw(RenderContext& ctx, RECT& screen_clip);
private:
	Tile* mapPtr;
	SVECTOR grid[9][11];
};