#pragma once

#include "types.h"
#include "os.h"
#include "gbi.h"

struct GlobalContext;

typedef struct Vec3f {
	f32 x, y, z;
} Vec3f;

typedef struct Vec3s {
	s16 x, y, z;
} Vec3s;

typedef struct Rect {
	u32 top;
	u32 bottom;
	u32 left;
	u32 right;
} Rect;

typedef union RGBA32 {
	struct {
		u8 r;
		u8 g;
		u8 b;
		u8 a;
	};
	u32 hex;
} RGBA32;

typedef struct DLContext {
	s32 size;
	Gfx* disp;
	Gfx* append;
	Gfx* append_end;
} DLContext;

typedef struct Graphics {
	/* 0x000 */ void* work_polyOpaDisp_start;
	/* 0x004 */ void* work_polyXluDisp_start;
	/* 0x008 */ s8 padding0[0xC];
	/* 0x014 */ void* work_overlayDisp_start;
	/* 0x018 */ s8 padding1[0x20];
	/* 0x038 */ OSMesg threads[8];
	/* 0x058 */ s8 padding2[4];
	/* 0x05C */ OSMesgQueue mesgQueues;
	/* 0x074 */ s8 padding3[0x14];
	/* 0x088 */ s32 _unk_0x88;
	/* 0x08C */ s32 _unk_0x8C;
	/* 0x090 */ void* microcode;
	/* 0x094 */ s32 microcode_length;
	/* 0x098 */ void* microcode2;
	/* 0x09C */ s8 padding4[0x1C];
	/* 0x0B8 */ void* WORK_DISP_start;
	/* 0x0BC */ s32 WORK_DISP_size;
	/* 0x0C0 */ void* buffer_0xC00_start;
	/* 0x0C4 */ s32 buffer_0xC00_size;
	/* 0x0C8 */ s8 padding5[0xE8];
	/* 0x1B0 */ DLContext WORK_DISP;
	/* 0x1C0 */ s8 padding6[0xE8];
	/* 0x2A8 */ DLContext DECAL_DISP;
	/* 0x2B8 */ DLContext POLY_OPA_DISP;
	/* 0x2C8 */ DLContext POLY_XLU_DISP;
	/* 0x2D8 */ s32 frameCtr1;
	/* 0x2DC */ void* frameBuffer;
	/* 0x2E0 */ s8 padding7[8];
	/* 0x2E8 */ s32 frameCtr2;
	/* 0x2EC */ void(*func_ptr)();
	/* 0x2F0 */ struct GlobalContext* globals;
} Graphics;

extern void guPerspective(Mtx* m, u16* perspNormal, f32 fovy, f32 aspect, f32 near, f32 far, f32 scale);
