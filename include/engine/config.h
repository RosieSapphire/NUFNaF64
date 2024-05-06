#ifndef _ENGINE_CONFIG_H_
#define _ENGINE_CONFIG_H_

#include "engine/util.h"

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

#define MAX_GFX_GLIST_SIZE 4096
#define NUM_GFX_TASKS 3

#define STICK_DEADZONE 8

#define TICKRATE 60
#define DELTATIME (1.0f / TICKRATE)
#define DELTACYCLES SEC_TO_CYCLES(DELTATIME)

#define HEAP_SIZE 0x80000

#define TEX_MAG_LOG 1

#endif /* _ENGINE_CONFIG_H_ */
