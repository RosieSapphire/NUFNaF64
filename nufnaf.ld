OUTPUT_ARCH (mips)

#include <nusys.h>

#if DEBUG
#define LIBULTRA libultra_d
#define LIBNUSYS libnusys_d
#define LIBNUSTD libnustd_d
#else
#define LIBULTRA libultra
#define LIBNUSYS libnusys
#define LIBNUSTD libnustd
#endif

/* Starts a segment */
#define BEGIN_SEG(name, addr) \
	_##name##SegmentStart = ADDR(.name); \
	_##name##SegmentRomStart = __romPos; \
	.name addr : AT(__romPos)

/* Ends a segment */
#define END_SEG(name) \
	_##name##SegmentEnd = ALIGN(ADDR(.name) + SIZEOF(.name),0x10); \
	_##name##SegmentRomEnd = _##name##SegmentEnd - \
		_##name##SegmentStart + _##name##SegmentRomStart; \
	__romPos = _##name##SegmentRomEnd; \
	. = _##name##SegmentEnd;

/* Starts a noload segment which is intended to contain variables which do not have data yet. */
#define BEGIN_NOLOAD(name) \
	_##name##SegmentBssStart = ADDR(.name.noload); \
	.name.noload (NOLOAD) :

/* Ends a noload segment */
#define END_NOLOAD(name) \
	_##name##SegmentBssEnd = ALIGN(ADDR(.name.noload) + \
		SIZEOF(.name.noload),0x10); \
	_##name##SegmentBssSize = _##name##SegmentBssEnd - \
		_##name##SegmentBssStart; \
	. = _##name##SegmentBssEnd;

SECTIONS
{
	__romPos = 0;

	BEGIN_SEG(boot, 0x04000000)
	{
		build/asm/rom_header.o(.text); /* ROM Header */
		build/boot.6102.o(.data); /* CIC 6102 bootcode */
	}
	END_SEG(boot)

	BEGIN_SEG(code, NU_SPEC_BOOT_ADDR) SUBALIGN(16)
	{
		/* text */
		build/asm/entry.o(.start);
		/usr/lib/n64/nusys/nusys_isv.o(.text);
		build/src*.o(.text);
		build/src/engine*.o(.text);
		build/src/game*.o(.text);
#if DEBUG
		build/src/debug*.o(.text);
#endif

		*/LIBULTRA.a:*.o(.text);
		*/LIBNUSYS.a:*.o(.text);
		*/LIBNUSTD.a:*.o(.text);
		*/libgcc.a:*.o(.text);

		/usr/lib/n64/PR/rspboot.o(.text);
		/usr/lib/n64/PR/gspF3DEX2.fifo.o(.text);
		/usr/lib/n64/PR/gspL3DEX2.fifo.o(.text);
		/usr/lib/n64/PR/gspF3DEX2.Rej.fifo.o(.text);
		/usr/lib/n64/PR/gspF3DEX2.NoN.fifo.o(.text);
		/usr/lib/n64/PR/gspF3DLX2.Rej.fifo.o(.text);
		/usr/lib/n64/PR/gspS2DEX2.fifo.o(.text);

		/* data */
		/usr/lib/n64/nusys/nusys_isv.o(.data*);
		build/src*.o(.data*);
		build/src/engine*.o(.data*);
		build/src/game*.o(.data*);
#if DEBUG
		build/src/debug*.o(.data*);
#endif

		*/LIBULTRA.a:*.o(.data*);
		*/LIBNUSYS.a:*.o(.data*);
		*/LIBNUSTD.a:*.o(.data*);
		*/libgcc.a:*.o(.data*);

		/usr/lib/n64/PR/rspboot.o(.data*);
		/usr/lib/n64/PR/gspF3DEX2.fifo.o(.data*);
		/usr/lib/n64/PR/gspL3DEX2.fifo.o(.data*);
		/usr/lib/n64/PR/gspF3DEX2.Rej.fifo.o(.data*);
		/usr/lib/n64/PR/gspF3DEX2.NoN.fifo.o(.data*);
		/usr/lib/n64/PR/gspF3DLX2.Rej.fifo.o(.data*);
		/usr/lib/n64/PR/gspS2DEX2.fifo.o(.data*);
		
		/* rodata */
		/usr/lib/n64/nusys/nusys_isv.o(.*rodata*);
		build/src*.o(.*rodata*);
		build/src/engine*.o(.*rodata*);
		build/src/game*.o(.*rodata*);
#if DEBUG
		build/src/debug*.o(.*rodata*);
#endif
		
		*/LIBULTRA.a:*.o(.*rodata*);
		*/LIBNUSYS.a:*.o(.*rodata*);
		*/LIBNUSTD.a:*.o(.*rodata*);
		*/libgcc.a:*.o(.*rodata*);
	}
	END_SEG(code)
	BEGIN_NOLOAD(code)
	{
		/usr/lib/n64/nusys/nusys_isv.o(COMMON);
		/usr/lib/n64/nusys/nusys_isv.o(.scommon);
		/usr/lib/n64/nusys/nusys_isv.o(.*bss*);
		build/src*.o(.*bss*);
		build/src/engine*.o(.*bss*);
		build/src/game*.o(.*bss*);
#if DEBUG
		build/src/debug*.o(.*bss*);
#endif

		*/LIBULTRA.a:*.o(COMMON);
		*/LIBULTRA.a:*.o(.scommon);
		*/LIBULTRA.a:*.o(.*bss*);
		*/LIBNUSYS.a:*.o(COMMON);
		*/LIBNUSYS.a:*.o(.scommon);
		*/LIBNUSYS.a:*.o(.*bss*);
		*/LIBNUSTD.a:*.o(COMMON);
		*/LIBNUSTD.a:*.o(.scommon);
		*/LIBNUSTD.a:*.o(.*bss*);
		*/libgcc.a:*.o(COMMON);
		*/libgcc.a:*.o(.scommon);
		*/libgcc.a:*.o(.*bss*);
	}
	END_NOLOAD(code)
	BEGIN_NOLOAD(heap)
	{
		build/src/engine/heap.o(.bss*);
	}
	END_NOLOAD(heap)
	BEGIN_SEG(segments, __romPos)
	{
		build/asm/segments.o(.data*);
	}
	END_SEG(segments)
#if DEBUG == 0
	/*
   	 * Discard everything not specifically mentioned above
   	 * (ONLY IF DEBUGGING IS DISABLED!!!)
   	 */
	/DISCARD/ :
	{
		*(*);
	}
#endif
}
