#ifndef _ENGINE_TIMER_H_
#define _ENGINE_TIMER_H_

typedef struct
{
	int cur, len;
} timer_t;

timer_t timer_create(int len);
int timer_tick(timer_t *t);

#endif /* _ENGINE_TIMER_H_ */
