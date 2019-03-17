#pragma once

#include "types.h"
#include "graphics.h"

// The subcommand for actor commands.
typedef struct ActorSubCommand {
	s16 action;     // A value that is left up to the actor for interpretation.
	s16 startFrame; // The first frame this particular command will take place.
	s16 endFrame;   // The last frame this particular command will take place.
	s16 var0;       // A value that is left up to the actor for interpretation.
	s32 var1;       // A value that is left up to the actor for interpretation.
	Vec3f start;    // The starting vector for the command.  Note that actors may interpret the fields within this structure differently.
	Vec3f end;      // The ending vector for the command.  Note that actors may interpret the fields within this structure differently.
	Vec3f normal;   // The normal vector for the command.  Note that actors may interpret the fields within this structure differently.
} ActorSubCommand;

// A cutscene command for actors.
typedef struct ActorCommand {
	s32 actor;                     // The cutscene actor ID.  This does not correspond to the actor instances.
	s32 count;                     // The number of subcommands within the command.
	ActorSubCommand subcommands[]; // The array of subcommands within the command.
} ActorCommand;

#define MAX_CUTSCENE_COMMANDS 0x91
#define CTSN_CMD_SPECIAL_TERMINATOR 0x3E8
#define CTSN_CMD_TERMINATOR -1
