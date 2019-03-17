#pragma once

#include "types.h"
#include "contexts.h"

extern u32 TestSwitchFlag(Game* game, u32 flag); // flag := [0, 31] targets temp switch flags, [32, 63] targets persistent switch flags
extern void SwitchFlagOn(Game* game, u32 flag);
extern void SwitchFlagOff(Game* game, u32 flag);
extern u32 TestUnusedSceneFlag(Game* game, u32 flag); // flag := [0, 63]
extern void UnusedSceneFlagOn(Game* game, u32 flag);
extern void UnusedSceneFlagOff(Game* game, u32 flag);
extern u32 TestChestFlag(Game* game, u32 flag); // flag := [0, 31]
extern void ChestFlagOn(Game* game, u32 flag);
extern u32 TestRoomFlag(Game* game, u32 flag); // flag := [0, 31]
extern void RoomFlagOn(Game* game, u32 flag);
extern void RoomFlagOff(Game* game, u32 flag);
extern u32 TestUnknownFlag(Game* game, u32 flag); // flag := [0, 31], not sure if persistent
extern void UnknownFlagOn(Game* game, u32 flag);
extern void UnknownFlagOff(Game* game, u32 flag);
extern u32 TestSceneFlag(Game* game, u32 flag); // flag := [0, 63] called "collectible" flags, but may extend beyond that
extern void SceneFlagOn(Game* game, u32 flag);
