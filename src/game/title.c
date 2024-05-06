#include <nusys.h>
#include <malloc.h>

#include "engine/util.h"
#include "engine/config.h"
#include "engine/timer.h"
#include "engine/gfx.h"
#include "engine/segments.h"

#include "game/title.h"

#define FREDDYFACE_STATE_CNT 4
#define FREDDYFACE_STATE_RAND_MAX 100
#define FREDDYFACE_STATE_TIMER_RESET SEC_TO_CYCLES(0.08f)

#define FREDDYFACE_FLICKER_RAND_MAX 0xFA
#define FREDDYFACE_FLICKER_TIMER_RESET SEC_TO_CYCLES(0.30f)

/* variables */
static OSTime start;
static OSTime now;

static int freddyface_state;
static timer_t freddyface_state_timer;
static int freddyface_flicker;
static timer_t freddyface_flicker_timer;

/* sprites */
static u8 *freddyfaces[FREDDYFACE_STATE_CNT];

void title_init(void)
{
	freddyface_state = 0;
	freddyface_state_timer = timer_create(FREDDYFACE_STATE_TIMER_RESET);
	freddyface_flicker = 0;
	freddyface_flicker_timer = timer_create(FREDDYFACE_FLICKER_TIMER_RESET);

	ROM_READ(freddyfaces[0], title_freddyface_0);
	ROM_READ(freddyfaces[1], title_freddyface_1);
	ROM_READ(freddyfaces[2], title_freddyface_2);
	ROM_READ(freddyfaces[3], title_freddyface_3);
	start = osGetTime();
}

static void title_update(void)
{
	if (timer_tick(&freddyface_state_timer))
		freddyface_state = RAND(FREDDYFACE_STATE_RAND_MAX);

	if (timer_tick(&freddyface_flicker_timer))
		freddyface_flicker = RAND(FREDDYFACE_FLICKER_RAND_MAX);
}

static void title_draw(void)
{
	int freddyface_state_clamped =
		freddyface_state < FREDDYFACE_STATE_CNT ? freddyface_state : 0;

	glistp = glist[glist_task];
	gfx_rcp_init();
	gfx_rect_fill(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0x0, 0x0, 0x0);
	gfx_sprite_draw(0, 0, freddyfaces[freddyface_state_clamped]);
	gfx_render_task();

#if !DEBUG
	return;
#endif

	nuDebConClear(0);

	DEBCON_PRINTF(1, 26, "Freddy Face State: %d", freddyface_state);
	DEBCON_PRINTF(1, 27, "Freddy Face State Timer: %f",
		      CYCLES_TO_SEC(freddyface_state_timer.cur));
	DEBCON_PRINTF(1, 28, "Scene Timer: %.3f", CYCLES_TO_SEC(now - start));

	nuDebConDisp(NU_SC_SWAPBUFFER);
}

void title_callback(int pending_gfx_cnt)
{
	static OSTime cyc_last;
	static OSTime cyc_accum;

	now = osGetTime();

	OSTime cyc_frame = now - cyc_last;

	cyc_accum += cyc_frame;

	while (cyc_accum >= DELTACYCLES)
	{
		title_update();
		cyc_accum -= DELTACYCLES;
	}

	/* const f32 subtick = (f32)cyc_accum / DELTACYCLES; */

	if (pending_gfx_cnt < NUM_GFX_TASKS)
		title_draw();

	cyc_last = now;
}
