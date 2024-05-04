#include <nusys.h>
#include <malloc.h>

#include "engine/util.h"
#include "engine/config.h"
#include "engine/gfx.h"
#include "engine/random.h"
#include "engine/segments.h"

#include "game/title.h"

#define FREDDYFACE_STATE_CNT 1
#define FREDDYFACE_STATE_TIMER_RESET 33

/* variables */
static OSTime start;
static OSTime now;

static int freddyface_state;
static int freddyface_state_timer;

/* sprites */
static u8 *title_freddyface_0;
	  /*
	  *title_freddyface_1,
	  *title_freddyface_2,
	  *title_freddyface_3;
	  */

void title_init(void)
{
	freddyface_state = 0;
	freddyface_state_timer = FREDDYFACE_STATE_TIMER_RESET;

	ROM_READ(title_freddyface_0);
	/*
	ROM_READ(title_freddyface_1);
	ROM_READ(title_freddyface_2);
	ROM_READ(title_freddyface_3);
	*/
	start = osGetTime();
}

static void title_update(void)
{
	if (freddyface_state_timer <= 0)
	{
		freddyface_state = random() % FREDDYFACE_STATE_CNT;
		freddyface_state_timer = FREDDYFACE_STATE_TIMER_RESET;
		osSyncPrintf("%d\n", freddyface_state);
	}
	freddyface_state_timer--;
}

static void title_draw(void)
{
	glistp = glist[glist_task];
	gfx_rcp_init();
	gfx_sprite_draw(0, 0, title_freddyface_0);
	gfx_render_task();

#if !DEBUG
	return;
#endif

	char conbuf[32];

	nuDebConTextPos(0, 1, 28);
	sprintf(conbuf, "Scene Timer: %.3f",
		USEC_TO_SEC(OS_CYCLES_TO_USEC(now - start)));
	nuDebConCPuts(0, conbuf);

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
