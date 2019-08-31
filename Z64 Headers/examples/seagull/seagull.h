#pragma once
#include <types.h>
#include <actors.h>
#include <animation.h>
#include <math.h>

#ifdef _MSC_BUILD
#define __attribute__(B) /* Do nothing */
#endif

typedef struct SeagullActor {
	BasicActor base;
	AnimationSkeletonData skeleton;
	s16 angle;
	s16 flap;
	s16 anim;
	s16 speed;
} SeagullActor;

void constructor(SeagullActor*, Game*);
void update(SeagullActor*, Game*);
void draw(SeagullActor*, Game*);

static const ActorInit INIT __attribute__((used)) = {
	.actorID = 55,
	.actorType = ACT_TYP_NPC,
	.roomNumber = ACT_RM_UNDEF,
	.flags = ACT_FLG_TARGETABLE | ACT_FLG_FORCE_DRAW | ACT_FLG_FORCE_UPDATE,
	.objectID = 134,
	.instanceSize = sizeof(SeagullActor),
	.constructor = constructor,
	.destructor = NULL,
	.update = update,
	.draw = draw
};
