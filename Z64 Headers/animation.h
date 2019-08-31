#pragma once

#include "types.h"
#include "memory.h"
#include "contexts.h"

typedef struct AnimationSkeletonData {
	/* 0x00 */ u8 limbCount;
	/* 0x01 */ u8 mode;
	/* 0x02 */ u8 dlistCount;
	/* 0x03 */ u8 _padding2;
	/* 0x04 */ u32 limbIndex;
	/* 0x08 */ SegmentAddr currentAnimation;
	/* 0x0C */ s32 unknown1;
	/* 0x10 */ f32 totalFrames;
	/* 0x14 */ f32 unknown2;
	/* 0x18 */ f32 currentFrame;
	/* 0x1C */ f32 playbackSpeed;
	/* 0x20 */ u32 unknown3;
	/* 0x24 */ u32 unknown4;
	/* 0x28 */ u32 unknown5;
	/* 0x2C */ u32 unknown6;
	/* 0x30 */ void(*funcPtr)(); // Unknown return values or parameters, requires investigation.
	/* 0x34 */ s32 unknown7;
	/* 0x38 */ s32 unknown8;
	/* 0x3C */ u16 unknown9;
	/* 0x3E */ u16 _padding3;;
} AnimationSkeletonData;

// Plays the animation continuously.  Frame offset and count are ignored (animation count is always used).
#define ANIM_MODE_DEFAULT 0
// Plays the segment of animation specified.  Animation blending is not enabled in this mode.
#define ANIM_MODE_SEGMENT 2
// Plays the segment of animation specified.  Animation blending is permitted in this mode.
#define ANIM_MODE_BLEND 4

typedef struct AnimationDef {
	SegmentAddr animation;
	f32 speed;
	f32 frameOffset;
	f32 frameCnt;
	u8 mode;
	u32 transition;
} AnimationDef;

/** General Animation Routines **/

// Returns the number of frames that are part of the referenced animation data.
extern s32 GetAnimationFrameCount(SegmentAddr animation);

// Returns the percentage of completion between the start and end values based on the current value.  0.0 == 0%, 1.0 == 100%.
extern f32 CalcFrameCompletion(s16 end, s16 start, s16 current);

/** Actor-Specific Routines **/

extern void SetupJointsAndAnimations(Game* context, void* animData, SegmentAddr joints, SegmentAddr animations, void* jointStruct, void* animStruct, s32 limbTotal); // limbTotal + 1

extern void SetupJointsAndAnimWithMtx(Game* context, void* animData, SegmentAddr joints, SegmentAddr animations);

// Changes the animation being played by the actor.  Transition is used to blend between different animations; the value indicates the number of frames, positive or negative.
extern void ChangeActorAnimation(void* animData, SegmentAddr newAnimation, f32 speed, f32 frameOffset, f32 frameCnt, u32 mode, f32 transition);

// Changes the animation being played by the actor using the specified animation definition.
extern void ChangeActorAnimByIdx(AnimationSkeletonData* animData, AnimationDef* animations, int index);

// Updates the animation's state, incrementing it by one or more frames (based on animation speed).
extern s32 UpdateActorAnimation(void* animData);

// Draws an animated actor using the specified animation data.  Unknown parameters can be set to NULL.
extern void DrawAnimatedActorWithMtx(Game* game, SegmentAddr* skeletonData, void* animData, void* func0, void* func1);

// Draws an animated actor using the specified animation data.  Unknown parameters can be set to NULL.
// Originally called Si2_draw_SV in the game.
extern void DrawAnimatedActor(Game* game, SegmentAddr* skeletonData, void* animData, s32 limbCount, void* func0, void* func1, BasicActor* actor);

// Checks if the animation is at the frame value specified.  Note that while it is a floating point argument, the frames are always integral values.
extern bool CheckIfAnimAtFrame(void* animData, f32 frame);
