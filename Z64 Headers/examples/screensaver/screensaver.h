#pragma once
#include <types.h>
#include <actors.h>
#include <debug.h>

#ifdef _MSC_BUILD
#define __attribute__(B) /* Do nothing */
#endif

void constructor(BasicActor*, Game*);
void destructor(BasicActor*, Game*);
void update(BasicActor*, Game*);
void draw(BasicActor*, Game*);

typedef struct ScreenSaverActor {
	BasicActor base;
	u16 x;
	u16 y;
	bool forward;
	bool downward;
	u8 color;
} ScreenSaverActor;

static const ActorInit INIT __attribute__((used)) = {
	.actorID = 0x31,
	.actorType = ACT_TYP_MISC,
	.roomNumber = ACT_RM_UNDEF,
	.flags = 0,
	.objectID = 1,
	.instanceSize = sizeof(ScreenSaverActor),
	.constructor = constructor,
	.destructor = destructor,
	.update = update,
	.draw = draw
};
