#pragma once

#include "types.h"
#include "memory.h"
#include "contexts.h"

/** General Animation Routines **/

// Returns the number of frames that are part of the referenced animation data.
extern s32 GetAnimationFrameCount(SegmentAddr animation);

// Returns the percentage of completion between the start and end values based on the current value.  0.0 == 0%, 1.0 == 100%.
extern f32 CalcFrameCompletion(s16 end, s16 start, s16 current);

/** Actor-Specific Routines **/

extern void SetupJointsAndAnimations(Game* context, void* anim_data, SegmentAddr joints, SegmentAddr animations, void* joint_struct, void* anim_struct, s32 limbTotal); // limbTotal + 1

extern void SetupJointsAndAnimWithMtx(Game* context, void* anim_data, SegmentAddr joints, SegmentAddr animations);

// Changes the animation being played by the actor.  Transition must be [0.0, 1.0f).
extern void ChangeActorAnimation(void* anim_data, SegmentAddr newAnimation, f32 speed, f32 a3, f32 frameCnt, u32 unknown, f32 transition);

// Update's the animation's state, incrementing it by one or more frames (based on animation speed).
extern s32 UpdateActorAnimation(void* anim_data);

// Checks if the animation is at the frame value specified.  Note that while it is a floating point argument, the frames are always integral values.
extern bool CheckIfAnimAtFrame(void* anim_data, f32 frame);
