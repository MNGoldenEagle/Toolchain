#pragma once

#include "types.h"
#include "memory.h"
#include "graphics.h"

typedef struct BasicActor BasicActor;
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
	ACT_FLG_NONE           = 0x00000000,
	ACT_FLG_TARGETABLE     = 0x00000001, // Actor can be Z-targeted
	ACT_FLG_APPROACH_MUSIC = 0x00000004, // Play enemy music when actor is near Link (requires TARGETABLE)
	ACT_FLG_FORCE_UPDATE   = 0x00000010, // Force update function to run regardless of proximity to camera
	ACT_FLG_FORCE_DRAW     = 0x00000020  // Force draw function to run regardless of proximity to camera
} ActorFlags;

typedef struct DamageEffect {
	u8 effect : 4;
	u8 damage : 4;
} DamageEffect;

typedef struct DamageChart {
	// Struck by a Deku nut.
	DamageEffect nuts;
	// Slashed by a Deku stick, or hit with a normal pot.
	DamageEffect sticks;
	// Hit with a Deku seed from a slingshot.
	DamageEffect slingshot;
	// Hit with a explosion (typ. from a bomb).
	DamageEffect bombs;
	// Hit with a boomerang.
	DamageEffect boomerang;
	// Hit with a typical arrow.
	DamageEffect arrow;
	// Hit with a hammer swing.
	DamageEffect hammer;
	// Struck by the hookshot or longshot.
	DamageEffect hookshot;
	// Slashed by a sword. (Kokiri, Master, Biggoron)
	DamageEffect swords[3];
	// Hit with a magic arrow. (fire, ice, light, plus three unused types)
	DamageEffect magicArrows[6];
	// Hit with a magical effect. (fire, ice, light)
	DamageEffect magicSpells[3];
	// Unknown damage types.
	DamageEffect unused[2];
	// Struck with a simple sword spin (non-magical or blue). (Kokiri, Biggoron, Master)
	DamageEffect basicSpin[3];
	// Slashed with a jump attack or charged sword spin. (Kokiri, Biggoron, Master)
	DamageEffect chargedSpin[3];
	// Unknown damage types.
	DamageEffect unused2[2];
	// Struck with a jump attack with the hammer.
	DamageEffect hammerJump;
	// Unknown damage type.
	DamageEffect unused3;
} DamageChart;

typedef struct ActorLife {
	/* 0x00 */ DamageChart* damageChart;
	/* 0x04 */ Vec3f pushbackVector;
	/* 0x10 */ s16 uA8;
	/* 0x12 */ s16 uAA;
	/* 0x14 */ s16 uAC;
	/* 0x16 */ u8 mass; // Determines how much the actor resists being pushed by Link or attacks.
	/* 0x17 */ u8 health;
} ActorLife;

typedef struct ActorLifeInit {
	/* 0x0 */ u8 health;
	/* 0x2 */ s16 u1;
	/* 0x4 */ s16 u2;
	/* 0x6 */ s16 u3;
	/* 0x8 */ u8 resistance;
} ActorLifeInit;

// The common, basic structure of an actor.
struct BasicActor {
	/* 0x000 */ u16 actorNumber;           // The index of the actor in the actor table.
	/* 0x002 */ u8 actorType;              // The type of actor according to ActorType.
	/* 0x003 */ u8 roomNumber;             // The room index for the current map the actor is in.  If set to FF, the actor will always remain loaded.
	/* 0x004 */ ActorFlags flags;          // The flags applied to the actor.
	/* 0x008 */ Vec3f position;            // Position used for collision detection.
	/* 0x014 */ Vec3s initialRotation;     // Initial rotation of the actor when spawned (based on map data or SpawnActor call).
	/* 0x01A */ u16 unk_0x01A;
	/* 0x01C */ u16 actorVariable;         // The actor's variable.
	/* 0x01E */ s8 objectIndex;            // The index of the object associated to this actor in the objects context.
	/* 0x01F */ u8 unk_0x01F;
	/* 0x020 */ u16 sfxIndex;              // Plays a positional sound effect at the actor's location. Only plays if update() is called.
	/* 0x022 */ u16 unk_0x022;
	/* 0x024 */ Vec3f position2;           // Current coordinates of the actor.
	/* 0x030 */ Vec3s directionVec;        // The directional vector that the actor is moving in.
	/* 0x036 */ u16 Unk_0x036;
	/* 0x038 */ Vec3f position3;
	/* 0x044 */ Vec3s rotation;            // The rotation of the actor, usually the same as the direction vector. Used for computation only; this value is not incorporated into matrix calculations on rendering.
	/* 0x04A */ u16 unk_0x04A;
	/* 0x04C */ f32 unk_0x04C;
	/* 0x050 */ Vec3f scale;               // The current scale of the actor (typically around 0.01).
	/* 0x05C */ Vec3f velocity;            // The velocity the actor is moving in.
	/* 0x068 */ f32 horizVelocity;         // The velocity of the actor along the horizontal plane (X and Z axes).
	/* 0x06C */ f32 gravityAccel;          // The gravitational acceleration to apply to the actor's vertical velocity on each frame.
	/* 0x070 */ f32 minYVelocity;          // Sets the minimum for vertical velocity.
	/* 0x074 */ void* wallPolygon;         // The "wall" collision polygon the actor is currently interacting with.
	/* 0x078 */ void* floorPolygon;        // The "floor" collision polygon the actor is currently interacting with.
	/* 0x07C */ u8 wallSource;             // Indicates the source of the wall collision polygon. (0x32 == Scene)
	/* 0x07D */ u8 floorSource;            // Indicates the source of the floor collision polygon. (0x32 == Scene)
	/* 0x07E */ u16 unk_0x07E;
	/* 0x080 */ f32 unk_0x080;
	/* 0x084 */ f32 unk_0x084;
	/* 0x088 */ u16 unk_0x088;
	/* 0x08A */ s16 angleToPlayer;         // The difference between this actor and Link.
	/* 0x08C */ f32 unk_0x08C;
	/* 0x090 */ f32 horizDistFromPlayer;   // The horizontal (X and Z axes) distance between this actor and Link.
	/* 0x094 */ f32 vertDistFromPlayer;    // The vertical distance between this actor and Link.
	/* 0x098 */ ActorLife life;
	/* 0x0B0 */ u8 damage;                 // Amount of damage dealt to the actor.
	/* 0x0B1 */ u8 damageEffect;           // The effect that corresponds to the weapon used by Link (see DamageEffect struct).
	/* 0x0B2 */ u8 impactEffect;           // Used to determine effects of projectile actors when colliding with an actor.
	/* 0x0B3 */ u8 unk_0x0B3;
	/* 0x0B4 */ Vec3s rotation2;           // The effective rotation of the actor.  This is usually used in matrix calculations during the render step.
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
	/* 0x100 */ Vec3f prevPosition;        // The previous position coordinates of the actor.
	/* 0x10C */ s8 unk_0x10C;
	/* 0x10D */ s8 unk_0x10D;
	/* 0x10E */ u16 interactText;          // The message displayed when Link talks to the actor.
	/* 0x110 */ s16 frozen;                // A generic counter that counts down on each frame automatically. Usually used for "frozen" effects.
	/* 0x112 */ u16 unk_0x112;
	/* 0x114 */ u8 unk_0x114;
	/* 0x115 */ bool alive;                // Whether the actor is alive. When TRUE, the update() and render() functions are called. When FALSE, update() and render() are no longer called and the actor can no longer do anything unless resurrected by another actor.
	/* 0x116 */ u8 unk_0x116;
	/* 0x117 */ s8 naviText;               // The text Navi will show when C-Up is pressed.
	/* 0x118 */ struct BasicActor* attachParent;       // The actor that this actor is a child of.
	/* 0x11C */ struct BasicActor* attachChild;        // The child actor of this actor.
	/* 0x120 */ struct BasicActor* prevActor;          // The previous actor of the same actor type.  If this is the first actor of that type, then this will be NULL.
	/* 0x124 */ struct BasicActor* nextActor;          // The next actor of the actor type.  If this is the last actor of that type, then this will be NULL.
	/* 0x128 */ void(*constructFunc)(BasicActor*, Game*);   // The constructor function of this actor.  Called when the actor instance is spawned.
	/* 0x12C */ void(*destructFunc)(BasicActor*, Game*);    // The destructor function of this actor.  Called when the room/map/scene is unloaded, or if the actor is manually despawned.
	/* 0x130 */ void(*updateFunc)(BasicActor*, Game*); // The update function of this actor.  Will be called on each frame to allow the actor to update its position.
	/* 0x134 */ void(*drawFunc)(BasicActor*, Game*);   // The draw function of this actor.  Will be called on each frame to allow the actor to render any relevant display lists.
	/* 0x138 */ void* overlayEntry;        // Points to the actor overlay entry in the overlay table.
};

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
	ACT_INIT_S8_STOP,
	ACT_INIT_U8_STOP,
	ACT_INIT_S16_STOP,
	ACT_INIT_U16_STOP,
	ACT_INIT_S32_STOP,
	ACT_INIT_U32_STOP,
	ACT_INIT_F32_STOP,
	ACT_INIT_F32_1000_STOP,
	ACT_INIT_S8,
	ACT_INIT_VEC3F = ACT_INIT_S8,
	ACT_INIT_U8,
	ACT_INIT_VEC3F_1000 = ACT_INIT_U8,
	ACT_INIT_S16,
	ACT_INIT_VEC3S = ACT_INIT_S16,
	ACT_INIT_U16,
	ACT_INIT_S32,
	ACT_INIT_U32,
	ACT_INIT_F32,
	ACT_INIT_F32_1000
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

typedef struct GfxState {
	Gfx* prevOpaq;
	Gfx* prevXlu;
	Gfx* prevDecal;
} GfxState;

extern void InitRenderer(GfxState* state, Graphics* gfxContext, s8* filename, s32 line);
extern void ActorDLStart(Graphics* gfxContext);
extern void ActorDLEnd(GfxState* state, Graphics* gfxContext, s8* filename, s32 line);

/** Actor Manipulation Routines **/

// Disables the actor but does not deallocate it from memory.  Sets the update() and draw() functions to NULL, effectively preventing it from using any CPU/RCP time.
extern void DisableActor(BasicActor* actor);
extern void SetActorHeight(BasicActor* instance, f32 offsetY);
// Sets the actor's scaling factor to a uniform value.  Typical scale value is 0.001.
extern void SetActorUniformScale(BasicActor* actor, f32 factor);
extern void InitializeActorShadow(void* shadow_data, f32 a1, void* shadowDrawFunc, f32 a3);
// Allocates and pushes a new matrix to the matrix stack and returns a pointer to the newly created matrix.
extern Mtx* AllocateModelMatrix(Graphics* gfx, s8* filename, u32 lineno);
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

/** Actor Lifecycle Routines **/

// Parses the compact initialization script and initializes the provided actor instance according to the instructions in the script.
extern void InitializeCompactFormat(BasicActor* instance, CompactInit* configuration);
// Assigns the actor's loaded resource to the SEG_OBJECT entry in the segment table.
extern void SetupActorObjectSeg(Game* context, BasicActor* instance);
