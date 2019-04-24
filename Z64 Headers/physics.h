#pragma once

#include "contexts.h"

// The type of respawn.
typedef enum RespawnType {
	SPAWN_VOID_OUT, // Used when respawning due to hitting a void plane.
	SPAWN_GROTTO,   // Used when returning Link back from where he entered a grotto.
	SPAWN_WARP      // Used when returning Link back to where he cast Farore's Wind.
} RespawnType;

// Sets the entrance index to use when Link is spawned based on the specified respawn type.
extern void SetRespawnPoint(Game* context, RespawnType type, s32 entranceIndex);
