################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
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
digital_clock.lss \

FLASH_IMAGE += \
digital_clock.hex \

EEPROM_IMAGE += \
digital_clock.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: digital_clock.elf secondary-outputs

# Tool invocations
digital_clock.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,digital_clock.map -mmcu=atmega128 -o "digital_clock.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

digital_clock.lss: digital_clock.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S digital_clock.elf  >"digital_clock.lss"
	@echo 'Finished building: $@'
	@echo ' '

digital_clock.hex: digital_clock.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex digital_clock.elf  "digital_clock.hex"
	@echo 'Finished building: $@'
	@echo ' '

digital_clock.eep: digital_clock.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex digital_clock.elf  "digital_clock.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: digital_clock.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega128 digital_clock.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(ASM_DEPS)$(EEPROM_IMAGE)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) digital_clock.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
