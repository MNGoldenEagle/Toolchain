#pragma once

#include "types.h"
#include "memory.h"
#include "graphics.h"

struct Game;
struct ObjectContext;

// Actor category types
typedef enum ActorType {
	ACT_TYP_SWITCH, // Switch/puzzle actors (buttons, torches, crystals, etc.)
	ACT_TYP_PROP1,  // Large set pieces
	ACT_TYP_PLAYER, // Playable characters (typically only contains Link)
	ACT_TYP_BOMB,   // Bombs and bombchus
	ACT_TYP_NPC,    // Interactive NPCs
	ACT_TYP_ENEMY,  // Enemies and minibosses
	ACT_TYP_PROP2,  // Simple set pieces
	ACT_TYP_ITEM,   // Items, collectibles, and arrows
	ACT_TYP_MISC,   // Miscellaneous actors
	ACT_TYP_BOSS,   // Bosses and associated actors
	ACT_TYP_DOOR,   // Transition actors
	ACT_TYP_CHESTS  // Chests
} ActorType;

// The flags that can be applied to an actor.
typedef enum ActorFlags {
	NONE = 0x00000000,
	TARGETABLE = 0x00000001, // Actor can be Z-targeted
	APPROACH_MUSIC = 0x00000004, // Play enemy music when actor is near Link (requires TARGETABLE)
	FORCE_UPDATE = 0x00000010, // Force update function to run regardless of proximity to camera
	FORCE_DRAW = 0x00000020  // Force draw function to run regardless of proximity to camera
} ActorFlags;

// The common, basic structure of an actor.
typedef struct BasicActor {
	/* 0x000 */ u16 actorNumber;           // The index of the actor in the actor table.
	/* 0x002 */ u8 actorType;              // The type of actor according to ActorType.
	/* 0x003 */ u8 roomNumber;             // The room index for the current map the actor is in.  If set to FF, the actor will always remain loaded.
	/* 0x004 */ ActorFlags flags;          // The flags applied to the actor.
	/* 0x008 */ Vec3f position;
	/* 0x014 */ Vec3s initialRotation;
	/* 0x01A */ u16 unk_0x01A;
	/* 0x01C */ u16 actorVariable;         // The actor's variable.
	/* 0x01E */ s8 objectIndex;            // The index of the object associated to this actor in the objects context.
	/* 0x01F */ u8 unk_0x01F;
	/* 0x020 */ u16 sfxIndex;
	/* 0x022 */ u16 unk_0x022;
	/* 0x024 */ Vec3f position2;
	/* 0x030 */ Vec3s accelNormal;
	/* 0x036 */ u16 Unk_0x036;
	/* 0x038 */ Vec3f position3;
	/* 0x044 */ Vec3s rotation;
	/* 0x04A */ u16 unk_0x04A;
	/* 0x04C */ f32 unk_0x04C;
	/* 0x050 */ Vec3f scale;               // The current scale of the actor (typically around 0.01).
	/* 0x05C */ Vec3f velocity;
	/* 0x068 */ f32 horizVelocity;
	/* 0x06C */ f32 gravityAccel;
	/* 0x070 */ f32 minYVelocity;
	/* 0x074 */ void* unk_0x074;
	/* 0x078 */ void* unk_0x078;
	/* 0x07C */ u8 unk_0x07C;
	/* 0x07D */ u8 unk_0x07D;
	/* 0x07E */ u16 unk_0x07E;
	/* 0x080 */ f32 unk_0x080;
	/* 0x084 */ f32 unk_0x084;
	/* 0x088 */ u16 unk_0x088;
	/* 0x08A */ s16 giveItemAngle;
	/* 0x08C */ f32 unk_0x08C;
	/* 0x090 */ f32 distanceFromPlayer;    // The distance between this actor and Link.
	/* 0x094 */ f32 unk_0x094;
	/* 0x098 */ void* damageChart;
	/* 0x09C */ Vec3f velocity2;
	/* 0x0A8 */ s16 unk_0x0A8;
	/* 0x0AA */ s16 unk_0x0AA;
	/* 0x0AC */ s16 unk_0x0AC;
	/* 0x0AE */ u8 unk_0x0AE;
	/* 0x0AF */ u8 health;
	/* 0x0B0 */ u8 damage;
	/* 0x0B1 */ u8 damageEffect;
	/* 0x0B2 */ u8 impactEffect;
	/* 0x0B3 */ u8 unk_0x0B3;
	/* 0x0B4 */ Vec3s rotation2;
	/* 0x0BA */ u8 unk_0x0BA;
	/* 0x0BB */ u8 unk_0x0BB;
	/* 0x0BC */ f32 unk_0x0BC;
	/* 0x0C0 */ void(*postDrawFunc)();
	/* 0x0C4 */ f32 unk_0x0C4;
	/* 0x0C8 */ u8 unk_0x0C8;
	/* 0x0C9 */ u8 unk_0x0C9;
	/* 0x0CA */ u16 unk_0x0CA;
	/* 0x0CC */ Vec3f unk_0x0CC;
	/* 0x0D8 */ Vec3f unk_0x0D8;
	/* 0x0E4 */ Vec3f unk_0x0E4; /* based on position and something in transition actor context (0x11D60) */
	/* 0x0F0 */ f32 unk_0x0F0;
	/* 0x0F4 */ f32 unk_0x0F4;
	/* 0x0F8 */ f32 unk_0x0F8;
	/* 0x0FC */ u32 unk_0x0FC;
	/* 0x100 */ Vec3f position4;
	/* 0x10C */ s8 unk_0x10C;
	/* 0x10D */ s8 unk_0x10D;
	/* 0x10E */ u16 interactText;          // The message displayed when Link talks to the actor.
	/* 0x110 */ s16 frozen; /* used when redeads are frozen */
	/* 0x112 */ u16 unk_0x112;
	/* 0x114 */ u8 unk_0x114;
	/* 0x115 */ u8 active;                 // The active state for the actor.  If set to 1, the actor's update() function will be called.  If set to 0, update() will not be called.
	/* 0x116 */ u8 unk_0x116;
	/* 0x117 */ s8 naviText;               // The text Navi will show when C-Up is pressed.
	/* 0x118 */ struct BasicActor* attachParent;       // The actor that this actor is a child of.
	/* 0x11C */ struct BasicActor* attachChild;        // The child actor of this actor.
	/* 0x120 */ struct BasicActor* prevActor;          // The previous actor of the same actor type.  If this is the first actor of that type, then this will be NULL.
	/* 0x124 */ struct BasicActor* nextActor;          // The next actor of the actor type.  If this is the last actor of that type, then this will be NULL.
	/* 0x128 */ void(*constructFunc)(BasicActor*, Game*);   // The constructor function of this actor.  Called when the actor instance is spawned.
	/* 0x12C */ void(*destructFunc)(BasicActor*, Game*); // The destructor function of this actor.  Called when the room/map/scene is unloaded, or if the actor is manually despawned.
	/* 0x130 */ void(*updateFunc)(BasicActor*, Game*); // The update function of this actor.  Will be called on each frame to allow the actor to update its position.
	/* 0x134 */ void(*drawFunc)(BasicActor*, Game*);   // The draw function of this actor.  Will be called on each frame to allow the actor to render any relevant display lists.
	/* 0x138 */ void* overlayEntry;        // Points to the actor overlay entry in the overlay table.
} BasicActor;

#define ACT_RM_UNDEF -1

// Actor initialization data
typedef struct ActorInit {
	/* 0x00 */ s16 actorID;         // The actor's index in the actor table
	/* 0x02 */ u8 actorType;        // The type of actor (see ActorType enum)
	/* 0x03 */ s8 roomNumber;       // The room number the actor is assigned to, can leave at -1 for initial definition
	/* 0x04 */ ActorFlags flags;    // The default behavioral flags to set on new instances of this actor
	/* 0x08 */ s16 objectID;		// The base resource file ID to load when the actor is loaded
	/* 0x0A */ s16 padding;
	/* 0x0C */ s32 instanceSize;    // The amount of memory to allocate per instance of this actor
	/* 0x10 */ void (*constructor)(BasicActor*, Game*); // Pointer to the constructor function for the actor
	/* 0x14 */ void (*destructor)(BasicActor*, Game*);  // Pointer to the destructor function for the actor
	/* 0x18 */ void (*update)(BasicActor*, Game*);      // Pointer to the update function for the actor
	/* 0x1C */ void (*draw)(BasicActor*, Game*);        // Pointer to the draw function for the actor
} ActorInit;

typedef enum InitType {
	S8_STOP,
	U8_STOP,
	S16_STOP,
	U16_STOP,
	S32_STOP,
	U32_STOP,
	F32_STOP,
	F32_1000_STOP,
	S8,
	VEC3F = S8,
	U8,
	VEC3F_1000 = U8,
	S16,
	VEC3S = S16,
	U16,
	S32,
	U32,
	F32,
	F32_1000
} InitType;

// Initializes values within the actor instance.  This format can be used to
// easily initialize actor instances with default values without using up space
// in the initializer.  The compact initialization format has limitations regarding
// datatypes, and cannot store values larger than 0xFFFF in a integral value.
//
// Note that the last entry in the compact initializer script must use a _STOP
// postfixed datatype.
typedef struct CompactInit {
	u8 dataType : 4; // The type of value to set (see InitType)
	u16 offset : 12; // The offset within the actor instance structure
	s16 value;       // The value to assign
} CompactInit;

/** Item spawning **/

// Spawns a collectible item 
extern void SpawnCollectibleItem(Game* game, Vec3f* position, s16 dropId);
extern void SpawnRandomCollectible(Game* game, BasicActor* actor, Vec3f* positions, s16 dropTableId);
// Spawns a new actor and attaches it as a child to the provided actor instance.
extern BasicActor* SpawnAndAttachActor(ObjectContext* objects, BasicActor* owner, Game* context, s32 actorNumber, f32 x, f32 y, f32 z, s16 rotX, s16 rotY, s16 rotZ, u16 initVariable);
// Spawns a new actor.
extern BasicActor* SpawnActor(ObjectContext* objects, Game* context, s32 actorNumber, f32 x, f32 y, f32 z, s16 rotX, s16 rotY, s16 rotZ, u16 initVariable);

/** Actor Draw Routines **/

extern void InitRenderer(u64* gfx, Graphics* gfxContext, u8* filename, s32 line);
extern void ActorDLStart(Graphics* gfxContext);
extern void DrawAnimatedActor(Game* game, SegmentAddr** skeleton_data, void* anim_data, s32 unk0, s32 unk1, s32 unk2);
extern void ActorDLEnd(u64* gfx, Graphics* gfxContext, u8* filename, s32 line);

/** Actor Manipulation Routines **/

// Disables the actor but does not deallocate it from memory.  Sets the update() and draw() functions to NULL, effectively preventing it from using any CPU/RCP time.
extern void DisableActor(BasicActor* actor);
extern void SetActorHeight(BasicActor* instance, f32 offsetY);
// Sets the actor's scaling factor to a uniform value.  Typical scale value is 0.001.
extern void SetActorUniformScale(BasicActor* actor, f32 factor);
extern void InitializeActorShadow(void* shadow_data, f32 a1, void* shadowDrawFunc, f32 a3);
// Allocates and pushes a new matrix to the matrix stack and returns a pointer to the newly created matrix.
extern Mtx* AllocateModelMatrix(Graphics* gfx, u8* filename, u32 lineno);
// Rotates the specified matrix around the X axis (roll).  If useCurrentMatrix is false, the topmost matrix in the stack will be rotated instead.
extern void RotateByRadiansX(f32 radians, bool useCurrentMatrix);
// Rotates the specified matrix around the Y axis (yaw).  If useCurrentMatrix is false, the topmost matrix in the stack will be rotated instead.
extern void RotateByRadiansY(f32 radians, bool useCurrentMatrix);
// Translates the specified matrix along the axes by the given amounts.  If useCurrentMatrix is false, the topmost matrix will be translated instead.
extern void TranslateMatrix(f32 x, f32 y, f32 z, bool useCurrentMatrix);
// Scales the specified matrix along the axes by the given amounts.  If useCurrentMatrix is false, the topmost matrix will be scaled instead.
extern void ScaleMatrix(f32 x, f32 y, f32 z, bool useCurrentMatrix);
// Returns TRUE if the provided actor is attached (a child) to another actor, FALSE otherwise.
extern bool IsActorAttached(BasicActor* instance, Game* context);

/** Combat-Related Routines **/

// Returns TRUE if the following conditions are met:
// * Link is within the specified distance to the actor.
// * If Link is attacking with his sword or Deku stick. (Note: 0x843, if == FF then jump attack, if == 1 then slash attack, == 0 when not attacking or stab attack)
// * If Link is facing in the angle toward the actor within the given delta.
// * If the actor is facing toward Link within the given delta.
// If any of the above conditions are not met, the function will return FALSE.
extern bool IsLinkAttackingActor(Game* context, BasicActor* instance, f32 distance, f32 actorYawDelta, s16 linkAngleDelta, s16 actorYaw);

/** Game Manipulation Routines **/

// The type of respawn.
typedef enum RespawnType {
	VOID_OUT, // Used when respawning due to hitting a void plane.
	GROTTO,   // Used when returning Link back from where he entered a grotto.
	WARP      // Used when returning Link back to where he cast Farore's Wind.
} RespawnType;

// Sets the entrance index to use when Link is spawned based on the specified respawn type.
extern void SetRespawnPoint(Game* context, RespawnType type, s32 entranceIndex);

/** Actor Lifecycle Routines **/

// Parses the compact initialization script and initializes the provided actor instance according to the instructions in the script.
extern void InitializeCompactFormat(BasicActor* instance, CompactInit* configuration);
// Assigns the actor's loaded resource to the SEG_OBJECT entry in the segment table.
extern void SetupActorObjectSeg(Game* context, BasicActor* instance);
