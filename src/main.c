#include <nusys.h>

#include "debug/debug.h"

#include "engine/input.h"
#include "engine/heap.h"
#include "engine/config.h"
#include "engine/scene.h"

#include "game/title.h"
#include "game/game.h"

void mainproc(void)
{
	debug_initialize();

	input_pattern = nuContInit();
	nuGfxInit();
	nuPiInit();
	InitHeap(heap, HEAP_SIZE);

	while (1)
	{
		switch (scene_index)
		{
		case SCENE_TITLE:
			scene_index = SCENE_NONE;
			title_init();
			nuGfxFuncSet(title_callback);
			nuGfxDisplayOn();
			break;

		case SCENE_GAME:
			scene_index = SCENE_NONE;
			game_init();
			nuGfxFuncSet(game_callback);
			nuGfxDisplayOn();
			break;

		default:
			break;
		}

		while (scene_index == SCENE_NONE);
		nuGfxDisplayOff();
	}
}
