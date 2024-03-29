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
#include "player.h"

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


	// CD stuff
	CD cdHandler;
	cdHandler.Init();



	// Camera stuff
	VECTOR cam_pos, cam_rot;
	// Camera default coordinates
	setVector(&cam_pos, ONE * 3401, ONE * 1468, ONE * 9608);
	setVector(&cam_rot, ONE * -6136, 0, 0);
	Camera cam(cam_pos, cam_rot);
	RECT	screen_clip;
	// Set clip region
	setRECT(&screen_clip, 0, 0, SCREEN_XRES, SCREEN_YRES);
	scSetClipRect(screen_clip.x, screen_clip.y, screen_clip.w, screen_clip.h);

	// PAD stuff
	Pad pad = Pad();

	FntOpen(0, 8, 320, 216, 0, 100);


	// Loading files
	TIM_IMAGE img;
	LoadTexture((u_long*)cdHandler.LoadFile("\\TILES.TIM;1"), &img);

	uint32_t* mainData = cdHandler.LoadFile("\\EXPORT.BIN;1");
	Data gameData(mainData);

	uint32_t* npcData = cdHandler.LoadFile("\\NPC.SMD;1");
	SMD* npcModel = (SMD*)npcData;
	smdInitData(npcModel);

	Ground test(gameData, npcModel);

	Player player = Player();

	for (;;) {

		player.Update(pad, cam);
		test.Update(player, npcModel, gameData);

		cam.Update(pad, ctx);
		test.Draw(ctx, screen_clip, cam);
		player.Draw(ctx, cam, screen_clip);
		

		// Draw some text in front of the square (Z = 0, primitives with higher
		// Z indices are drawn first).
		//ctx.draw_text(8, 16, 0, "sfssdsfsggsiufsiuefiufigwiugfiusuguas\nusgfuiegfiugsfgsiegui");

		ctx.flip();
	}

	return 0;
}