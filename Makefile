##########
# Config #
##########
DEBUG   := 1
GAME    := nufnaf
VERBOSE := 0

####################
# Asset Conversion #
####################
PNG2BIN   := ./tools/png2bin/png2bin
PNG_FILES := $(wildcard assets/raw/*.png)
BIN_FILES := $(patsubst assets/raw/%.png,assets/%.bin,$(PNG_FILES))

##############
# Input Data #
##############
SRC_DIRS  := asm src src/engine src/game
ifeq ($(DEBUG),1)
	SRC_DIRS += src/debug
endif
C_FILES   := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))
S_FILES   := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.s))

###############
# Output Data #
###############
ROM       := build/$(GAME).z64
ELF       := $(patsubst %.z64,%.elf,$(ROM))
LD_SCRIPT := $(GAME).ld
BOOT	  := /usr/lib/n64/PR/bootcode/boot.6102
BOOT_OBJ  := build/boot.6102.o
O_FILES := /usr/lib/n64/nusys/nusys_isv.o \
	   $(foreach file,$(C_FILES),build/$(file:.c=.o)) \
	   $(foreach file,$(S_FILES),build/$(file:.s=.o)) \
	   $(BOOT_OBJ)
DEP_FILES := $(O_FILES:.o=.d) $(ASM_O_FILES:.o=.d)  build/$(LD_SCRIPT).d

##############################################################################
# I need this because adding this directory to the path didn't FUCKING WORK! #
##############################################################################
CRASH95BINDIR := /opt/crashsdk/bin

V :=
ifeq ($(VERBOSE),0)
	V := @
endif
NO_COL  := \033[0m
RED     := \033[0;31m
GREEN   := \033[0;32m
BLUE    := \033[0;34m
YELLOW  := \033[0;33m
BLINK   := \033[33;5m
define print
@printf "$(GREEN)$(1) $(YELLOW)$(2)$(GREEN) -> $(BLUE)$(3)$(NO_COL)\n"
endef

#############
# Libraries #
#############
LIBS := -L/usr/lib/n64/nusys -lnusys_d \
	-L/usr/lib/n64 -lultra_d -lnustd_d
ifeq ($(DEBUG),0)
	LIBS := $(patsubst -l%_d,-l%,$(LIBS))
endif
LIBS += -L$(N64_LIBGCCDIR) -lgcc

###################################
# Compiler/Assembler/Linker Flags #
###################################
ifeq ($(DEBUG),1)
	OPT    := -O0 -g3
else
	OPT    := -Os
endif
CSTD           := -Wall -Wextra -Werror -pedantic -std=c99
INCLUDE_DIRS   := /usr/include/n64 /usr/include/n64/PR \
		  /usr/include/n64/nusys /usr/include/n64/nustd \
		  tools include
DEFINES        := DEBUG=$(DEBUG) F3DEX_GBI_2=$(DEBUG) NU_DEBUG=$(DEBUG)
DEF_INC_CFLAGS := $(foreach i,$(INCLUDE_DIRS),-I$(i)) \
		  $(foreach d,$(DEFINES),-D$(d))
CFLAGS         := -G 0 $(OPT) -mabi=32 -ffreestanding -mfix4300 \
		  $(DEF_INC_CFLAGS)
ASFLAGS        := -march=vr4300 -mabi=32 $(foreach i,$(INCLUDE_DIRS),-I$(i)) \
		  $(foreach d,$(DEFINES),--defsym $(d))
CPPFLAGS       := -P -Wno-trigraphs $(DEF_INC_CFLAGS)

#############
# Toolchain #
#############
AS        := $(CRASH95BINDIR)/mips-n64-as
CC        := $(CRASH95BINDIR)/mips-n64-gcc
CPP       := cpp
LD        := $(CRASH95BINDIR)/mips-n64-ld
AR        := $(CRASH95BINDIR)/mips-n64-ar
OBJDUMP   := $(CRASH95BINDIR)/mips-n64-objdump
OBJCOPY   := $(CRASH95BINDIR)/mips-n64-objcopy

default: $(PNG2BIN) $(BIN_FILES) $(ROM)

clean:
	rm -rf build/ assets/*.bin

####################################################
# Ensure build directory exists before compilation #
####################################################
DUMMY != mkdir -p build/ $(addprefix build/,$(SRC_DIRS))

##########################
# Convert Texture Assets #
##########################
assets/%.bin: assets/raw/%.png $(PNG2BIN)
	$(call print,Converting:,$<,$@)
	$(V)$(PNG2BIN) $< $@

$(PNG2BIN):
	make -C tools/png2bin/

##################
# Compile C Code #
##################
build/%.o: %.c
	$(call print,Compiling:,$<,$@)
	$(V)$(CC) -c $(CFLAGS) -MMD -MF build/$*.d  -o $@ $<

###############
# Compile ASM #
###############
build/%.o: %.s
	$(call print,Assembling:,$<,$@)
	$(V)$(CC) -c $(CFLAGS) $(foreach i,$(INCLUDE_DIRS),-Wa,-I$(i)) \
		-x assembler-with-cpp -MMD -MF build/$*.d  -o $@ $<

###########################
# Preprocess Linkerscript #
###########################
build/$(LD_SCRIPT): $(LD_SCRIPT)
	$(call print,Preprocessing linker script:,$<,$@)
	$(V)$(CPP) $(CPPFLAGS) -DBUILD_DIR=build \
		-MMD -MP -MT $@ -MF $@.d -o $@ $<

####################
# Copy Boot Object #
####################
$(BOOT_OBJ): $(BOOT)
	$(V)$(OBJCOPY) -I binary -B mips -O elf32-bigmips $< $@

#####################
# Generate ELF file #
#####################
$(ELF): $(O_FILES) build/$(LD_SCRIPT)
	@printf "$(GREEN)Linking ELF file:  $(BLUE)$@ $(NO_COL)\n"
	$(V)$(LD) -L build -T build/$(LD_SCRIPT) \
		-Map build/$(GAME).map --no-check-sections \
		-o $@ $(O_FILES) $(LIBS)

######################
# Building Final ROM #
######################
$(ROM): $(ELF)
	$(call print,Building ROM:,$<,$@)
	$(V)$(OBJCOPY) --pad-to=0x100000 --gap-fill=0xFF $< $@ -O binary
	$(V)makemask $@

MAKEFLAGS += --no-builtin-rules

-include $(DEP_FILES)

print-% : ; $(info $* is a $(flavor $*) variable set to [$($*)]) @true

###############################################
# Betty Linter (code style checker basically) #
###############################################
BETTY_SCAN:=$(wildcard src/*.c src/engine/*.c src/game/*.c \
	    include/*.h include/engine/*.h include/game/*.h)
BETTY_FLAGS:=-strict -subjective --allow-global-variables
BETTY_IGNORE:=--ignore=CAMELCASE

betty:
	betty-style $(BETTY_SCAN) $(BETTY_FLAGS) $(BETTY_IGNORE)
