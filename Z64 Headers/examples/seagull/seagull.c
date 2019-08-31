/**
 * seagull.c
 *
 * A simple seagull actor that flies in circles/ellipses.
 *
 * by Dr.Disco <z64.me>
 *
 * variable structure (hex)
 * xyzz
 * x = elliptical factor (0x0 = circle,  0xF = really skinny ellipse)
 * y = speed-1           (0x0 = slowest, 0xF = fastest)
 * z = ((radius/64)-1)|(direction<<7) (max value is 0x7F, rolls over to minimum starting at 0x80)
 * z & 0x80 = direction (whether z is >= 0x80 controls clockwisedness)
 *
 * variable structure (binary)
 * xxxx yyyy w zzzzzzz
 * x = elliptical factor (0 = circle,  15 = really skinny ellipse)
 * y = speed-1           (0 = slowest, 15 = fastest)
 * w = clockwisedness
 * z = (radius/64)-1
 **/

#include "seagull.h"

// Hierarchy
static const SegmentAddr SKEL_BIRD = {
	.segment = SEG_OBJECT,
	.offset = 0xE68
};

// Animations

// Gliding animation
static const SegmentAddr ANIM_GLIDE = {
	.segment = SEG_OBJECT,
	.offset = 0x58
};

// Flap wings
static const SegmentAddr ANIM_FLAP = {
	.segment = SEG_OBJECT,
	.offset = 0x168
};

// Animation IDs
#define ANIM_GLIDE_ID  0
#define ANIM_FLAP_ID   1

/* .text */
void constructor(SeagullActor* actor, Game* game) {
	// initialize skeleton for drawing, with animation
	SetupJointsAndAnimations(game, &actor->skeleton, SKEL_BIRD, ANIM_GLIDE, NULL, NULL, 0);

	// scale actor's object model to 1/1000
	SetActorUniformScale(&actor->base, 0.01f);

	// initialize internal entity components
	actor->angle = 0;
	actor->flap = 100;
	actor->anim = ANIM_GLIDE_ID;

	// clockwisedness flag in actor variable
	f32 clockwisedness = 1;
	if (actor->base.actorVariable & 0x80)
		clockwisedness = -1;

	// tilt in direction of turn
	actor->base.rotation2.z = 15 * 182 * clockwisedness;

	// speed flag
	actor->speed = clockwisedness * (((actor->base.actorVariable >> 8) & 0xF) + 1) * 182;

	// scale speed for larger radii
	actor->speed /= ((actor->base.actorVariable & 0x7F) + 1) * 2;
}

void update(SeagullActor* actor, Game* game) {
	f32 length, x, z;

	// get radius
	length = ((actor->base.actorVariable & 0x7F) + 1) * 64;

	// calculate x and z (note that z uses elliptical scale factor)
	x = actor->base.position.x + length * coss(actor->angle);
	z = (16.0f - (actor->base.actorVariable >> 12)) / 16.0f;
	z *= actor->base.position.z + length * sins(actor->angle);

	// make seagull face the proper direction
	actor->base.rotation2.y = -actor->angle;

	// turn it the other way (180 degrees) if it's going counterclockwise
	if (actor->speed < 0) {
		actor->base.rotation2.y += 180 * 182;
	}

	// set actor location in world
	actor->base.position2.x = x;
	actor->base.position2.z = z;
	actor->base.position3.x = x;
	actor->base.position3.z = z;
	actor->angle += actor->speed;

	// animation timer expired, alternate the animation
	if (!--actor->flap) {
		SegmentAddr anim;

		if (actor->anim == ANIM_GLIDE_ID) {
			anim = ANIM_FLAP;
			actor->anim = ANIM_FLAP_ID;

			// play the wing flapping animation (20 frames) either 1, 2, or 3 times,
			// buffering 4 frames at the end to smoothly go back to gliding afterwards
			actor->flap = 20 * GetShortRandom(1, 3) + 4;
		} else {
			anim = ANIM_GLIDE;
			actor->anim = ANIM_GLIDE_ID;
			// glide for a duration of 30 to 60 frames
			actor->flap = GetShortRandom(30, 30);
		}

		// immediately switch to the desired animation
		ChangeActorAnimation(&actor->skeleton, anim, 1.0, 0, 0, 0, 0);
	}
}

void draw(SeagullActor* actor, Game* game) {
	// update the transformation matrices for each limb, based on the current animation frame
	UpdateActorAnimation(&actor->skeleton);

	// draw a matrix-enabled skeleton
	DrawAnimatedActor(game, actor->skeleton.limbIndex, actor->skeleton.unknown3, actor->skeleton.dlistCount, NULL, NULL, &actor->base);
}
