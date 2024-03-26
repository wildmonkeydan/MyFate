/*
 * PSn00bSDK C++ basic graphics example
 * (C) 2020-2023 Lameguy64, spicyjpeg - MPL licensed
 *
 * A C++ variant of the beginner/hello example showcasing the use of classes and
 * templates in place of structures, making the code more readable and less
 * error-prone. The OT and primitive buffer are now allocated on the heap and
 * automatically freed when the RenderContext class is destroyed or goes out of
 * scope.
 *
 * See the original example for more details.
 */

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <psxgpu.h>

#include "render.h"
#include "cd.h"
#include "data.h"

/* Main */

static constexpr int SCREEN_XRES = 320;
static constexpr int SCREEN_YRES = 240;

int main(int argc, const char **argv) {
	// Initialize the GPU and load the default font texture provided by
	// PSn00bSDK at (960, 0) in VRAM.
	ResetGraph(0);
	FntLoad(960, 0);

	// Set up our rendering context.
	RenderContext ctx;
	ctx.setup(SCREEN_XRES, SCREEN_YRES, 0, 0, 0);

	CD cdHandler;
	cdHandler.Init();

	uint32_t* mainData = cdHandler.LoadFile("\\EXPORT.BIN;1");
	Data gameData(mainData);

	int x  = 0, y  = 0;
	int dx = 1, dy = 1;

	for (;;) {
		// Update the position and velocity of the bouncing square.
		if (x < 0 || x > (SCREEN_XRES - 64))
			dx = -dx;
		if (y < 0 || y > (SCREEN_YRES - 64))
			dy = -dy;

		x += dx;
		y += dy;

		// Draw the square by allocating a TILE (i.e. untextured solid color
		// rectangle) primitive at Z = 1.
		auto tile = ctx.new_primitive<TILE>(1);

		setTile(tile);
		setXY0 (tile, x, y);
		setWH  (tile, 64, 64);
		setRGB0(tile, 255, 255, 0);

		// Draw some text in front of the square (Z = 0, primitives with higher
		// Z indices are drawn first).
		ctx.draw_text(8, 16, 0, "sfssdsfsggsiufsiuefiufigwiugfiusuguas");

		ctx.flip();
	}

	return 0;
}