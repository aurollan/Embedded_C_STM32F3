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

FILE_ASM = startup_stm32f303xc.S

FILE_SRC = main.c \
		   stm32f30x_it.c \
		   system_stm32f30x.c

FILE_OBJ = $(FILE_SRC:.c=.o)
FILE_OBJ += $(FILE_ASM:.S=.o)
OBJ = $(addprefix $(DIR_OBJ)/, $(FILE_OBJ))

ASM = $(addprefix $(DIR_ASM)/, $(FILE_ASM))
SRC = $(addprefix $(DIR_SRC)/, $(FILE_SRC))

################################################################################
#							GCC COMPILATION FLAGS							   #
################################################################################

# Optimization flag (-O0 = no compiler optimization)
OPTI = -O0
# Debug flag including symbols for gdb
DEBUG = -g
# Catching all warning and making them error
WE_FLAGS = -Werror -Wall -Wextra
# Extra flags (explained in Readme.md)
EX_FLAGS = -mlittle-endian -mthumb-interwork -mfloat-abi=hard -mfpu=fpv4-sp-d16
# TODO : FIND EXPLAINATION
EX_FLAGS2 =  -Os -flto -ffunction-sections -fdata-sections
#Startup definition
# TODO : FIND EXPLAINATION
STARTUP_DEFS =-D__STARTUP_CLEAR_BSS -D__START=main
# Options for specific architecture
ARCH_FLAGS =-mthumb -mcpu=cortex-m$(CORTEX_M)

# Include path
INCLUDE = $(addprefix -I , $(DIR_INC))

CFLAGS = $(ARCH_FLAGS) $(STARTUP_DEFS) $(EX_FLAGS) $(EX_FLAGS2) $(OPTI) $(DEBUG)

################################################################################
#								LINKING										   #
################################################################################

# Link for code size
# TODO : FIND EXPLAINATION
GC=-Wl,--gc-sections
# Use newlib-nano. To disable it, specify USE_NANO=
# TODO : FIND EXPLAINATION
# USE_NANO=--specs=nano.specs
USE_NANO=
# Use semihosting or not
USE_SEMIHOST =--specs=rdimon.specs
USE_NOHOST =--specs=nosys.specs
LDSCRIPTS= -L./ldscripts -T ld_script/STM32F303VC_FLASH.ld
# Create map file
# TODO : FIND EXPLAINATION
MAP=-Wl,-Map=$(NAME).map

LFLAGS=$(USE_NANO) $(USE_NOHOST) $(LDSCRIPTS) $(GC) $(MAP)


################################################################################
#								MAKE RULES									   #
################################################################################

all:debug $(NAME)

debug:
	@echo $(OBJ)
	@echo $(SRC)
	@echo $(ASM)
	@echo $(INCLUDE)
	@echo "---------------------------------------------------------------------"
$(NAME): $(OBJ)
	$(CC) $(OBJ) $(CFLAGS) $(LFLAGS) -o $@
	# @echo "\033[33;32m=== Compilation $(NAME)...\t\t\tDONE"
	@echo "---------------------------------------------------------------------"

$(DIR_OBJ)/%.o: $(DIR_SRC)/%.c
	mkdir -p obj
	$(CC) $(CFLAGS) $(LFLAGS) -o $@ -c $< $(INCLUDE)
	#@echo "\033[0;32m [OK] \033[0m       \033[0;33m Compiling:\033[0m" $<
	@echo "---------------------------------------------------------------------"

clean: 
	rm -rf *.map
	rm -rf *.o
	# @echo "\033[33;32m=== Cleaning project $(OBJ_DIR)...\t\tDONE"
	@echo "---------------------------------------------------------------------"

fclean: clean
	rm -rf $(NAME)	
	# @echo "\033[33;32m=== Cleaning executable $(NAME)...\t\t\tDONE"
	@echo "---------------------------------------------------------------------"

re: fclean all

################################################################################
#							FIND HOW THIS WORKS								   #
################################################################################

# -Os -flto -ffunction-sections -fdata-sections to compile for code size
# -fmessage-length=0 -fsigned-char -ffreestanding
# -DUSE_FULL_ASSERT -DDEBUG -DTRACE -DSTM32F10X_CL -DUSE_STDPERIPH_DRIVER -DHSE_VALUE=12000000 -UOS_USE_SEMIHOSTING -UOS_USE_TRACE_SEMIHOSTING_STDOUT

# g3 -std=gnu11 -MMD -MP -MF"src/main.d" -MT"src/main.o" -c -o "src/main.o" "../src/main.c"

# -T mem.ld -T libs.ld -T sections.ld -nostartfiles -Xlinker --gc-sections -L"../ldscripts" -Wl,-Map,"AP.map" --specs=nano.specs -o "AP.elf" ./src/main.o ./{my other object files}.o
