#include <nusys.h>

#include "engine/util.h"

void timer_print(u64 start, u64 head)
{
	char conbuf[20];

	nuDebConTextPos(0, 1, 27);
	sprintf(conbuf, "TIME: %.3f",
		USEC_TO_SEC(OS_CYCLES_TO_USEC(head - start)));
	nuDebConPuts(0, conbuf);
}

void framerate_print(u64 last, u64 now)
{
	char conbuf[20];

	nuDebConTextPos(0, 1, 28);
	sprintf(conbuf, "FPS: %.3f",
		1.0f / USEC_TO_SEC(OS_CYCLES_TO_USEC(now - last)));
	nuDebConCPuts(0, conbuf);
}
