#include "ground.h"
#include "utils.h"

#include <inline_c.h>
#include <stdlib.h>

static constexpr Background::Type backgrounds[] = { Background::Type::Void, Background::Type::Void, Background::Type::Sky, Background::Type::Vapour, Background::Type::Sky, Background::Type::Void, Background::Type::Void, Background::Type::Void, Background::Type::Sky };
static constexpr unsigned char ambientLighting[] = { 64, 64, 128, 128, 128, 96, 96, 96, 128 };

Ground::Ground(Data& dat, SMD* npcModel, SMD* questionModel) {
	mapPtr = (Tile*)dat.GetLevel(0);

	for (int y = 0; y < 9; y++) {
		for (int x = 0; x < 11; x++) {
			grid[y][x].vx = x * 1024;
			grid[y][x].vz = y * 1024;
			grid[y][x].vy = 0;
		}
	}

	numNpcs = *(dat.GetLevel(0) + 320);

	if (numNpcs != 0) {
		npcs = (NPC*)malloc(sizeof(NPC) * numNpcs);
	}

	for (int i = 0; i < numNpcs; i++) {
		npcs[i] = NPC(dat.GetNpc(*(dat.GetLevel(0) + (321 + i))), npcModel, questionModel);
	}
}

void Ground::Draw(RenderContext& ctx, RECT& screen_clip, Camera& cam) {
	int p;
	POLY_FT4* poly;

	poly = (POLY_FT4*)ctx._next_packet;

	for (int y = 0; y < 8; y++) {
		for (int x = 0; x < 10; x++) {
			
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
			

			setPolyFT4(poly);

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

			setRGB0(poly, ambientLighting[currentRoom], ambientLighting[currentRoom], ambientLighting[currentRoom]);
			setTPage(poly, 1, 1, 640, 0);
			setClut(poly, 320, 472);
			setUV4(poly, mapPtr[(y * 10) + x].u, mapPtr[(y * 10) + x].v,
				mapPtr[(y * 10) + x].u + 63, mapPtr[(y * 10) + x].v,
				mapPtr[(y * 10) + x].u, mapPtr[(y * 10) + x].v + 63,
				mapPtr[(y * 10) + x].u + 63, mapPtr[(y * 10) + x].v + 63);


			addPrim(ctx._buffers[ctx._active_buffer]._ot + p, poly);
			poly++;
		}
	}

	ctx._next_packet = (uint8_t*)poly;


	for (int i = 0; i < numNpcs; i++) {
		npcs[i].Draw(ctx, cam);
	}
}

void Ground::SwitchLevel(Data& dat, SMD* npcModel, SMD* question, int level) {
	mapPtr = (Tile*)dat.GetLevel(level);

	free(npcs);

	numNpcs = *(dat.GetLevel(level) + 320);
	//printf("%d", numNpcs);

	if (numNpcs != 0) {
		npcs = (NPC*)malloc(sizeof(NPC) * numNpcs);
	}

	for (int i = 0; i < numNpcs; i++) {
		printf("%d\n", *(dat.GetLevel(level) + (321 + i)));
		npcs[i] = NPC(dat.GetNpc(*(dat.GetLevel(level) + (321 + i))), npcModel, question);
	}

	currentRoom = level;
	gte_SetBackColor(ambientLighting[currentRoom], ambientLighting[currentRoom], ambientLighting[currentRoom]);
}

void Ground::Update(Player& ply, SMD* npcModel, SMD* questionModel, Data& dat, Background& back, Dialouge& diag) {
	RECT collsions;

	collsions.x = ply.collision.x >> 10;
	collsions.y = ply.collision.y >> 10;
	collsions.w = (ply.collision.x + ply.collision.w) >> 10;
	collsions.h = (ply.collision.y + ply.collision.h) >> 10;


	if (collsions.x < 0)
		collsions.x = 0;

	if (collsions.y < 0)
		collsions.y = 0;

	if (collsions.w > 9)
		collsions.w = 9;

	if (collsions.h > 7)
		collsions.h = 7;


	for (int i = collsions.x; i <= collsions.w; i++) {
		for (int j = collsions.y; j <= collsions.h; j++) {
			Tile* tile = &mapPtr[(j * 10) + i];

			if (tile->exit) {
				SwitchLevel(dat, npcModel, questionModel, tile->exitLevel);
				int moveTileX = i;
				int moveTileY = j + 1;

				if (j == 0) {
					moveTileY = 5;
				}
				else if (j == 7) {
					moveTileY = 2;
				}

				if (i == 0) {
					moveTileX = 7;
				}
				else if (i == 9)
				{
					moveTileX = 2;
				}

				ply.position.vx = moveTileX << 10;
				ply.position.vz = moveTileY << 10;

				back.SwitchType(backgrounds[tile->exitLevel]);

				break;
			}
		}
	}

	for (int i = 0; i < numNpcs; i++) {
		if (npcs[i].IsNear(ply.collision) && ply.pad->IsFaceButtonDown() && !diag.talking) {
			diag.Talk(npcs[i].GetDialougeOffset(*ply.pad));
		}
	}
}