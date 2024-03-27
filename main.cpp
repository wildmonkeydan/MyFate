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
#include "ground.h"
#include "camera.h"
#include "utils.h"

/* Main */

int main(int argc, const char **argv) {
	int currentLevel = 0;
	int levelChangeTimer = -1;

	// Initialize the GPU and load the default font texture provided by
	// PSn00bSDK at (960, 0) in VRAM.
	ResetGraph(0);
	FntLoad(960, 0);

	// Set up our rendering context.
	RenderContext ctx;
	ctx.setup(SCREEN_XRES, SCREEN_YRES, 0, 0, 0);

	CD cdHandler;
	cdHandler.Init();

	VECTOR cam_pos, cam_rot;

	// Camera default coordinates
	setVector(&cam_pos, 0, ONE * -200, 0);
	setVector(&cam_rot, 0, 0, 0);

	Camera cam(cam_pos, cam_rot);

	RECT	screen_clip;
	// Set clip region
	setRECT(&screen_clip, 0, 0, SCREEN_XRES, SCREEN_YRES);

	Pad pad = Pad();

	FntOpen(0, 8, 320, 216, 0, 100);

	TIM_IMAGE img;
	LoadTexture((u_long*)cdHandler.LoadFile("\\TILES.TIM;1"), &img);

	uint32_t* mainData = cdHandler.LoadFile("\\EXPORT.BIN;1");
	Data gameData(mainData);

	Ground test(gameData.GetLevel(currentLevel));

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

		cam.Update(pad, ctx);
		test.Draw(ctx, screen_clip);

		if (levelChangeTimer >= 0) {
			levelChangeTimer++;

			if (levelChangeTimer > 30) {
				levelChangeTimer = -1;
			}
		}

		if (pad.IsButtonDown(PAD_START) && currentLevel < 8 && levelChangeTimer == -1) {
			currentLevel++;
			test = Ground(gameData.GetLevel(currentLevel));
			levelChangeTimer++;
		}

		if (pad.IsButtonDown(PAD_SELECT) && currentLevel > 0 && levelChangeTimer == -1) {
			currentLevel--;
			test = Ground(gameData.GetLevel(currentLevel));
			levelChangeTimer++;
		}

		// Draw the square by allocating a TILE (i.e. untextured solid color
		// rectangle) primitive at Z = 1.
		auto tile = ctx.new_primitive<TILE>(1);

		setTile(tile);
		setXY0 (tile, x, y);
		setWH  (tile, 64, 64);
		setRGB0(tile, 255, 255, 0);

		// Draw some text in front of the square (Z = 0, primitives with higher
		// Z indices are drawn first).
		//ctx.draw_text(8, 16, 0, "sfssdsfsggsiufsiuefiufigwiugfiusuguas\nusgfuiegfiugsfgsiegui");

		ctx.flip();
	}

	return 0;
}