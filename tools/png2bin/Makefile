CFLAGS=-Wall -Wextra -Werror -O0 -ggdb3

APP=png2bin

default:
	cc $(CFLAGS) $(APP).c -o $(APP) -lm

clean:
	rm -f $(APP)

BETTY_SCAN   := $(APP).c
BETTY_FLAGS  := -strict -subjective
BETTY_IGNORE :=

betty:
	betty-style $(BETTY_SCAN) $(BETTY_FLAGS) $(BETTY_IGNORE)
