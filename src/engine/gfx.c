#include <assert.h>
#include <nusys.h>

#include "png2bin/textype.h"

#include "engine/util.h"
#include "engine/config.h"
#include "engine/gfx.h"

Gfx glist[NUM_GFX_TASKS][MAX_GFX_GLIST_SIZE];
Gfx *glistp = glist[0];
u8 glist_task = 0;

static Vp vp = {{
	{SCREEN_WIDTH << 1, SCREEN_HEIGHT << 1, G_MAXZ >> 1, 0},
	{SCREEN_WIDTH << 1, SCREEN_HEIGHT << 1, G_MAXZ >> 1, 0},
}};

static Gfx gfx_rcp_init_dl[] = {
	/* RSP */
	gsSPViewport(&vp),
	gsSPClearGeometryMode(0xFFFFFFFF),
	gsSPTexture(0, 0, 0, 0x0, G_OFF),

	/* RDP */
	gsDPPipelineMode(G_PM_1PRIMITIVE),
	gsDPSetScissor(G_SC_NON_INTERLACE, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT),
	gsDPSetCombineKey(G_CK_NONE),
	gsDPSetAlphaCompare(G_AC_NONE),
	gsDPSetColorDither(G_CD_BAYER),
	gsDPSetTextureFilter(G_TF_POINT),
	gsDPSetTextureConvert(G_TC_FILT),
	gsDPSetTexturePersp(G_TP_NONE),
	gsDPPipeSync(),
	gsSPEndDisplayList(),
};

void gfx_rcp_init(void)
{
	gSPSegment(glistp++, 0, 0x0);
	gSPDisplayList(glistp++, OS_K0_TO_PHYSICAL(gfx_rcp_init_dl));
}

void gfx_rect_fill(const s16 x, const s16 y, const u16 w, const u16 h,
		   const u8 r, const u8 g, const u8 b)
{
	gDPPipeSync(glistp++);
	gDPSetCycleType(glistp++, G_CYC_FILL);
	gDPSetColorImage(glistp++, G_IM_FMT_RGBA, G_IM_SIZ_16b,
			 SCREEN_WIDTH, nuGfxCfb_ptr);
	gDPSetFillColor(glistp++, (GPACK_RGBA5551(r, g, b, 0x1) << 16 |
			GPACK_RGBA5551(r, g, b, 0x1)));
	gDPFillRectangle(glistp++, x, y, x + w - 1, y + h - 1);
}

void gfx_sprite_draw_primblend(const s16 x, const s16 y, const u8 *spr,
			       const u8 r, const u8 g, const u8 b, const u8 a)
{
	/* all */
	u16 type      = *((u16 *)spr);
	u16 width     = *((u16 *)spr + 1);
	u16 height    = *((u16 *)spr + 2);

	/* rgba16 */
	u8 *pxls      = (u8 *)(spr + (2 * sizeof(u16)));

	/* ci8 */
	u16 tlut_size = *((u16 *)spr + 3);
	u16 *tlut     = (u16 *)(spr + 8);
	u8 *indis     = (u8 *)(tlut + tlut_size);

	const int w   = width << TEX_MAG_LOG;
	const int h   = height << TEX_MAG_LOG;

	gfx_sprite_render_setup(type, a, tlut);
	if (width * height >= 0x739)
		gfx_sprite_render_multi(x, y, width, height, pxls,
					indis, type, r, g, b, a);
	else
		gfx_sprite_render_normal(x, y, width, height, pxls,
					 indis, type, r, g, b, a);

	gDPPipeSync(glistp++);
	gDPSetTextureLUT(glistp++, G_TT_NONE);
}

void gfx_sprite_draw(const s16 x, const s16 y, const u8 *spr)
{
	gfx_sprite_draw_primblend(x, y, spr, 0x00, 0x00, 0x00, 0x00);
}

void gfx_render_task(void)
{
	Gfx *glist_cur = glist[glist_task];
	const s16 glist_len = glistp - glist_cur;

	gDPFullSync(glistp++);
	gSPEndDisplayList(glistp++);
	assert(glist_len < MAX_GFX_GLIST_SIZE);
	nuGfxTaskStart(glist_cur, glist_len * sizeof(*glistp),
		       NU_GFX_UCODE_F3DEX2,
#if DEBUG
		       NU_SC_NOSWAPBUFFER);
#else
		       NU_SC_SWAPBUFFER);
#endif
	glist_task = (glist_task + 1) % NUM_GFX_TASKS;
}
