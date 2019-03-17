#pragma once

#include "types.h"

extern void* debugText;

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
// Sets the RGBA value for the specified text structure.
extern void SetTextRGBA(void* text, u8 r, u8 g, u8 b, u8 a);
// Sets the position for the specified text structure.
extern void SetTextPos(void* text, u16 x, u16 y);
// Sets the position for the specified text structure.
extern void SetTextString(void* text, char* format, ...);
