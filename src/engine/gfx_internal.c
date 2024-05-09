#include "png2bin/textype.h"
#include "engine/gfx.h"

void gfx_sprite_render_normal(const s16 x, const s16 y, const u16 width,
			      const u16 height, const u8 *pxls,
			      const u8 *indis, const u8 type, const u8 r,
			      const u8 g, const u8 b, const u8 a)
{
	const int w = width << TEX_MAG_LOG;
	const int h = height << TEX_MAG_LOG;

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

void gfx_sprite_render_multi(const s16 x, const s16 y, const u16 width,
			     const u16 height, const u8 *pxls,
			     const u8 *indis, const u8 type, const u8 r,
			     const u8 g, const u8 b, const u8 a)
{
	const int w = width << TEX_MAG_LOG;
	const int h = height << TEX_MAG_LOG;

	for (int i = 0; i < height; i++)
	{
		switch (type)
		{
		case TEXTYPE_RGBA16:
			gDPLoadMultiTile(glistp++, pxls, 0, G_TX_RENDERTILE,
					 G_IM_FMT_RGBA, G_IM_SIZ_16b,
					 width, height, 0, i, width - 1, i, 0,
					 G_TX_NOMIRROR, G_TX_NOMIRROR,
					 G_TX_NOMASK, G_TX_NOMASK,
					 G_TX_NOLOD, G_TX_NOLOD);
			break;

		case TEXTYPE_CI8:
			gDPLoadMultiTile(glistp++, indis, 0, G_TX_RENDERTILE,
					 G_IM_FMT_CI, G_IM_SIZ_8b,
					 width, height, 0, i, width - 1, i, 0,
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
		gSPTextureRectangle(glistp++, MAX(x, 0) << 2, (y + i) << 2,
				    (MAX(x, 0) + MIN(w, 640)) << 2,
				    (y + i + 1) << 2, G_TX_RENDERTILE,
				    (0 - MIN(x, 0)) << 5, i << 5,
				    1 << 10, 1 << 10);
	}
}

void gfx_sprite_render_setup(const u16 type, const u8 alpha, const u16 *tlut)
{
	gDPPipeSync(glistp++);
	gDPSetCycleType(glistp++, G_CYC_1CYCLE);
	gDPSetRenderMode(glistp++, G_RM_TEX_EDGE, G_RM_TEX_EDGE);
	gDPSetTexturePersp(glistp++, G_TP_NONE);
	if (alpha)
	{
		gDPSetCombineMode(glistp++, G_CC_MODULATERGBA_PRIM,
				  G_CC_MODULATERGBA_PRIM);
	}
	else
	{
		gDPSetCombineMode(glistp++, G_CC_DECALRGBA, G_CC_DECALRGBA);
	}

	if (type != TEXTYPE_CI8)
		return;

	gDPSetTextureLUT(glistp++, G_TT_RGBA16);
	gDPLoadTLUT_pal256(glistp++, tlut);
}
