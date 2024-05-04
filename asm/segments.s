.include "macros.inc"

.section .data

glabel _freddySegmentRomStart
.incbin "assets/freddy.ci8.bin"
.balign 16
glabel _freddySegmentRomEnd
