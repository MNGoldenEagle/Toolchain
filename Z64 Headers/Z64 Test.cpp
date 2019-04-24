#include "contexts.h"
#include <cstdio>

int calcOffset(void*, void*);
void describeSaveFile();
void describeGame();

static const Gfx dlist[1] = { gsDPSetRenderMode(G_RM_FOG_PRIM_A, G_RM_AA_ZB_OPA_SURF2) };

int main()
{
	printf("gsDPSetRenderMode(G_RM_FOG_PRIM_A, G_RM_AA_ZB_OPA_SURF2) == %08x %08x\n", dlist->words.w0, dlist->words.w1);
}

void describeSaveFile() {
	SaveFile file;
	printf("sizeof(SaveFile) == %08x\n", sizeof(SaveFile));
	printf("sizeof(SceneFlags) == %08x\n", sizeof(SceneFlags));
	printf("sizeof(WarpData) == %08x\n", sizeof(WarpData));
	printf("cutsceneNumber -> %08x\n", calcOffset(&file, &file.cutsceneNumber));
	printf("fileId -> %08x\n", calcOffset(&file, &file.fileId));
	printf("rupees -> %08x\n", calcOffset(&file, &file.rupees));
	printf("magicUpgrade2 -> %08x\n", calcOffset(&file, &file.magicUpgrade2));
	printf("startSceneIndex -> %08x\n", calcOffset(&file, &file.startSceneIndex));
	printf("inventory -> %08x\n", calcOffset(&file, &file.inventory));
	printf("dungeonItems -> %08x\n", calcOffset(&file, &file.dungeonItems));
	printf("dungeonKeys -> %08x\n", calcOffset(&file, &file.dungeonKeys));
	printf("sceneFlags -> %08x\n", calcOffset(&file, &file.sceneFlags));
	printf("savedWarp -> %08x\n", calcOffset(&file, &file.savedWarp));
	printf("eventFlags -> %08x\n", calcOffset(&file, &file.eventFlags));
	printf("itemFlags -> %08x\n", calcOffset(&file, &file.itemFlags));
	printf("npcFlags -> %08x\n", calcOffset(&file, &file.npcFlags));
	printf("padding12 -> %08x\n", calcOffset(&file, &file.padding12));
	printf("checksum -> %08x\n", calcOffset(&file, &file.checksum));
	printf("grottoEntranceIndex -> %08x\n", calcOffset(&file, &file.grottoEntranceIndex));
}

void describeGame() {
	Game game;
	Graphics grph;
	printf("game -> %08x\n", (int)&game);
	printf("OPA_DISP.append_end -> %08x\n", calcOffset(&grph, &grph.POLY_OPA_DISP.append_end));
	printf("cameras           -> %08x\n", calcOffset(&game, &game.cameras));
	printf("actors            -> %08x\n", calcOffset(&game, &game.actors));
	printf("flags             -> %08x\n", calcOffset(&game, &game.flags));
	printf("titleCard         -> %08x\n", calcOffset(&game, &game.titleCard));
	printf("cutscenes         -> %08x\n", calcOffset(&game, &game.cutscenes));
	printf("messages          -> %08x\n", calcOffset(&game, &game.messages));
	printf("interface         -> %08x\n", calcOffset(&game, &game.interface));
	printf("playerAnim        -> %08x\n", calcOffset(&game, &game.playerAnim));
	printf("objects           -> %08x\n", calcOffset(&game, &game.objects));
	printf("rooms             -> %08x\n", calcOffset(&game, &game.rooms));
	printf("transitions       -> %08x\n", calcOffset(&game, &game.transitions));
	printf("playFrameCounter  -> %08x\n", calcOffset(&game, &game.playFrameCounter));
	printf("loadEntranceIndex -> %08x\n", calcOffset(&game, &game.loadEntranceIndex));
	printf("collisionAT       -> %08x\n", calcOffset(&game, &game.collisionAT));
	printf("collisionAC       -> %08x\n", calcOffset(&game, &game.collisionAC));
	printf("collisionOT       -> %08x\n", calcOffset(&game, &game.collisionOT));
	printf("buffers           -> %08x\n", calcOffset(&game, &game.buffers));
}

int calcOffset(void* ptr1, void* ptr2) {
	int value1 = (int)ptr1;
	int value2 = (int)ptr2;
	return value2 - value1;
}