################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include UART/subdir.mk
-include Timers/subdir.mk
-include Teclado/subdir.mk
-include SCTIMER/subdir.mk
-include LCD/subdir.mk
-include IO/subdir.mk
-include I2C/subdir.mk
-include GPIO/subdir.mk
-include Drivers/subdir.mk
-include CTIMER/subdir.mk
ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := 24LC256 EEPROM Example
BUILD_ARTIFACT_EXTENSION := axf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all:
	+@$(MAKE) --no-print-directory main-build && $(MAKE) --no-print-directory post-build

# Main-build Target
main-build: 24LC256\ EEPROM\ Example.axf

# Tool invocations
24LC256\ EEPROM\ Example.axf: $(OBJS) $(USER_OBJS) makefile $(OPTIONAL_TOOL_DEPS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -Xlinker -Map="24LC256 EEPROM Example.map" -Xlinker --cref -Xlinker --gc-sections -Xlinker -print-memory-usage -mcpu=cortex-m0 -mthumb -T 24LC256_EEPROM_Example_Debug.ld -o "24LC256 EEPROM Example.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) 24LC256/\ EEPROM/\ Example.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "24LC256 EEPROM Example.axf"; # arm-none-eabi-objcopy -v -O binary "24LC256 EEPROM Example.axf" "24LC256 EEPROM Example.bin" ; # checksum -p LPC845 -d "24LC256 EEPROM Example.bin";
	-@echo ' '

.PHONY: all clean dependents main-build post-build

-include ../makefile.targets
