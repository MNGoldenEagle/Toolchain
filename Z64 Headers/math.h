#pragma once

#include "types.h"

/** Random Number Generation **/

// Returns a randomly generated signed short.  The range of the short generated is: [offset, offset + range)
extern s16 GetShortRandom(s16 offset, s16 range);

extern f32 coss(u16 angle);
extern f32 cosf(f32 angle);
extern f32 sins(u16 angle);
extern f32 sinf(f32 angle);
extern f32 sqrtf(f32 value);
