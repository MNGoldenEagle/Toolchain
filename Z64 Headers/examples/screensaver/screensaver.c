#include "screensaver.h"

const RGBA32 COLORS[12] = {
	{255,0,0,255},   // Red
	{255,128,0,255}, // Orange
	{255,255,0,255}, // Yellow
	{128,255,0,255}, // Chartreuse
	{0,255,0,255},   // Green
	{0,255,128,255}, // Celadon
	{0,255,255,255}, // Cyan
	{0,128,255,255}, // Azure
	{0,0,255,255},   // Blue
	{128,0,255,255}, // Violet
	{255,0,255,255}, // Fuschia
	{255,0,128,255}  // Pink
};

static const s8 BOUNCE_TEXT[] = "BOUNCING TEXT IS BOUNCY";

#define MIN_X 0
#define MAX_X 27 - sizeof(BOUNCE_TEXT)
#define MIN_Y 0
#define MAX_Y 19
#define MAX_COLORS 12

void constructor(BasicActor* instance, Game* game) {
	ScreenSaverActor* actor = (ScreenSaverActor*)instance;
	actor->x = MIN_X;
	actor->y = MIN_Y;
	actor->forward = TRUE;
	actor->downward = TRUE;
	actor->color = 0;
}

void destructor(BasicActor* instance, Game* game) {
	// Do nothing
}

void update(BasicActor* instance, Game* game) {
	ScreenSaverActor* actor = (ScreenSaverActor*)instance;
	if (actor->forward) {
		int x = ++actor->x;
		if (x > MAX_X) {
			actor->forward = FALSE;
		}
	} else {
		int x = --actor->x;
		if (x < MIN_X) {
			actor->forward = TRUE;
		}
	}
	if (actor->downward) {
		int y = ++actor->y;
		if (y > MAX_Y) {
			actor->downward = FALSE;
		}
	} else {
		int y = --actor->y;
		if (y < MIN_Y) {
			actor->downward = TRUE;
		}
	}
	actor->color = (actor->color + 1) % MAX_COLORS;
}

void draw(BasicActor* instance, Game* game) {
	ScreenSaverActor* actor = (ScreenSaverActor*)instance;
	GfxState state;
	InitRenderer(&state, game->globals.graphics, __FILE__, __LINE__);
	DebugText debugText;
	InitDebugText(&debugText);
	debugText.dlist = game->globals.graphics->POLY_OPA_DISP.append;
	OpenDebugText(&debugText, &debugText.dlist);
	SetTextPos(&debugText, actor->x, actor->y);
	RGBA32 color = COLORS[actor->color];
	SetTextRGBA(&debugText, color.r, color.g, color.b, color.a);
	SetTextString(&debugText, BOUNCE_TEXT);
	CloseDebugText(&debugText);
	ActorDLEnd(&state, game->globals.graphics, __FILE__, __LINE__);
}
