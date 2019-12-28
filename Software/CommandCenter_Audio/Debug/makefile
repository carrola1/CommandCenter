################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Startup/subdir.mk
-include Src/subdir.mk
-include Middlewares/Third_Party/FatFs/src/subdir.mk
-include Drivers/STM32L0xx_HAL_Driver/Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
CommandCenter_Audio.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
CommandCenter_Audio.list \


# All Target
all: CommandCenter_Audio.elf secondary-outputs

# Tool invocations
CommandCenter_Audio.elf: $(OBJS) $(USER_OBJS) /home/andrew/Documents/GitHub/CommandCenter/Software/CommandCenter_Audio/STM32L052C8TX_FLASH.ld
	arm-none-eabi-gcc -o "CommandCenter_Audio.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m0plus -T"/home/andrew/Documents/GitHub/CommandCenter/Software/CommandCenter_Audio/STM32L052C8TX_FLASH.ld" --specs=nosys.specs -Wl,-Map="CommandCenter_Audio.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

CommandCenter_Audio.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "CommandCenter_Audio.list"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
