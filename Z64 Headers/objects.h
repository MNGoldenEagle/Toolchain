#pragma once

#include "types.h"
#include "contexts.h"

extern s16 GetObjectIndex(ObjectContext* objects, s32 objectID);
extern bool CheckObjectLoaded(ObjectContext* objects, s32 index);
extern void SpawnObject(ObjectContext* objects, u16 objectID);
