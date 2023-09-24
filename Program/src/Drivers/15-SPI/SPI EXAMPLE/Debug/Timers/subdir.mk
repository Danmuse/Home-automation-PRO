################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Timers/timers.c 

C_DEPS += \
./Timers/timers.d 

OBJS += \
./Timers/timers.o 


# Each subdirectory must supply rules for building sources it contributes
Timers/%.o: ../Timers/%.c Timers/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__CODE_RED -DCORE_M0PLUS -D__MTB_BUFFER_SIZE=256 -D__USE_ROMDIVIDE -D__LPC84X__ -D__REDLIB__ -I"/home/ubuntu/Documents/ComputingSND/Exercises LPC845-BRK/RC522 RFID Example/SCTIMER" -I"/home/ubuntu/Documents/ComputingSND/Exercises LPC845-BRK/RC522 RFID Example/CTIMER" -I"/home/ubuntu/Documents/ComputingSND/Exercises LPC845-BRK/RC522 RFID Example/Drivers" -I"/home/ubuntu/Documents/ComputingSND/Exercises LPC845-BRK/RC522 RFID Example/GPIO" -I"/home/ubuntu/Documents/ComputingSND/Exercises LPC845-BRK/RC522 RFID Example/IO" -I"/home/ubuntu/Documents/ComputingSND/Exercises LPC845-BRK/RC522 RFID Example/LCD" -I"/home/ubuntu/Documents/ComputingSND/Exercises LPC845-BRK/RC522 RFID Example/src" -I"/home/ubuntu/Documents/ComputingSND/Exercises LPC845-BRK/RC522 RFID Example/Teclado" -I"/home/ubuntu/Documents/ComputingSND/Exercises LPC845-BRK/RC522 RFID Example/Timers" -I"/home/ubuntu/Documents/ComputingSND/Exercises LPC845-BRK/RC522 RFID Example/UART" -I"/home/ubuntu/Documents/ComputingSND/Exercises LPC845-BRK/RC522 RFID Example/RC522" -I"/home/ubuntu/Documents/ComputingSND/Exercises LPC845-BRK/RC522 RFID Example/SPI" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0 -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-Timers

clean-Timers:
	-$(RM) ./Timers/timers.d ./Timers/timers.o

.PHONY: clean-Timers

