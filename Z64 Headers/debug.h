#pragma once

#include "types.h"
#include "graphics.h"

// Debug Output Functions

// Sends an assertion failure message to the debugger console.
extern void AssertMessage(char* condition, char* filename, int lineNbr);
// Sends a debug message (supporting printf formatting) to the debugger console.
extern void DebugMessage(char* format, ...);
// Sends a NPE message to the debugger console if the ptr == NULL.
extern void NullPointerCheck(char* variableName, void* ptr, char* filename, int lineNbr);
// Sends the current thread information to the debugger console.
extern void PrintThreadInfo(char* filename, int lineNbr);

// Text Functions

typedef struct DebugText DebugText;

struct DebugText {
	/* 0x00 */ void (*drawFunc)(DebugText*, s8*, s32);
	/* 0x04 */ Gfx* dlist;
	/* 0x08 */ s16 x;
	/* 0x0A */ s16 y;
	/* 0x0C */ u16 offsetX;
	/* 0x0E */ u8 offsetY;
	/* 0x0F */ u8 flags;
	/* 0x10 */ RGBA32 color;
};

extern void DrawDebugText(DebugText* data, s8* string, s32 length);
// Initializes the text structure.
extern void InitDebugText(DebugText* data); // 0x800FBB8C
// Sets the RGBA value for the specified text structure.
extern void SetTextRGBA(DebugText* text, u8 r, u8 g, u8 b, u8 a);
// Sets the position for the specified text structure.
extern void SetTextPos(DebugText* text, u16 x, u16 y);
// Sets the position for the specified text structure.
extern void SetTextString(DebugText* text, s8* format, ...);
// 
extern void OpenDebugText(DebugText* text, Gfx** dlist);
extern void CloseDebugText(DebugText* data);
