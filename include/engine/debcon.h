#ifndef _ENGINE_DEBCON_H_
#define _ENGINE_DEBCON_H_

enum
{
	DCV_TYPE_NONE = -1,
	DCV_TYPE_U8,
	DCV_TYPE_S8,
	DCV_TYPE_U16,
	DCV_TYPE_S16,
	DCV_TYPE_F32,
	DCV_TYPE_TIMER,
};

typedef struct
{
	const char *name;
	u32 bitmask;
	u8 type;
	void *ptr;
} debcon_var_t;

void debcon_var_push_bitmask(const char *name, u32 bitmask, u8 type, void *ptr);
void debcon_var_push(const char *name, u8 type, void *ptr);
void debcon_var_pop(void);
void debcon_var_pop_all(void);
void debcon_print_all(void);

#endif /* _ENGINE_DEBCON_H_ */
