

# Options for specific architecture
ARCH_FLAGS=-mthumb -mcpu=cortex-m$(CORTEX_M)

# Startup code
STARTUP=$(BASE)/startup_ARM$(CORE).S


# -Os -flto -ffunction-sections -fdata-sections to compile for code size
# -fmessage-length=0 -fsigned-char -ffreestanding
# -DUSE_FULL_ASSERT -DDEBUG -DTRACE -DSTM32F10X_CL -DUSE_STDPERIPH_DRIVER -DHSE_VALUE=12000000 -UOS_USE_SEMIHOSTING -UOS_USE_TRACE_SEMIHOSTING_STDOUT

# g3 -std=gnu11 -MMD -MP -MF"src/main.d" -MT"src/main.o" -c -o "src/main.o" "../src/main.c"

# -T mem.ld -T libs.ld -T sections.ld -nostartfiles -Xlinker --gc-sections -L"../ldscripts" -Wl,-Map,"AP.map" --specs=nano.specs -o "AP.elf" ./src/main.o ./{my other object files}.o

################################################################################
#							NAMING VARIABLE									   #
################################################################################
# Project name
NAME = blinking_leds
# Selecting Core (STM32 DISCOVERY IS ARM M4)
CORTEX_M = 4
CORE = CM$(CORTEX_M)
BASE = .
# Compiler & Linker
CC = arm-none-eabi-gcc

################################################################################
#							GCC COMPILATION FLAGS							   #
################################################################################

# Optimization flag (-O0 = no compiler optimization)
OPTI = -O0
# Debug flag including symbols for gdb
DEBUG= -g
# Catching all warning and making them error
WE_FLAGS = -Werror -Wall -Wextra
# Extra flags (explained in Readme.md)
EX_FLAGS = -mlittle-endian -mthumb-interwork -mfloat-abi=hard -mfpu=fpv4-sp-d16
# Use newlib-nano. To disable it, specify USE_NANO=
USE_NANO=--specs=nano.specs
# Use semihosting or not
USE_SEMIHOST=--specs=rdimon.specs
USE_NOHOST=--specs=nosys.specs
#Startup definition
STARTUP_DEFS=-D__STARTUP_CLEAR_BSS -D__START=main


################################################################################
#							COMPILATION FLAGS CAT							   #
################################################################################
LFLAGS=$(USE_NANO) $(USE_NOHOST) $(LDSCRIPTS) $(GC) $(MAP)
CFLAGS=$(ARCH_FLAGS) $(STARTUP_DEFS) -Os -flto -ffunction-sections -fdata-sections

################################################################################
#								LINKING										   #
################################################################################
# Link for code size
GC=-Wl,--gc-sections
LDSCRIPTS=-L. -L$(BASE)/ldscripts -T nokeep.ld

################################################################################
#								MAPPING										   #
################################################################################
# Create map file
MAP=-Wl,-Map=$(NAME).map

################################################################################
#								MAKE RULES									   #
################################################################################

$(NAME)-$(CORE).axf: $(NAME).c $(STARTUP)
	$(CC) $^ $(CFLAGS) $(DEBUG) $(OPTI) $(LFLAGS) -o $@

clean: 
	rm -f $(NAME)*.axf *.map *.o
