#include <nusys.h>
#include <malloc.h>

#include "engine/util.h"
#include "engine/config.h"
#include "engine/timer.h"
#include "engine/gfx.h"
#include "engine/segments.h"
#include "engine/debcon.h"
#include "engine/scene.h"
#include "engine/input.h"

#include "game/title.h"

#define FREDDYFACE_STATE_CNT 4
#define FREDDYFACE_STATE_RAND_MAX 100
#define FREDDYFACE_STATE_TIMER_RESET SEC_TO_CYCLES(0.08f)

#define FREDDYFACE_FLICKER_RAND_MAX 0xFA
#define FREDDYFACE_FLICKER_TIMER_RESET SEC_TO_CYCLES(0.30f)

/* variables */
static OSTime start;
static OSTime now;

static u8 freddyface_state;
static timer_t freddyface_state_timer;
static u8 freddyface_flicker;
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

	/* setup debug variables */
	debcon_var_push("Freddy Face Flicker",
			DCV_TYPE_U8, &freddyface_flicker);
	debcon_var_push("Freddy Face Flicker Timer", DCV_TYPE_TIMER,
			&freddyface_flicker_timer.cur);
	debcon_var_push("Freddy Face State", DCV_TYPE_U8, &freddyface_state);
	debcon_var_push("Freddy Face State Timer", DCV_TYPE_TIMER,
			&freddyface_state_timer.cur);

	start = osGetTime();
}

static void title_update(void)
{
	nuContDataGetEx(&input_data, 0);
	if (input_data.trigger & START_BUTTON)
	{
		scene_index = SCENE_GAME;
		title_terminate();
		nuGfxFuncRemove();
		return;
	}

	if (timer_tick(&freddyface_state_timer))
		freddyface_state = RAND(FREDDYFACE_STATE_RAND_MAX);

	if (timer_tick(&freddyface_flicker_timer))
		freddyface_flicker = RAND(FREDDYFACE_FLICKER_RAND_MAX);
}

static void title_draw(const OSTime cyc_frame)
{
	int freddyface_state_clamped =
		freddyface_state < FREDDYFACE_STATE_CNT ? freddyface_state : 0;

	glistp = glist[glist_task];
	gfx_rcp_init();
	gfx_rect_fill(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0x0, 0x0, 0x0);
	gfx_sprite_draw_primblend(0, 0, freddyfaces[freddyface_state_clamped],
				  0xFF, 0xFF, 0xFF, freddyface_flicker);
	gfx_render_task();

#if !DEBUG
	return;
#endif

	nuDebConClear(0);
	debcon_print_all();
	nuDebConDisp(NU_SC_SWAPBUFFER);
}

void title_callback(unsigned long pending_gfx_cnt)
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
		title_draw(cyc_frame);

	cyc_last = now;
}

void title_terminate(void)
{
	free(freddyfaces[0]);
	free(freddyfaces[1]);
	free(freddyfaces[2]);
	free(freddyfaces[3]);
	debcon_var_pop_all();
}
