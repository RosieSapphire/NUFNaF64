#include <nusys.h>
#include <malloc.h>

#include "engine/input.h"
#include "engine/segments.h"
#include "engine/util.h"
#include "engine/gfx.h"

#define QUAD_TURN_SPEED 60

static OSTime time_start;

static u8 *freddy;

void stage00_init(void)
{
	ROM_READ(freddy);
	time_start = osGetTime();
}

static void stage00_update(void)
{
	nuContDataGetEx(&input_data, 0);
}

static void stage00_draw(const f32 subtick,
			 const OSTime last, const OSTime now)
{
	char conbuf[20];

	glistp = glist[glist_task];
	gfx_rcp_init();
	gfx_rect_fill(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0x1A, 0x33, 0x4D);
	gfx_sprite_draw(0, 0, freddy);
	gfx_render_task();

#if !DEBUG
	return;
#endif

	/* debugging */
	framerate_print(last, now);
	timer_print(time_start, now);
	nuDebConDisp(NU_SC_SWAPBUFFER);
}

void stage00_callback(int pending_gfx)
{
	static OSTime cyc_last;
	static OSTime cyc_accum;
	OSTime cyc_now = osGetTime();
	OSTime cyc_frame = cyc_now - cyc_last;

	cyc_accum += cyc_frame;

	while (cyc_accum >= DELTACYCLES)
	{
		stage00_update();
		cyc_accum -= DELTACYCLES;
	}

	const f32 subtick = (f32)cyc_accum / DELTACYCLES;

	if (pending_gfx < NUM_GFX_TASKS)
		stage00_draw(subtick, cyc_last, cyc_now);
	cyc_last = cyc_now;
}
