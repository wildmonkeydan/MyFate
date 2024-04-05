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
#include <stdlib.h>

#include "render.h"
#include "cd.h"
#include "data.h"
#include "ground.h"
#include "camera.h"
#include "utils.h"
#include "player.h"
#include "dialouge.h"
#include "background.h"
#include "item.h"

enum class GameMode {
	Intro,
	Game,
	Item,
	End
};

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

	//FntOpen(0, 8, 320, 216, 0, 100);

	//
	// Loading files
	//

	// Textures

	TIM_IMAGE img;

	u_long* imgData = (u_long*)cdHandler.LoadFile("\\TILES.TIM;1");
	LoadTexture(imgData, &img);
	free(imgData);

	imgData = (u_long*)cdHandler.LoadFile("\\SKY.TIM;1");
	LoadTexture(imgData, &img);
	free(imgData);

	imgData = (u_long*)cdHandler.LoadFile("\\VAPOUR.TIM;1");
	LoadTexture(imgData, &img);
	free(imgData);

	// Data

	uint32_t* mainData = cdHandler.LoadFile("\\EXPORT.BIN;1");
	Data gameData(mainData);

	// Models

	uint32_t* npcData = cdHandler.LoadFile("\\NPC.SMD;1");
	SMD* npcModel = (SMD*)npcData;
	smdInitData(npcModel);

	uint32_t* questData = cdHandler.LoadFile("\\QUEST.SMD;1");
	SMD* questModel = (SMD*)questData;
	smdInitData(questModel);

	uint32_t* plyData = cdHandler.LoadFile("\\PLAY.SMD;1");
	SMD* plyModel = (SMD*)plyData;
	smdInitData(plyModel);

	Ground test(gameData, npcModel, questModel);
	Dialouge dialouge(gameData.GetString(0));
	Player player = Player(&pad, plyModel);
	Background back = Background();
	Item item = Item(DialougeItem::None, cdHandler);


	GameMode mode = GameMode::Game;
	DialougeItem itemE = DialougeItem::None;

	for (;;) {
		switch (mode) {

		case GameMode::Game:
			player.Update(cam);
			test.Update(player, npcModel, questModel, gameData, back, dialouge);
			itemE = dialouge.Update(pad);

			if (itemE != DialougeItem::None) {
				item = Item(itemE, cdHandler);
				mode = GameMode::Item;
			}

			back.Update();

			back.Draw(ctx);
			cam.Update(pad, ctx);
			test.Draw(ctx, screen_clip, cam);
			player.Draw(ctx, cam, screen_clip);
			dialouge.Draw(ctx);

			// Draw some text in front of the square (Z = 0, primitives with higher
			// Z indices are drawn first).
			//ctx.draw_text(8, 16, 0, "sfssdsfsggsiufsiuefiufigwiugfiusuguas\nusgfuiegfiugsfgsiegui");

			break;

		case GameMode::Item:
			if (item.Update(pad)) {
				mode = GameMode::Game;
				player.AddItem(item.Give());
			}

			item.Draw(ctx);

			break;
		}
		
		ctx.flip();
	}

	return 0;
}