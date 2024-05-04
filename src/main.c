#include <nusys.h>

#include "debug/debug.h"

#include "engine/input.h"
#include "engine/heap.h"
#include "engine/config.h"

#include "game/title.h"

void mainproc(void)
{
	debug_initialize();

	input_pattern = nuContInit();
	nuGfxInit();
	nuPiInit();
	InitHeap(heap, HEAP_SIZE);

	title_init();
	nuGfxFuncSet((NUGfxFunc)title_callback);
	nuGfxDisplayOn();

	while (1)
	{}
}
