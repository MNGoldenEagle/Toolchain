#pragma once

#include "contexts.h"

typedef enum RespawnType {
	VOIDOUT,     // Position to respawn to when voiding out
	GROTTO,      // Position to respawn to when exiting grotto
	FARORES_WIND // Position to respawn to when using Farore's Wind
};

extern void SetRespawnPoint(Game* game, RespawnType type, s32 spawnVar);