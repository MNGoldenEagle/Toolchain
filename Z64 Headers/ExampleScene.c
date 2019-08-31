#include "scenes.h"

SceneCommand SCENE_LIST[] = {
	{.scenes = {.cmd = SCN_CMD_MULTI,.address = SCENE_SEGMENT(&sceneHeaders)}}
};

SceneCommand CHILD_SCENE[] = {
	{.starts = {.cmd = SCN_CMD_START_POS,.length = sizeof(starts),.address = SCENE_SEGMENT(&starts)}},
	{.sounds = {.cmd = SCN_CMD_SOUND,.reverb = 3,.nightSound = SFX_ENV_WIND_CRICKETS,.music = 0}},
	{.rooms = {.cmd = SCN_CMD_ROOMS,.length = sizeof(childRooms),.address = SCENE_SEGMENT(&childRooms)}},
	{.transitions = {.cmd = SCN_CMD_XACTORS,.length = sizeof(childTransitions),.address = SCENE_SEGMENT(&childTransitions)}},
	{.options = {.cmd = SCN_CMD_OPTIONS,.cameraMode = SCN_CAM_DEFAULT,.locationId = SCN_LOC_GANON_CASTLE}},
	{.collision = {.cmd = SCN_CMD_COLLISION,.address = SCENE_SEGMENT(0)}},
	{.entrances = {.cmd = SCN_CMD_ENTRANCES,.address = SCENE_SEGMENT(0)}}
};

SceneCommand ADULT_SCENE[] = {

};

SegmentAddr sceneHeaders[] = {
	DIRECT_SEGMENT(0),
	SCENE_SEGMENT(&CHILD_SCENE),
	DIRECT_SEGMENT(0),
	SCENE_SEGMENT(&ADULT_SCENE)
};

ActorDefinition starts[] = {
};

RoomEntry childRooms[] = {

};

TransitionActor childTransitions[] = {
};