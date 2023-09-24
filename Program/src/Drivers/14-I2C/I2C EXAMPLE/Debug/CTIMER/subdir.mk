################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../CTIMER/ctimer.c 

C_DEPS += \
./CTIMER/ctimer.d 

OBJS += \
./CTIMER/ctimer.o 


# Each subdirectory must supply rules for building sources it contributes
CTIMER/%.o: ../CTIMER/%.c CTIMER/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__CODE_RED -DCORE_M0PLUS -D__MTB_BUFFER_SIZE=256 -D__USE_ROMDIVIDE -D__LPC84X__ -D__REDLIB__ -I"/home/ubuntu/Documents/ComputingSND/Exercises LPC845-BRK/24LC256 EEPROM Example/SCTIMER" -I"/home/ubuntu/Documents/ComputingSND/Exercises LPC845-BRK/24LC256 EEPROM Example/CTIMER" -I"/home/ubuntu/Documents/ComputingSND/Exercises LPC845-BRK/24LC256 EEPROM Example/Drivers" -I"/home/ubuntu/Documents/ComputingSND/Exercises LPC845-BRK/24LC256 EEPROM Example/GPIO" -I"/home/ubuntu/Documents/ComputingSND/Exercises LPC845-BRK/24LC256 EEPROM Example/IO" -I"/home/ubuntu/Documents/ComputingSND/Exercises LPC845-BRK/24LC256 EEPROM Example/LCD" -I"/home/ubuntu/Documents/ComputingSND/Exercises LPC845-BRK/24LC256 EEPROM Example/src" -I"/home/ubuntu/Documents/ComputingSND/Exercises LPC845-BRK/24LC256 EEPROM Example/Teclado" -I"/home/ubuntu/Documents/ComputingSND/Exercises LPC845-BRK/24LC256 EEPROM Example/Timers" -I"/home/ubuntu/Documents/ComputingSND/Exercises LPC845-BRK/24LC256 EEPROM Example/UART" -I"/home/ubuntu/Documents/ComputingSND/Exercises LPC845-BRK/24LC256 EEPROM Example/I2C" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0 -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-CTIMER

clean-CTIMER:
	-$(RM) ./CTIMER/ctimer.d ./CTIMER/ctimer.o

.PHONY: clean-CTIMER

