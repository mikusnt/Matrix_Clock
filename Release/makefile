################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include seq/subdir.mk
-include devices/subdir.mk
-include data_types/subdir.mk
-include I2C_TWI/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
Matrix_Clock.lss \

FLASH_IMAGE += \
Matrix_Clock.hex \

EEPROM_IMAGE += \
Matrix_Clock.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: Matrix_Clock.elf secondary-outputs

# Tool invocations
Matrix_Clock.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,Matrix_Clock.map -mmcu=atmega88p -o "Matrix_Clock.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

Matrix_Clock.lss: Matrix_Clock.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S Matrix_Clock.elf  >"Matrix_Clock.lss"
	@echo 'Finished building: $@'
	@echo ' '

Matrix_Clock.hex: Matrix_Clock.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex Matrix_Clock.elf  "Matrix_Clock.hex"
	@echo 'Finished building: $@'
	@echo ' '

Matrix_Clock.eep: Matrix_Clock.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex Matrix_Clock.elf  "Matrix_Clock.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: Matrix_Clock.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega88p Matrix_Clock.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(ASM_DEPS)$(EEPROM_IMAGE)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) Matrix_Clock.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
