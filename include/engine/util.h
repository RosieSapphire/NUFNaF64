#ifndef _ENGINE_UTIL_H_
#define _ENGINE_UTIL_H_

#include <ultra64.h>

#define CLAMP(X, MINI, MAXI) (MAX(MIN(X, MAXI), MINI))
#define ABS(X) (X < 0 ? X * -1 : X)

#define SEC_TO_USEC(X) ((OSTime)(X * 1000000))
#define USEC_TO_SEC(X) ((f32)X / 1000000.0f)
#define CYCLES_TO_SEC(X) ((f32)USEC_TO_SEC(OS_CYCLES_TO_USEC((OSTime)X)))
#define SEC_TO_CYCLES(X) ((OSTime)(OS_USEC_TO_CYCLES((OSTime)SEC_TO_USEC(X))))

#define ROM_START(X) ((u32)_ ## X ## SegmentRomStart)
#define ROM_END(X) ((u32)_ ## X ## SegmentRomEnd)
#define ROM_SIZE(X) (ROM_END(X) - ROM_START(X))

#define ROM_READ(DST, SRC) \
	DST = malloc(ROM_SIZE(SRC)); \
	nuPiReadRom(ROM_START(SRC), DST, ROM_SIZE(SRC));

#endif /* _ENGINE_UTIL_H_ */
