#pragma once
#include "types.h"

/**
 * Contains types and structure defs for the N64 OS.
 */

typedef s32 OSPri;
typedef s32 OSId;
typedef u32 OSIntMask;
typedef u64 OSTime;
typedef void *OSMesg;

typedef union __OSfp {
	struct f {
		f32 f_odd;
		f32 f_even;
	} f;
	f64 d;
} __OSfp;

typedef struct OSThreadContext {
	u64 at, v0, v1, a0, a1, a2, a3;
	u64 t0, t1, t2, t3, t4, t5, t6, t7;
	u64 s0, s1, s2, s3, s4, s5, s6, s7;
	u64 t8, t9, gp, sp, s8, ra;
	u64 lo, hi;
	u32 sr, pc, cause, badvaddr, rcp;
	u32 fpcsr;
	__OSfp fp0, fp2, fp4, fp6, fp8, fp10, fp12, fp14;
	__OSfp fp16, fp18, fp20, fp22, fp24, fp26, fp28, fp30;
} OSThreadContext;

typedef struct OSThread {
	struct OSThread *next;
	OSPri priority;
	struct OSThread **queue;
	struct OSThread **tlnext;
	u16 state;
	u16 flags;
	OSId id;
	s32 fp;
	OSThreadContext registers;
} OSThread;

typedef struct OSMesgQueue {
	OSThread *rcvqueues;
	OSThread *sendqueues;
	s32 validCount;
	s32 first;
	s32 msgCount;
	OSMesg *msg;
} OSMesgQueue;

typedef struct OSTimer {
	struct OSTimer *next; /* point to next timer in list */
	struct OSTimer *prev; /* point to previous timer in list */
	OSTime interval;      /* duration set by user */
	OSTime value;         /* time remaining before */
	OSMesgQueue *mq;      /* Message Queue */
	OSMesg msg;           /* Message to send */
} OSTimer;

typedef struct {
	u32	type;
	u32	flags;

	u64	*ucode_boot;
	u32	ucode_boot_size;

	u64	*ucode;
	u32	ucode_size;

	u64	*ucode_data;
	u32	ucode_data_size;

	u64	*dram_stack;
	u32	dram_stack_size;

	u64	*output_buff;
	u64	*output_buff_size;

	u64	*data_ptr;
	u32	data_size;

	u64	*yield_data_ptr;
	u32	yield_data_size;

} OSTask_t;

typedef union {
	OSTask_t		t;
	long long int	force_structure_alignment;
} OSTask;

typedef u32 OSYieldResult;

// Thread Management Routines
extern void osSendMesg(OSMesgQueue *mq, OSMesg msg, s32 flag);
extern void osStopThread(OSThread *t);
extern void osRecvMesg(OSMesgQueue *mq, OSMesg *msg, s32 flag);
extern void osCreateThread(OSThread *t, OSId id, void* entry, void* arg, void* sp, OSPri priority);
extern OSId osGetThreadId(OSThread *t);
extern OSIntMask osSetIntMask(OSIntMask mask);
extern void osCreateMesgQueue(OSMesgQueue *mq, OSMesg *msg, s32 count);
extern void osJamMesg(OSMesgQueue *mq, OSMesg msg, s32 flag);
extern void osSetThreadPri(OSThread* t, OSPri priority);
extern OSPri osGetThreadPri(OSThread* t);
extern void osStartThread(OSThread* t);
extern s32 osStopTimer(OSTimer* timer);
extern void osYieldThread();

// CPU Management Routines
extern u32 osVirtualToPhysical(void* vaddr);
extern void osInvalICache(void* vaddr, s32 length);
extern void osInvalDCache(void* vaddr, s32 length);
extern void osWritebackDCache(void* vaddr, s32 length);
extern void osWritebackDCacheAll();
extern u32 osGetCount();
extern void __osSetCompare(u32 value);
extern u32 __osSetFpcCsr(u32 value);
extern u32 __osGetFpcCsr();
extern u32 __osGetCause();

// RCP Management Routines
extern void osSpTaskLoad(OSTask* task);
extern u32 osDpGetStatus();
extern void osDpSetStatus(u32 status);
extern u32 osAiGetLength();

// Clock Routines
extern OSTime osGetTime();
