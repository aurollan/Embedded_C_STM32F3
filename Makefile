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
LD = arm-none-eabi-ld

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
		   i2c.c \
		   itm.c \
		   leds.c \
		   delay.c \
		   usart.c \
		   stm32f30x_it.c \
		   stm32f30x_rcc.c \
		   stm32f30x_i2c.c \
		   stm32f30x_gpio.c \
		   system_stm32f30x.c \

FILE_OBJ = $(FILE_SRC:.c=.o)
FILE_OBJ += $(FILE_ASM:.s=.o)

OBJ = $(addprefix $(DIR_OBJ)/, $(FILE_OBJ))
ASM = $(addprefix $(DIR_ASM)/, $(FILE_ASM))
SRC = $(addprefix $(DIR_SRC)/, $(FILE_SRC))

################################################################################
#							GCC COMPILATION FLAGS							   #
################################################################################

# Architecture flags (ARM)
A_FLAGS =-mthumb -mcpu=cortex-m$(CORTEX_M) -mlittle-endian -mthumb-interwork
# Debug flags
D_FLAGS = -g3
# Floating point flags (ARM)
F_FLAGS =  -mfloat-abi=softfp -mfpu=fpv4-sp-d16
# Include path flags
I_FLAGS = $(addprefix -I , $(DIR_INC))
# DEFINE/Macro flags
# DM_FLAGS =-D__STARTUP_CLEAR_BSS
DM_FLAGS =  -DSTM32F303xC -DUSE_FULL_ASSERT
# Optimization flags
O_FLAGS = -Os -ffunction-sections -fdata-sections
# Warning flags
W_FLAGS = -Werror -Wall -Wextra
# Spec file flags (--specs=nano.specs --specs=rdimon.specs --specs=nosys.specs)
SP_FLAGS = --specs=nosys.specs

C_FLAGS = $(A_FLAGS) $(D_FLAGS) $(F_FLAGS) $(I_FLAGS) \
		  $(DM_FLAGS) $(O_FLAGS) $(W_FLAGS) $(SP_FLAGS)

################################################################################
#								LINKING										   #
################################################################################

# Optimization flags
LO_FLAGS = --gc-sections
# Create map file
CMF_FLAGS = -Map=$(NAME).map
# linking script flags
SC_FLAGS = -T ld_scripts/STM32F303VC_FLASH.ld

L_FLAGS = $(SC_FLAGS) $(LO_FLAGS) $(CMF_FLAGS)

################################################################################
#								MAKE RULES									   #
################################################################################

all: $(NAME)

debug:
	@echo $(L_FLAGS)
	@echo $(INCLUDE)
	@echo $(C_FLAGS)
	@echo $(ASM)

$(NAME): $(OBJ)
	$(LD) $^ $(L_FLAGS) -o $@
	@echo "\033[33;32m=== Compilation $(NAME)...\t\t\tDONE\033[0m"
	@echo "---------------------------------------------------------------------"

$(DIR_OBJ)/%.o: $(DIR_SRC)/%.c
	@mkdir -p obj
	$(CC) $(C_FLAGS) -o $@ -c $<
	@echo "\033[0;32m [OK] \033[0m       \033[0;33m Compiling:\033[0m" $<
	@echo "---------------------------------------------------------------------"

$(DIR_OBJ)/%.o: $(DIR_ASM)/%.s
	@mkdir -p obj
	$(CC) $(C_FLAGS) -o $@ -c $<
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