#ifndef _ENGINE_DEBCON_H_
#define _ENGINE_DEBCON_H_

#define DCV_TYPE_NONE -1
#define DCV_TYPE_U8    0
#define DCV_TYPE_FLOAT 1
#define DCV_TYPE_TIMER 2

typedef struct
{
	const char *name;
	u8 type;
	void *ptr;
} debcon_var_t;

void debcon_var_push(const char *name, u8 type, void *ptr);
void debcon_var_pop(void);
void debcon_var_pop_all(void);
void debcon_print_all(void);

#endif /* _ENGINE_DEBCON_H_ */
