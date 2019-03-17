#pragma once

#include "types.h"

// Digital controller button flags
typedef enum ContButton {
	CONT_C_RIGHT = 0x0001, // C-Right
	CONT_C_LEFT  = 0x0002, // C-Left
	CONT_C_DOWN  = 0x0004, // C-Down
	CONT_C_UP    = 0x0008, // C-Up
	CONT_R       = 0x0010, // R shoulder button
	CONT_L       = 0x0020, // L shoulder button
	CONT_D_RIGHT = 0x0100, // D-Right
	CONT_D_LEFT  = 0x0200, // D-Left
	CONT_D_DOWN  = 0x0400, // D-Down
	CONT_D_UP    = 0x0800, // D-Up
	CONT_START   = 0x1000, // Start button
	CONT_Z       = 0x2000, // Z trigger button
	CONT_B       = 0x4000, // B button
	CONT_A       = 0x8000  // A button
} ContButton;

// The current state of the controller
typedef struct OSContPad {
	/* 0x0 */ u16 button; // A set of flags for the buttons on the controller
	/* 0x2 */ s8 stick_x; // The horizontal position of the joystick
	/* 0x3 */ s8 stick_y; // The vertical position of the joystick
	/* 0x4 */ u8 errno;   // The controller's error code; typically 0
	/* 0x5 */ u8 padding;
} OSContPad;

typedef struct Controller {
	/* 0x00 */ OSContPad input;   /* doesn't appear to be read by game */
	/* 0x06 */ OSContPad input2;  /* seems to be set simultaneously as input */
	/* 0x0C */ OSContPad press;   /* only tracks button presses; stick sometimes matches input, sometimes doesn't */
	/* 0x12 */ OSContPad release; /* only tracks button releases; stick is something weird */
} Controller;
