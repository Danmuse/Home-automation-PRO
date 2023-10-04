################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../UART/uart0.c \
../UART/uart1.c 

C_DEPS += \
./UART/uart0.d \
./UART/uart1.d 

OBJS += \
./UART/uart0.o \
./UART/uart1.o 


# Each subdirectory must supply rules for building sources it contributes
UART/%.o: ../UART/%.c UART/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__CODE_RED -DCORE_M0PLUS -D__MTB_BUFFER_SIZE=256 -D__USE_ROMDIVIDE -D__LPC84X__ -D__REDLIB__ -I"/home/mariano/Documentos/Facultad/InfoII/WorkspaceLPC845/RC522/SCTIMER" -I"/home/mariano/Documentos/Facultad/InfoII/WorkspaceLPC845/RC522/CTIMER" -I"/home/mariano/Documentos/Facultad/InfoII/WorkspaceLPC845/RC522/Drivers" -I"/home/mariano/Documentos/Facultad/InfoII/WorkspaceLPC845/RC522/GPIO" -I"/home/mariano/Documentos/Facultad/InfoII/WorkspaceLPC845/RC522/IO" -I"/home/mariano/Documentos/Facultad/InfoII/WorkspaceLPC845/RC522/LCD" -I"/home/mariano/Documentos/Facultad/InfoII/WorkspaceLPC845/RC522/src" -I"/home/mariano/Documentos/Facultad/InfoII/WorkspaceLPC845/RC522/Teclado" -I"/home/mariano/Documentos/Facultad/InfoII/WorkspaceLPC845/RC522/Timers" -I"/home/mariano/Documentos/Facultad/InfoII/WorkspaceLPC845/RC522/UART" -I"/home/mariano/Documentos/Facultad/InfoII/WorkspaceLPC845/RC522/RC522" -I"/home/mariano/Documentos/Facultad/InfoII/WorkspaceLPC845/RC522/SPI" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0 -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-UART

clean-UART:
	-$(RM) ./UART/uart0.d ./UART/uart0.o ./UART/uart1.d ./UART/uart1.o

.PHONY: clean-UART

