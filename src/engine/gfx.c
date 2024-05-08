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

	/* setup */
	gDPPipeSync(glistp++);
	gDPSetCycleType(glistp++, G_CYC_1CYCLE);
	gDPSetRenderMode(glistp++, G_RM_TEX_EDGE, G_RM_TEX_EDGE);
	gDPSetTexturePersp(glistp++, G_TP_NONE);
	if (a)
	{
		gDPSetCombineMode(glistp++, G_CC_MODULATERGBA_PRIM,
				  G_CC_MODULATERGBA_PRIM);
	}
	else
	{
		gDPSetCombineMode(glistp++, G_CC_DECALRGBA, G_CC_DECALRGBA);
	}
	if (type == TEXTYPE_CI8)
	{
		gDPSetTextureLUT(glistp++, G_TT_RGBA16);
		gDPLoadTLUT_pal256(glistp++, tlut);
	}

	/* if texture is big enough, multitexture it */
	if (width * height >= 0x739)
	{
		for (int i = 0; i < height; i++)
		{
			switch (type)
			{
			case TEXTYPE_RGBA16:
				gDPLoadMultiTile(glistp++, pxls, 0,
						 G_TX_RENDERTILE,
						 G_IM_FMT_RGBA, G_IM_SIZ_16b,
						 width, height, 0, i,
						 width - 1, i, 0,
						 G_TX_NOMIRROR, G_TX_NOMIRROR,
						 G_TX_NOMASK, G_TX_NOMASK,
						 G_TX_NOLOD, G_TX_NOLOD);
				break;

			case TEXTYPE_CI8:
				gDPLoadMultiTile(glistp++, indis, 0,
						 G_TX_RENDERTILE,
						 G_IM_FMT_CI, G_IM_SIZ_8b,
						 width, height, 0, i,
						 width - 1, i, 0,
						 G_TX_NOMIRROR, G_TX_NOMIRROR,
						 G_TX_NOMASK, G_TX_NOMASK,
						 G_TX_NOLOD, G_TX_NOLOD);
				break;
			}
			if (a)
			{
				gDPSetPrimColor(glistp++, 0, 0,
						r * a, g * a, b * a, 0xFF);
			}
			gSPTextureRectangle(glistp++,
					    MAX(x, 0) << 2, (y + i) << 2,
					    (MAX(x, 0) + MIN(w, 640)) << 2,
					    (y + i + 1) << 2,
					    G_TX_RENDERTILE,
					    (0 - MIN(x, 0)) << 5, i << 5,
					    1 << 10, 1 << 10);
		}
	}
	else
	{
		switch (type)
		{
		case TEXTYPE_RGBA16:
			gDPLoadTextureBlock(glistp++, pxls,
					    G_IM_FMT_RGBA, G_IM_SIZ_16b,
					    width, height, 0,
					    G_TX_NOMIRROR, G_TX_NOMIRROR,
					    G_TX_NOMASK, G_TX_NOMASK,
					    G_TX_NOLOD, G_TX_NOLOD);
			break;

		case TEXTYPE_CI8:
			gDPLoadTextureBlock(glistp++, indis,
					    G_IM_FMT_CI, G_IM_SIZ_8b,
					    width, height, 0,
					    G_TX_NOMIRROR, G_TX_NOMIRROR,
					    G_TX_NOMASK, G_TX_NOMASK,
					    G_TX_NOLOD, G_TX_NOLOD);
			break;
		}
		gSPTextureRectangle(glistp++,
				    x << 2, y << 2,
				    (x + w) << 2, (y + h) << 2,
				    G_TX_RENDERTILE, 0, 0,
				    1 << (10 - TEX_MAG_LOG),
				    1 << (10 - TEX_MAG_LOG));
	}

	/* cleanup */
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
