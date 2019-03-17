#pragma once

#include "types.h"

typedef struct CollisionAT {
	u8 padding[0xCC];
} CollisionAT;

typedef struct CollisionAC {
	u8 padding[0xF4];
} CollisionAC;

typedef struct CollisionOT {
	u8 padding[0x104];
} CollisionOT;
