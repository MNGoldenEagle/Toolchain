#pragma once

#include "types.h"
#include "contexts.h"

// The segment type that can be used within the game.
typedef enum SegmentType {
	SEG_DIRECT        = 0,  // Direct address; raw RAM location
	SEG_LOGO          = 1,  // Resources dedicated to the boot screen (spinning N64 logo)
	SEG_SCENE         = 2,  // The currently loaded scene file
	SEG_ROOM          = 3,  // The currently loaded map file
	SEG_GLOBAL_KEEP   = 4,  // The common resources file (always loaded)
	SEG_SELECTED_KEEP = 5,  // The mode-specific resources file (dungeon or field, depending on scene)
	SEG_OBJECT        = 6,  // The resource file associated to the currently running actor
	SEG_PLAYER        = 7,  // The resource file for the player actor (usually Link)
	SEG_CONTEXT_1     = 8,  // A segment that can be used to allow for programmatic modification of display lists (usually used by scene initializers)
	SEG_CONTEXT_2     = 9,  // A segment that can be used to allow for programmatic modification of display lists
	SEG_CONTEXT_3     = 10, // A segment that can be used to allow for programmatic modification of display lists
	SEG_CONTEXT_4     = 11, // A segment that can be used to allow for programmatic modification of display lists
	SEG_CONTEXT_5     = 12, // A segment that can be used to allow for programmatic modification of display lists
	SEG_CONTEXT_6     = 13, // A segment that can be used to allow for programmatic modification of display lists
	SEG_Z_BUFFER      = 14, // The current Z buffer
	SEG_FRAME_BUFFER  = 15  // The current frame buffer
} SegmentType;

// A segment-based address.  Segment addresses are defined as an offset from a base address assigned to the segment entry stored in
// the high byte.  For example, if the segment address raw value is 0x04012345, and the SEG_GLOBAL_KEEP entry in the segment table is
// assigned to the address 0x80110000, then the RCP will interpret the segment address as pointing to the RAM address 0x80122345.
typedef union SegmentAddr {
	struct {
		SegmentType segment :  8; // The segment base for the address
		u32 offset          : 24; // The offset from the segment base
	};
	u32 address; // The raw segment address
} SegmentAddr;

typedef struct FileDesc {
	u32 romStart;
	void* dest;
	int size;
} file_d;

// The segment table used by the RCP.  There are sixteen entries in the table each corresponding to the SegmentType enum.  Note that
// the first entry in the table is always NULL.
extern u32* segmentTable;

// The size of the segmentTable array.
#define SEGMENT_TABLE_SZ 16

/* Converts the provided segment address into the equivalent RAM address.
   The segment table that this method relies on will not react to changes made via calls using spSegment() or in RCP code.
   The values in this table will generally be accurate only during draw() stages. */
inline void* FromSegmentToVirtual(SegmentAddr address) {
	u8 segment = address.segment;
	u32 baseAddr = segmentTable[segment];
	u32 offset = address.offset;
	return (void*)(u32)(0x80000000 + baseAddr + offset);
}
