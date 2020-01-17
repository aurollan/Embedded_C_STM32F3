.DEFAULT_GOAL := all
.PHONY: all clean fclean

################################################################################
#								PROJECT	CONFIG 								   #
################################################################################

# Selecting Core (STM32F303VC DISCOVERY IS ARM M4)
CORTEX_M = 4
CORE = CM$(CORTEX_M)
# Project name
NAME = blinking_leds-$(CORE)

# Compiler & Linker
CC = arm-none-eabi-gcc

# STM32 library path
DIR_STM = STM32F30x_DSP_StdPeriph_Lib_V1.2.3

DIR_INC = inc
DIR_INC += $(DIR_STM)/Libraries/CMSIS/Include
DIR_INC += $(DIR_STM)/Libraries/STM32F30x_StdPeriph_Driver/inc
DIR_INC += $(DIR_STM)/Libraries/CMSIS/Device/ST/STM32F30x/Include
		  
DIR_ASM = asm
DIR_SRC = src
DIR_OBJ = obj

################################################################################
#							PROJECT FILES									   #
################################################################################

FILE_ASM = startup_stm32f30x.s

FILE_SRC = main.c \
		   stm32f30x_it.c \
		   system_stm32f30x.c \
		   stm32f30x_gpio.c \
		   stm32f30x_rcc.c


FILE_OBJ = $(FILE_SRC:.c=.o)
FILE_OBJ += $(FILE_ASM:.s=.o)

OBJ = $(addprefix $(DIR_OBJ)/, $(FILE_OBJ))
ASM = $(addprefix $(DIR_ASM)/, $(FILE_ASM))
SRC = $(addprefix $(DIR_SRC)/, $(FILE_SRC))

################################################################################
#							GCC COMPILATION FLAGS							   #
################################################################################

# Optimization flag (-O0 = no compiler optimization)
OPTI = -O2
# Debug flag including symbols for gdb
DEBUG = -g3
# Catching all warning and making them error
W_FLAGS = -Werror -Wall -Wextra
# Extra flags (explained in Readme.md)
EX_FLAGS = -mlittle-endian -mthumb-interwork -mfloat-abi=hard \
		   -mfpu=fpv4-sp-d16 -Os -ffunction-sections -fdata-sections
#Startup definition
# STARTUP_DEFS =-D__STARTUP_CLEAR_BSS -D__START=main
STARTUP_DEFS =
# Options for specific ARM architecture
ARCH_FLAGS =-mthumb -mcpu=cortex-m$(CORTEX_M)

# Include path
INCLUDE = $(addprefix -I , $(DIR_INC))

CFLAGS = $(ARCH_FLAGS) $(STARTUP_DEFS) $(EX_FLAGS) $(OPTI) $(DEBUG) $(W_FLAGS)

################################################################################
#								LINKING										   #
################################################################################

# remove all unused code
GC = -Wl,--gc-sections
# Create map file
# TODO : FIND EXPLAINATION
MAP=-Wl,-Map=$(NAME).map

# Use newlib-nano. C library for embedded
# USE_NANO=--specs=nano.specs
USE_NANO=
# Use semihosting or not
USE_SEMIHOST =--specs=rdimon.specs
USE_NOHOST =--specs=nosys.specs
LDSCRIPTS= -L./ld_scripts -T ld_scripts/STM32F303VC_FLASH.ld

LFLAGS=$(USE_NANO) $(USE_NOHOST) $(LDSCRIPTS) $(GC) $(MAP)

################################################################################
#								MAKE RULES									   #
################################################################################

all: debug $(NAME)

debug:
	@echo $(LFLAGS)
	@echo $(INCLUDE)
	@echo $(CFLAGS)
	@echo $(ASM)

$(NAME): $(OBJ)
	@echo "---------------------------------------------------------------------"
	@echo $^
	@echo "---------------------------------------------------------------------"
	$(CC) $^ $(CFLAGS) $(LFLAGS) -o $@
	@echo "\033[33;32m=== Compilation $(NAME)...\t\t\tDONE"
	@echo "---------------------------------------------------------------------"

$(DIR_OBJ)/%.o: $(DIR_SRC)/%.c
	@mkdir -p obj
	$(CC) $(CFLAGS) $(LFLAGS) -o $@ -c $< $(INCLUDE)
	@echo "\033[0;32m [OK] \033[0m       \033[0;33m Compiling:\033[0m" $<
	@echo "---------------------------------------------------------------------"

$(DIR_OBJ)/%.o: $(DIR_ASM)/%.s
	@mkdir -p obj
	$(CC) $(CFLAGS) $(LFLAGS) -o $@ -c $< $(INCLUDE)
	@echo "\033[0;32m [OK] \033[0m       \033[0;33m Compiling:\033[0m" $<
	@echo "---------------------------------------------------------------------"

clean: 
	@rm -rf *.map
	@rm -rf obj
	@echo "\033[33;32m=== Cleaning project $(DIR_OBJ) directory and $(NAME).map...\t\tDONE\033[0m"

fclean: clean
	@rm -rf $(NAME)	
	@echo "\033[33;32m=== Cleaning executable $(NAME)...\t\t\tDONE\033[0m"

re: fclean all

################################################################################
#							FIND HOW THIS WORKS								   #
################################################################################

# -DUSE_FULL_ASSERT
#  -DDEBUG
#  -DTRACE
#  -DSTM32F10X_CL
#  -DUSE_STDPERIPH_DRIVER
#  -DHSE_VALUE=12000000
#  -UOS_USE_SEMIHOSTING
#  -UOS_USE_TRACE_SEMIHOSTING_STDOUT
