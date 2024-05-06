#ifndef _ENGINE_SEGMENTS_H_
#define _ENGINE_SEGMENTS_H_

#define DEFINE_SEGMENT(X) \
	extern u8 _ ## X ## SegmentRomStart[]; \
	extern u8 _ ## X ## SegmentRomEnd[];

DEFINE_SEGMENT(title_freddyface_0)
DEFINE_SEGMENT(title_freddyface_1)
DEFINE_SEGMENT(title_freddyface_2)
DEFINE_SEGMENT(title_freddyface_3)

#endif /* _ENGINE_SEGMENTS_H_ */
