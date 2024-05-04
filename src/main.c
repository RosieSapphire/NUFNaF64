#include <nusys.h>

#include "debug/debug.h"

#include "engine/input.h"
#include "engine/heap.h"
#include "engine/config.h"

#include "game/stage00.h"

void mainproc(void)
{
	debug_initialize();

	input_pattern = nuContInit();
	nuGfxInit();
	nuPiInit();
	InitHeap(heap, HEAP_SIZE);

	stage00_init();
	nuGfxFuncSet((NUGfxFunc)stage00_callback);
	nuGfxDisplayOn();

	while (1)
	{}
}
