#include "ground.h"
#include "utils.h"

#include <inline_c.h>

Ground::Ground(uint8_t* map) {
	mapPtr = (Tile*)map;

	for (int y = 0; y < 9; y++) {
		for (int x = 0; x < 11; x++) {
			grid[y][x].vx = x * 1024;
			grid[y][x].vz = y * 1024;
			grid[y][x].vy = 0;
		}
	}
}

void Ground::Draw(RenderContext& ctx, RECT& screen_clip) {
	int p;

	for (int y = 0; y < 8; y++) {
		for (int x = 0; x < 10; x++) {
			auto poly = ctx.new_primitive<POLY_FT4>();

			setPolyFT4(poly);
			// Load first three vertices to GTE
			gte_ldv3(
				&grid[y][x],
				&grid[y][x + 1],
				&grid[y + 1][x]);

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
			gte_ldv0(&grid[y + 1][x + 1]);
			gte_rtps();
			gte_stsxy(&poly->x3);

			if (quad_clip(&screen_clip,
				(DVECTOR*)&poly->x0, (DVECTOR*)&poly->x1,
				(DVECTOR*)&poly->x2, (DVECTOR*)&poly->x3))
				continue;

			gte_avsz4();
			gte_stotz(&p);

			setRGB0(poly, 128, 128, 128);
			setTPage(poly, 1, 1, 640, 0);
			setClut(poly, 640, 480);
			setUV4(poly, mapPtr[(y * 10) + x].u, mapPtr[(y * 10) + x].v,
				mapPtr[(y * 10) + x].u + 63, mapPtr[(y * 10) + x].v,
				mapPtr[(y * 10) + x].u, mapPtr[(y * 10) + x].v + 63,
				mapPtr[(y * 10) + x].u + 63, mapPtr[(y * 10) + x].v + 63);
		}
	}
}