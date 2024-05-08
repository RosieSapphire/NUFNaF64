#include <nusys.h>
#include <malloc.h>

#include "engine/util.h"
#include "engine/input.h"
#include "engine/config.h"
#include "engine/scene.h"
#include "engine/segments.h"
#include "engine/gfx.h"
#include "engine/debcon.h"

#include "game/game.h"

/* constants */
#define OFFICE_SCROLL_MIN -213.0f
#define OFFICE_SCROLL_SPEED 4.0f

/* variables */
static OSTime start;
static OSTime now;

static f32 office_scroll;

/* sprites */
static u8 *office_normal;

void game_init(void)
{
	office_scroll = 0.0f;

	ROM_READ(office_normal, game_office_normal);

	debcon_var_push("Office Normal TXTYPE", DCV_TYPE_U16, office_normal);
	debcon_var_push("Control Stick X", DCV_TYPE_S8, &input_data.stick_x);
	debcon_var_push("Office Scroll", DCV_TYPE_F32, &office_scroll);

	start = osGetTime();
}

static void game_update(void)
{
	nuContDataGetEx(&input_data, 0);
	if (input_data.trigger & START_BUTTON)
	{
		scene_index = SCENE_TITLE;
		game_terminate();
		nuGfxFuncRemove();
	}

	float stick_xf = (f32)input_data.stick_x / STICK_MAX;

	if (ABS(input_data.stick_x) > STICK_DEADZONE)
	{
		office_scroll -= stick_xf * OFFICE_SCROLL_SPEED;
		office_scroll = CLAMP(office_scroll, OFFICE_SCROLL_MIN, 0.0f);
	}
}

static void game_draw(void)
{
	glistp = glist[glist_task];
	gfx_rcp_init();
	gfx_rect_fill(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0xFF, 0xFF, 0xFF);
	gfx_sprite_draw(office_scroll, 0, office_normal);
	gfx_render_task();

#if !DEBUG
	return;
#endif

	nuDebConClear(0);
	debcon_print_all();
	nuDebConDisp(NU_SC_SWAPBUFFER);
}

void game_callback(long unsigned int pending_gfx_cnt)
{
	static OSTime cyc_last;
	static OSTime cyc_accum;

	now = osGetTime();

	OSTime cyc_frame = now - cyc_last;

	cyc_accum += cyc_frame;

	while (cyc_accum >= DELTACYCLES)
	{
		game_update();
		cyc_accum -= DELTACYCLES;
	}

	/* const f32 subtick = (f32)cyc_accum / DELTACYCLES; */

	if (pending_gfx_cnt < NUM_GFX_TASKS)
		game_draw();

	cyc_last = now;
}

void game_terminate(void)
{
	free(office_normal);
	debcon_var_pop_all();
}
