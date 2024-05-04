#ifndef _ENGINE_SEGMENTS_H_
#define _ENGINE_SEGMENTS_H_

#define DEFINE_SEGMENT(X) \
	extern u8 _ ## X ## SegmentRomStart[]; \
	extern u8 _ ## X ## SegmentRomEnd[];

DEFINE_SEGMENT(freddy)

#endif /* _ENGINE_SEGMENTS_H_ */
