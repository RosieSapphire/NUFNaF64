#ifndef _ENGINE_UTIL_H_
#define _ENGINE_UTIL_H_

#include <ultratypes.h>

#define ABS(X) (X < 0 ? X * -1 : X)
#define SEC_TO_USEC(X) (X * 1000000)
#define USEC_TO_SEC(X) ((f32)X / 1000000)

#define ROM_START(X) ((u32)_ ## X ## SegmentRomStart)
#define ROM_END(X) ((u32)_ ## X ## SegmentRomEnd)
#define ROM_SIZE(X) (ROM_END(X) - ROM_START(X))
#define ROM_READ(X) \
	X = malloc(ROM_SIZE(X)); \
	nuPiReadRom(ROM_START(X), X, ROM_SIZE(X));

#endif /* _ENGINE_UTIL_H_ */
