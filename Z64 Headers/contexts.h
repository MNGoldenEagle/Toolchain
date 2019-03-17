#pragma once

#include "types.h"
#include "actors.h"
#include "controller.h"
#include "graphics.h"
#include "collision.h"
#include "cutscenes.h"

// An entry in the actor table.
typedef struct ActorTableEntry {
	// The number of actor instances have been created for this type.
	u32 count;
	// A pointer to the first actor instance in this category.
	BasicActor* first;
} ActorTableEntry;

// Game state for tracking actor information.
typedef struct ActorContext {
	/* 0x00 */ u8 padding[8];
	           // The total number of actors loaded.
	/* 0x04 */ u8 actorsLoaded;
	/* 0x05 */ u8 padding2[3];
	           // A table of all allocated actor instances, sorted by type (see ActorType enum).
	/* 0x08 */ ActorTableEntry actorTable[12];
	/* 0x6C */ u8 padding3[0x38];
	           // The actor tracked by the Z-target reticle.
	/* 0xA4 */ BasicActor* followActor;
	           // The actor currently Z-targeted.
	/* 0xA8 */ BasicActor* targetedActor;
	/* 0xAC */ u8 padding4[0x58];
} ActorContext;

// Game state for transition actor handling.
typedef struct TransitionActorContext {
	/* 0x00 */ u32 padding;
	           // The number of transition actors in the current scene.
	/* 0x04 */ u8 count;
	/* 0x05 */ u8 padding2[3];
	           // A pointer to the list of transition actors as defined in the scene file.
	/* 0x08 */ void* actorDefs;
	/* 0x0C */ void(*ptr1)();  /* points to function on Link's actor: ROM 0x00C1594C */
	/* 0x10 */ void(*ptr2)(); /* points to function on Link's actor: ROM 0x00C17E3C */
	/* 0x14 */ void(*ptr3)(); /* points to function on Link's actor: ROM 0x00C21D68 */
	/* 0x18 */ void(*ptr4)();
	/* 0x1C */ void(*ptr5)();
	/* 0x20 */ void(*ptr6)();
	/* 0x24 */ void(*ptr7)();
	/* 0x28 */ void(*ptr8)();
	/* 0x2C */ void(*ptr9)();
	/* 0x30 */ u8 padding3[0x84];
} TransitionActorContext;

// Game state for tracking the overlay resource files loaded into memory.
typedef struct ObjectContext {
	u8 padding[0x518];
} ObjectContext;

typedef struct BufferContext {
	u8 padding[0x3F4];
} BufferContext;

typedef enum StateLifecycle {
	SWITCH_STATE,
	RUN_STATE
} StateLifecycle;

// Global state used across all game modes.
typedef struct GlobalContext {
	           // The graphics context for storing display lists and interacting with the RDP.
	/* 0x00 */ Graphics* graphics;
	           // The update function for the current game mode.
	/* 0x04 */ void(*stateUpdate)();
	           // The uninitialization function for the current game mode.
	/* 0x08 */ void(*stateUninitialize)();
	           // The initialization function for the next game mode (for transitioning between game modes).
	/* 0x0C */ void(*nextInitializer)();
	           // Size of the initializing game mode's context (including global context, set to 0 after mode is initialized).
	/* 0x10 */ void* size;
	           // The state of controller 1.
	/* 0x14 */ Controller controller1;
	           // The state of controller 2.
	/* 0x2C */ Controller controller2;
	           // The state of controller 3.
	/* 0x44 */ Controller controller3;
	           // The state of controller 4.
	/* 0x5C */ Controller controller4;
	           // The total size of the current game mode's heap.
	/* 0x74 */ u32 heapSize;
	           // Pointer to the beginning of the heap.
	/* 0x78 */ void* heap;
	           // Pointer to the next section within the heap to allocate to.
	/* 0x7C */ void* heapAppendStart;
	           // Pointer to the end of the unallocated section with the heap.
	/* 0x80 */ void* heapAppendEnd;
	           // Pointer to a heap node.
	/* 0x84 */ void* stateHeapNode;
	           // Pointer to a heap node.
	/* 0x88 */ void* stateHeapNode2;
	           // Pointer to a heap node.
	/* 0x8C */ void* stateHeapNode3;
	/* 0x90 */ void* unknown0;
	/* 0x94 */ void* unknown1;
	           // Whether the game mode is executing or ready to switch.
	/* 0x98 */ StateLifecycle lifecycle;
	           // Number of frames since the current game mode was initialized.
	/* 0x9C */ s32 stateLifetime;
	/* 0xA0 */ s32 unknown;
} GlobalContext;

// Stores current state for the active game camera.
typedef struct CameraContext {
	            // The dimensions of the screen.
	/* 0x000 */ Rect screen;
	            // The draw distance of the camera.
	/* 0x010 */ f32 cameraDist;
	            // The distance for the fog effect, if active.
	/* 0x014 */ f32 fogDist;
	/* 0x018 */ f32 zDist;
	/* 0x01C */ f32 unk_0xDC;
	/* 0x020 */ u8 padding[0x190];
	            // The actor the camera will follow behind.
	/* 0x1B0 */ BasicActor* followActor;
	/* 0x1B4 */ u8 padding2[0x19B0];
} CameraContext;

// Tracks the flags used in the game.
typedef struct FlagsContext { /* 0x1D28 */
	           // The flags used by switches and toggle-based actors.
	/* 0x00 */ u32 switches;
	           // Temporary flags used by switches and toggle-based actors.
	/* 0x04 */ u32 switches2;
	           // Flags that are specific to scenes.  Appears to be unused.
	/* 0x08 */ u32 unusedScenes;
	           // Flags that are specific to scenes.  Appears to be unused.
	/* 0x0C */ u32 unusedScenes2;
	           // Flags for tracking opened chests.
	/* 0x10 */ u32 chests;
	           // Flags for tracking whether a room has been cleared.  May also be used for other room-based flags.
	/* 0x14 */ u32 rooms;
	           // Flags for an unspecified purpose.
	/* 0x18 */ u32 unknown;
	           // Flags for tracking scene-specific flags, such as collected items.
	/* 0x1C */ u32 scenes;
	           // Flags for tracking scene-specific flags.
	/* 0x20 */ u32 scenes2;
} FlagsContext;

// Tracks the currently visible title card.
typedef struct TitleCardContext { /* 0x01D4C */
                // Pointer to the 8-bit IA texture for the title card.
	/* 0x000 */ void* titleCard;
	            // The horizontal position of the title card.
	/* 0x004 */ u16 x;
	            // The vertical position of the title card.
	/* 0x006 */ u16 y;
	            // The width of the title card's texture.
	/* 0x008 */ u8 texWidth;
	            // The height of the title card's texture.
	/* 0x009 */ u8 texHeight;
	            // The number of frames the title card will remain visible on the screen.
	/* 0x00A */ u8 time;
	/* 0x00B */ u8 padding;
	            // The alpha level of the title card.  0 = transparent, ~0xFF is fully visible.
	/* 0x00C */ s16 alpha;
	            // The intensity level of the title card.  0 = black, ~0xFF is white.
	/* 0x00E */ s16 intensity;
} TitleCardContext;

// The current state of the cutscene.
typedef enum CutsceneState {
	OFF,
	INIT,
	RUNNING,
	STOP
} CutsceneState;

// Stores information about the currently loaded cutscene.
typedef struct CutsceneContext {
	/* 0x000 */ u32 padding;
	            // The currently loaded cutscene script.
	/* 0x004 */ void* currentCutscene;
	            // The current state of the cutscene (see CutsceneState).
	/* 0x008 */ u8 state;
	/* 0x009 */ u8 padding3[7];
	            // The current frame of the cutscene.
	/* 0x010 */ u16 currentFrame;
	/* 0x012 */ u8 padding4[10];
	            // The current focus point for the camera.
	/* 0x01C */ void* currentCameraFocus;
	            // The current point for the camera.
	/* 0x020 */ void* currentCameraPoint;
	/* 0x024 */ u8 padding5[8];
	            // Relevant actor subcommands as specified in the script for the current frame.
	/* 0x02C */ ActorSubCommand* actorSubcommand[3];
} CutsceneContext;

// Stores information about the currently loaded room.
typedef struct RoomContext {
	          // The room index in the currently loaded scene for the loaded room.
	/* 0x0 */ u8 roomIndex;
	/* 0xB */ u8 padding[11];
	          // A pointer to the room descriptor.
	/* 0xC */ void* currentRoom;
} RoomContext;

// Tracks information regarding textboxes and messaging.
typedef struct TextBoxContext {
	u8 padding[0x120];
} TextBoxContext;

// Stores information about the HUD and other interface elements.
typedef struct InterfaceContext {
	u8 padding[0x270];
} InterfaceContext;

// Stores information about Link's animations.
typedef struct LinkAnimContext {
	/* 0x0 */ u8 padding[0xC84];
} LinkAnimContext;

// The state for the main game.
typedef struct Game {
	              // The global state (valid across all game modes).
	/* 0x00000 */ GlobalContext globals;
	              // The index for the currently loaded scene.  If changing the scene, this should be updated prior to triggering the load sequence.
	/* 0x000A4 */ u16 currentScene;
	/* 0x000A6 */ u8 padding[0x1A];
	              // The state of the game camera.
	/* 0x000C0 */ CameraContext cameras;
	              // The state of actors loaded in the game.
	/* 0x01C24 */ ActorContext actors;
	              // The state of non-persistent flags.
	/* 0x01D28 */ FlagsContext flags;
	              // The state of the title card.
	/* 0x01D4C */ TitleCardContext titleCard;
	/* 0x01D5C */ u32 padding2[2];
	              // The state of the current cutscene.
	/* 0x01D64 */ CutsceneContext cutscenes;
	/* 0x01D9C */ u8 padding3[0xE634];
	              // The state of the text box system.
	/* 0x103D0 */ TextBoxContext messages;
	              // The state of the HUD and other interface elements.
	/* 0x104F0 */ InterfaceContext interface;
	/* 0x10760 */ u8 padding4[0x3C0];
	              // The state of Link's animation.
	/* 0x10B20 */ LinkAnimContext playerAnim;
	              // The state of the currently loaded resources.
	/* 0x117A4 */ ObjectContext objects;
	              // The state of the currently loaded room.
	/* 0x11CBC */ RoomContext rooms;
	/* 0x11CCC */ u8 padding5[0x64];
	              // The state of the transition actors for the scene.
	/* 0x11D30 */ TransitionActorContext transitions;
	              // The frame counter for the current game mode (e.g., not file select, map select, or paused).
	/* 0x11DE4 */ u32 playFrameCounter;
	              // A boolean tracking whether Link is a child.  
	/* 0x11DE8 */ u8 linkIsChild;
	/* 0x11DE9 */ u8 padding6;
	              // The current spawn index for the loaded scene.
	/* 0x11DEA */ u8 spawnIndex;
	              // The total number of actors to load for the current map (NULLed after map is loaded).
	/* 0x11DEB */ u8 numMapActorsToLoad;
	              // Number of rooms that are defined in the loaded scene file.
	/* 0x11DEC */ u32 roomCount;
	/* 0x11DF0 */ u32 padding7[2];
	              // Pointer to the array of actor definitions in the map definition.
	/* 0x11DF8 */ void** mapActors;
	/* 0x11DFC */ u32 padding8[2];
	              // Pointer to the exit list for the current scene definition.
	/* 0x11E04 */ void* sceneExits;
	/* 0x11E08 */ u8 padding9[0x12];
	              // The entrance index to use when triggering loading the next area.
	/* 0x11E1A */ u16 loadEntranceIndex;
	/* 0x11E1C */ u8 padding10[0x44];
	/* 0x11E60 */ CollisionAT collisionAT;
	/* 0x11F2C */ CollisionAC collisionAC;
	/* 0x12020 */ CollisionOT collisionOT;
	/* 0x12124 */ BufferContext buffers;
} Game;

typedef struct SceneFlags {
	/* 0x00 */ u32 chests;
	/* 0x04 */ u32 switches;
	/* 0x08 */ u32 clearedRooms;
	/* 0x0C */ u32 collectibles;
	/* 0x10 */ u32 unknown;
	/* 0x14 */ u32 visitedRooms;
	/* 0x18 */ u32 visitedFloors;
} SceneFlags;

typedef struct WarpData {
	/* 0x00 */ f32 x;
	/* 0x04 */ f32 y;
	/* 0x08 */ f32 z;
	/* 0x0C */ u16 yaw;
	/* 0x0E */ u16 sceneIndex;
	/* 0x10 */ u32 mapIndex;
	/* 0x14 */ u32 activated;
} WarpData;

// The save file, also used to track overall game state.
typedef struct SaveFile {
	/* 0x0000 */ s32 entranceIndex; // The entrance index denoting where Link will load into.
	/* 0x0004 */ s32 linkIsChild; // If TRUE, Link will be a child.
	/* 0x0008 */ s16 padding;
	/* 0x000A */ s16 cutsceneNumber; // If < 0, indicates the scene cutscene to play.  -16 = cutscene 0, -15 = cutscene 1, and so on, to cutscene 15.
	/* 0x000C */ s16 currentTime; // The current timestamp.
	/* 0x0010 */ s32 isNight; // Will be set to TRUE when the currentTime reaches "night".
	/* 0x0014 */ u32 padding2[2];
	/* 0x001C */ s8 fileId[6]; // Must be set to ZELDAZ or the file will be considered corrupt.
	/* 0x0022 */ u16 deathCount; // Number of times Link has died on the file.
	/* 0x0024 */ s8 linkName[8]; // Link's name.  Note that letters in the buffer not set will be 0xDF.
	/* 0x002C */ s16 n64ddFile; // Indicates the file requires the N64DD to be plugged in.
	/* 0x002E */ s16 heartContainers; // Number of heart pieces, where 0x10 == 1 heart piece.
	/* 0x0030 */ s16 hearts; // Number of hearts Link has, where 0x10 == 1 heart.
	/* 0x0032 */ s8 magicMeterSize; // Magic meter type. 0 == no magic, 1 == normal meter, 2 == extended meter.
	/* 0x0033 */ s8 magicAmount; // Amount of magic Link starts with. 0x30 == half bar, 0x60 == full bar.
	/* 0x0034 */ u16 rupees; // Number of rupees Link has collected.
	/* 0x0036 */ u16 padding3;
	/* 0x0038 */ s16 naviTimer; // Increments each update cycle; Navi's reminder is triggered at 0x257.  Resets to 0 if below 0xBB8 when loading a new map.
	/* 0x003A */ s8 magicUpgrade; // First magic upgrade retrieved.
	/* 0x003B */ s8 padding4;
	/* 0x003C */ s8 magicUpgrade2; // Second magic upgrade retrieved.
	/* 0x003E */ s8 giantSwordUpgrade2; // Second giant sword upgrade retrieved.
	/* 0x0040 */ s16 padding5[20];
	/* 0x0066 */ s16 startSceneIndex; // Index of scene to spawn in when file is loaded.
	/* 0x0068 */ s8 equips[4]; // Stores what items have been equipped to each item (B, C-Left, C-Down, C-Right)
	/* 0x006C */ s8 equipIdx[3]; // Stores the indexes of the items in the inventory screen equipped by C-Left, C-Down, and C-Right.
	/* 0x0070 */ u16 equipped; // Stores which equipment is equipped. (0xF000 &= boots, 0x0F00 &= tunics, 0x00F0 &= shields, 0x000F &= swords)
	/* 0x0072 */ u16 padding6;
	/* 0x0074 */ s8 inventory[24]; // Stores the current inventory, from top left to bottom right.
	/* 0x008C */ s8 ammoCount[16]; // Stores the current amount of the item (or its ammo) is available.
	/* 0x009C */ u16 equipment; // Stores what equipment Link possesses. (0x7000 &= boots, 0x0700 &= tunics, 0x0070 &= shields, 0x000F &= swords)
	/* 0x009E */ u16 padding7;
	/* 0x00A0 */ u32 equipmentFlags; // Stores the equipment and/or upgrades Link possesses. (e.g., deku stick/nut upgrades, Zora scale, quivers)
	/* 0x00A4 */ u32 questItems; // Stores flags for tracking quest items.
	/* 0x00A8 */ s8 dungeonItems[12]; // Stores flags for tracking dungeon items. ( 0x01 == boss key, 0x02 == compass, 0x04 == dungeon map)
	/* 0x00B4 */ s32 padding8[2];
	/* 0x00BC */ s8 dungeonKeys[19]; // Stores the number of small keys for each dungeon.  -1 indicates that no key icon should be shown.
	/* 0x00CF */ s8 hardenedHearts; // Number of 'double defense' hearts.  Can be greater than the total number of hearts.
	/* 0x00D0 */ u16 skulltulaTokens; // Number of gold skulltula tokens collected.
	/* 0x00D2 */ s16 padding9;
	/* 0x00D4 */ SceneFlags sceneFlags[101]; // Persisted flags for each scene.
	/* 0x0B0C */ s32 padding10[161];
	/* 0x0E64 */ WarpData savedWarp; // The warp point that was last set by the player (typ. via Farore's Wind).
	/* 0x0E7C */ s32 padding11[22];
	/* 0x0ED4 */ u16 eventFlags[14]; // Flags for tracking various events and story-driven checkpoints.
	/* 0x0EF0 */ u16 itemFlags[4]; // Flags for tracking whether certain items have been obtained for the first time.
	/* 0x0EF8 */ u16 npcFlags[30]; // Flags used by actors and NPCs for tracking Link's interaction with them.
	/* 0x0F34 */ s16 padding12[527];
	/* 0x1352 */ u16 checksum; // The computed checksum for the persisted save file (everything before the checksum).
	/* 0x1354 */ s32 fileIndex; // The index of the selected save file, where 0 is file 1.  If 0xFF, then saving is disabled.
	/* 0x1358 */ s32 padding13[8];
	/* 0x1378 */ u16 grottoEntranceIndex; // The entrance index to use when leaving a grotto.
} SaveFile;