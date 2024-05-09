#ifndef _ENGINE_GFX_H_
#define _ENGINE_GFX_H_

#include "engine/config.h"

extern Gfx glist[NUM_GFX_TASKS][MAX_GFX_GLIST_SIZE];
extern Gfx *glistp;
extern u8 glist_task;

/* main */
void gfx_rcp_init(void);
void gfx_rect_fill(const s16 x, const s16 y, const u16 w, const u16 h,
		   const u8 r, const u8 g, const u8 b);
void gfx_sprite_draw_primblend(const s16 x, const s16 y, const u8 *spr,
			       const u8 r, const u8 g, const u8 b, const u8 a);
void gfx_sprite_draw(const s16 x, const s16 y, const u8 *spr);
void gfx_render_task(void);

/* internal */
void gfx_sprite_render_normal(const s16 x, const s16 y, const u16 width,
			      const u16 height, const u8 *pxls,
			      const u8 *indis, const u8 type, const u8 r,
			      const u8 g, const u8 b, const u8 a);
void gfx_sprite_render_multi(const s16 x, const s16 y, const u16 width,
			     const u16 height, const u8 *pxls,
			     const u8 *indis, const u8 type, const u8 r,
			     const u8 g, const u8 b, const u8 a);
void gfx_sprite_render_setup(const u16 type, const u8 alpha, const u16 *tlut);

#endif /* _ENGINE_GFX_H_ */
