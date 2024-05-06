#include "engine/config.h"
#include "engine/timer.h"

timer_t timer_create(int len)
{
	return ((timer_t){len, len});
}

int timer_tick(timer_t *t)
{
	int ticks = 0;

	t->cur -= DELTACYCLES;
	while (t->cur <= 0)
	{
		t->cur += t->len;
		ticks++;
	}

	return (ticks);
}
