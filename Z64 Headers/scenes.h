#pragma once

#include "types.h"
#include "memory.h"
#include "sound.h"

struct Game;

typedef enum LevelCommand {
	SCN_CMD_START_POS,
	MAP_CMD_ACTORS,
	SCN_CMD_CAMERAS,
	SCN_CMD_COLLISION,
	SCN_CMD_ROOMS,
	MAP_CMD_WIND,
	SCN_CMD_ENTRANCES,
	SCN_CMD_KEEP,
	MAP_CMD_OPTIONS,
	MAP_CMD_UNUSED,
	MAP_CMD_DLISTS,
	MAP_CMD_OBJECTS,
	MAP_CMD_LIGHTS,
	SCN_CMD_PATHS,
	SCN_CMD_XACTORS,
	SCN_CMD_LIGHTS,
	MAP_CMD_TIME,
	SCN_CMD_SKYBOX,
	MAP_CMD_SKYBOX,
	SCN_CMD_EXITS,
	MAP_CMD_END,
	SCN_CMD_END = MAP_CMD_END,
	SCN_CMD_SOUND,
	MAP_CMD_SOUND,
	SCN_CMD_CUTSCENES,
	MAP_CMD_MULTI,
	SCN_CMD_MULTI = MAP_CMD_MULTI,
	SCN_CMD_OPTIONS
} LevelCommand;

typedef enum SceneMessageData {
	SCN_MSG_NONE,
	SCN_MSG_WORLD,
	SCN_MSG_DUNGEON
} SceneMessageData;

typedef enum SceneKeepId {
	SCN_KEEP_NONE,
	SCN_KEEP_GLOBAL,
	SCN_KEEP_WORLD,
	SCN_KEEP_DUNGEON
} SceneKeepId;

typedef enum SkyboxId {
	SKY_NONE,
	SKY_DEFAULT,
	SKY_HYLIAN_BAZAAR,
	SKY_FIXED_CLOUDS,
	SKY_MARKET_RUINS,
	SKY_DARK_STORM,
	SKY_UNUSED,
	SKY_LINK_HOME,
	SKY_UNUSED,
	SKY_MARKET_DAY,
	SKY_MARKET_NIGHT,
	SKY_MASK_SHOP,
	SKY_KNOWITALL_HOUSE,
	SKY_UNUSED,
	SKY_TWINS_HOUSE,
	SKY_STABLE,
	SKY_STEW_LADY_HOUSE,
	SKY_KOKIRI_SHOP,
	SKY_UNUSED,
	SKY_GORON_SHOP,
	SKY_ZORA_SHOP,
	SKY_UNUSED,
	SKY_KAKARIKO_POTION_SHOP,
	SKY_HYRULE_POTION_SHOP,
	SKY_BOMB_SHOP,
	SKY_UNUSED,
	SKY_DOG_LADY_HOUSE,
	SKY_IMPA_HOUSE,
	SKY_GERUDO_TENT,
	SKY_ENV_COLOR_ONLY,
	SKY_UNUSED,
	SKY_UNUSED,
	SKY_MIDO_HOUSE,
	SKY_SARIA_HOUSE,
	SKY_OLD_MAN_HOUSE
} SkyboxId;

typedef enum SceneCameraMode {
	SCN_CAM_DEFAULT,
	SCN_CAM_BKGD_FIXED_2,
	SCN_CAM_BKGD_PIVOT_1ST,
	SCN_CAM_BKGD_FIXED,
	SCN_CAM_BKGD_PIVOT,
	SCN_CAM_SHOOTING_GALLERY
} SceneCameraMode;

typedef enum SceneLocationId {
	SCN_LOC_HYRULE_FIELD,
	SCN_LOC_KAKARIKO,
	SCN_LOC_GRAVEYARD,
	SCN_LOC_ZORA_RIVER,
	SCN_LOC_KOKIRI_FOREST,
	SCN_LOC_FOREST_MEADOW,
	SCN_LOC_LAKE_HYLIA,
	SCN_LOC_ZORA_DOMAIN,
	SCN_LOC_ZORA_FOUNTAIN,
	SCN_LOC_GERUDO_VALLEY,
	SCN_LOC_LOST_WOODS,
	SCN_LOC_DESERT_COLOSSUS,
	SCN_LOC_GERUDO_FORTRESS,
	SCN_LOC_HAUNTED_WASTELAND,
	SCN_LOC_HYRULE_MARKET,
	SCN_LOC_HYRULE_CASTLE,
	SCN_LOC_DEATH_MTN_TRAIL,
	SCN_LOC_DEATH_MTN_CRATER,
	SCN_LOC_GORON_CITY,
	SCN_LOC_LON_LON_RANCH,
	SCN_LOC_WINDMILL,
	SCN_LOC_GANON_CASTLE,
	SCN_LOC_NO_CHANGE
} SceneLocationId;

typedef struct SceneStartPosHeader {
	LevelCommand cmd : 8;
	u8 length;
	s16 _padding;
	SegmentAddr address;
} SceneStartPosHeader;

typedef struct SceneCameraHeader {
	LevelCommand cmd : 8;
	u8 length;
	s16 _padding;
	SegmentAddr address;
} SceneCameraHeader;

typedef struct SceneCollisionHeader {
	LevelCommand cmd : 8;
	SegmentAddr address;
} SceneCollisionHeader;

typedef struct SceneRoomsHeader {
	LevelCommand cmd : 8;
	u8 length;
	SegmentAddr address;
} SceneRoomsHeader;

typedef struct SceneEntrancesHeader {
	LevelCommand cmd : 8;
	SegmentAddr address;
} SceneEntrancesHeader;

typedef struct SceneKeepHeader {
	LevelCommand cmd : 8;
	SceneMessageData messages : 8;
	s16 _padding1;
	s16 _padding2;
	SceneKeepId keep : 16;
} SceneKeepHeader;

typedef struct ScenePathsHeader {
	LevelCommand cmd : 8;
	SegmentAddr address;
} ScenePathsHeader;

typedef struct SceneTransitionsHeader {
	LevelCommand cmd : 8;
	u8 length;
	SegmentAddr address;
} SceneTransitionsHeader;

typedef struct SceneLightsHeader {
	LevelCommand cmd : 8;
	u8 length;
	SegmentAddr address;
} SceneLightsHeader;

typedef struct SceneSkyboxHeader {
	LevelCommand cmd : 8;
	s8 _padding1;
	s16 _padding2;
	SkyboxId skyboxId : 8;
	u8 _padding3 : 4;
	bool overcast : 4;
	u8 _padding4 : 4;
	bool ignoreClock : 4;
	u8 _padding5;
} SceneSkyboxHeader;

typedef struct SceneExitsHeader {
	LevelCommand cmd : 8;
	SegmentAddr address;
} SceneExitsHeader;

typedef struct SceneSoundsHeader {
	LevelCommand cmd : 8;
	s8 _padding1 : 4;
	u8 reverb : 4;
	s16 _padding2;
	s16 _padding3;
	EnvironmentSound nightSound : 8;
	u8 daySequence : 8;
} SceneSoundsHeader;

typedef struct SceneCutscenesHeader {
	LevelCommand cmd : 8;
	SegmentAddr address;
} SceneCutscenesHeader;

typedef struct HeaderList {
	LevelCommand cmd : 8;
	SegmentAddr address;
} HeaderList;

typedef struct SceneOptionsHeader {
	LevelCommand cmd : 8;
	SceneCameraMode cameraMode : 4;
	s16 _padding1;
	s16 _padding2;
	s8 _padding3;
	SceneLocationId location : 8;
} SceneOptionsHeader;

typedef struct Terminator {
	LevelCommand cmd : 8;
	s8 _padding1;
	s32 _padding2;
} Terminator;

typedef union SceneCommand {
	HeaderList scenes;
	SceneStartPosHeader starts;
	SceneCameraHeader cameras;
	SceneCollisionHeader collision;
	SceneRoomsHeader rooms;
	SceneEntrancesHeader entrances;
	SceneKeepHeader keep;
	ScenePathsHeader paths;
	SceneTransitionsHeader transitions;
	SceneLightsHeader lights;
	SceneSkyboxHeader skybox;
	SceneExitsHeader exits;
	SceneSoundsHeader sounds;
	SceneCutscenesHeader cutscenes;
	SceneOptionsHeader options;
	Terminator base;
	u64 raw;
} SceneCommand;

typedef struct RoomEntry {
	u32 start;
	u32 end;
} RoomEntry;

typedef struct Entrance {
	u8 startPosIdx;
	u8 roomIdx;
} Entrance;

typedef struct Path {
	// The length of the array pointed to by the address.
	s8 length;
	// Points to an array of Vec3s that consists of the points of the path.
	SegmentAddr address;
} Path;

typedef struct TransitionActor {
	u8 frontRoom;
	u8 frontCameraMode;
	u8 backRoom;
	u8 backCameraMode;
	s16 actorId;
	Vec3s position;
	s16 yaw;
	u16 variable;
} TransitionActor;

typedef struct Lighting {
	RGB24 ambient;
	RGB24 diffuse1;
	Vec3b diffuse1Normal;
	RGB24 diffuse2;
	Vec3b diffuse2Normal;
	RGB24 fog;
	s16 fogDistance;
	s16 drawDistance;
} Lighting;

typedef struct ActorDefinition {
	s16 actorId;
	Vec3s position;
	Vec3s rotation;
	u16 variable;
} ActorDefinition;

typedef s16 Exit;

// Scene Functions
extern void SpawnScene(Game* game, s32 scene_index, s32 start_index);
