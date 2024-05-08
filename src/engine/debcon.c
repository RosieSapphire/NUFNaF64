#include <stdarg.h>
#include <nusys.h>

#include "engine/config.h"
#include "engine/debcon.h"

static int debcon_var_head;
static debcon_var_t debcon_vars[DEBCONVARS_MAX];

void debcon_var_push_bitmask(const char *name, u32 bitmask, u8 type, void *ptr)
{
	debcon_var_t *dcv = debcon_vars + debcon_var_head++;

	dcv->name = name;
	dcv->bitmask = bitmask;
	dcv->type = type;
	dcv->ptr = ptr;
}

void debcon_var_push(const char *name, u8 type, void *ptr)
{
	debcon_var_push_bitmask(name, 0xFFFFFFFF, type, ptr);
}

void debcon_var_pop(void)
{
	debcon_var_t *dcv = debcon_vars + --debcon_var_head;

	dcv->name = NULL;
	dcv->type = DCV_TYPE_NONE;
	dcv->ptr = NULL;
}

void debcon_var_pop_all(void)
{
	int debcon_var_head_last = debcon_var_head;

	for (int i = 0; i < debcon_var_head_last; i++)
		debcon_var_pop();
}

void debcon_print_all(void)
{
	char conbuf[128];

	for (int i = 0; i < debcon_var_head; i++)
	{
		debcon_var_t *dcv = debcon_vars + i;

		nuDebConTextPos(0, 2, 28 - i);
		switch (dcv->type)
		{
		case DCV_TYPE_U8:
			sprintf(conbuf, "%s: %u\n",
				dcv->name, *(u8 *)dcv->ptr & dcv->bitmask);
			break;

		case DCV_TYPE_S8:
			sprintf(conbuf, "%s: %d\n",
				dcv->name, *(s8 *)dcv->ptr & dcv->bitmask);
			break;

		case DCV_TYPE_U16:
			sprintf(conbuf, "%s: %u\n",
				dcv->name, *(u16 *)dcv->ptr & dcv->bitmask);
			break;

		case DCV_TYPE_S16:
			sprintf(conbuf, "%s: %d\n",
				dcv->name, *(s16 *)dcv->ptr & dcv->bitmask);
			break;

		case DCV_TYPE_F32:
			{
				u32 masked = *(u32 *)dcv->ptr & dcv->bitmask;

				sprintf(conbuf, "%s: %.5f\n", dcv->name,
					*(float *)&masked);
			}
			break;

		case DCV_TYPE_TIMER:
			sprintf(conbuf, "%s: %.5f\n", dcv->name,
				CYCLES_TO_SEC((*(int *)dcv->ptr &
						dcv->bitmask)));
			break;

		default:
			osSyncPrintf("Unknown Debcon Type '%d'\n", dcv->type);
			break;
		}
		nuDebConCPuts(0, conbuf);
	}
}
