.include "macros.inc"

.section .data

glabel _title_freddyface_0SegmentRomStart
.incbin "assets/title_freddyface_0.ci8.bin"
.balign 16
glabel _title_freddyface_0SegmentRomEnd

glabel _title_freddyface_1SegmentRomStart
.incbin "assets/title_freddyface_1.ci8.bin"
.balign 16
glabel _title_freddyface_1SegmentRomEnd

glabel _title_freddyface_2SegmentRomStart
.incbin "assets/title_freddyface_2.ci8.bin"
.balign 16
glabel _title_freddyface_2SegmentRomEnd

glabel _title_freddyface_3SegmentRomStart
.incbin "assets/title_freddyface_3.ci8.bin"
.balign 16
glabel _title_freddyface_3SegmentRomEnd
