################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Teclado/teclado.c 

C_DEPS += \
./Teclado/teclado.d 

OBJS += \
./Teclado/teclado.o 


# Each subdirectory must supply rules for building sources it contributes
Teclado/%.o: ../Teclado/%.c Teclado/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__CODE_RED -DCORE_M0PLUS -D__MTB_BUFFER_SIZE=256 -D__USE_ROMDIVIDE -D__LPC84X__ -D__REDLIB__ -I"/home/mariano/Documentos/Facultad/InfoII/WorkspaceLPC845/24LC256/SCTIMER" -I"/home/mariano/Documentos/Facultad/InfoII/WorkspaceLPC845/24LC256/CTIMER" -I"/home/mariano/Documentos/Facultad/InfoII/WorkspaceLPC845/24LC256/Drivers" -I"/home/mariano/Documentos/Facultad/InfoII/WorkspaceLPC845/24LC256/GPIO" -I"/home/mariano/Documentos/Facultad/InfoII/WorkspaceLPC845/24LC256/IO" -I"/home/mariano/Documentos/Facultad/InfoII/WorkspaceLPC845/24LC256/LCD" -I"/home/mariano/Documentos/Facultad/InfoII/WorkspaceLPC845/24LC256/src" -I"/home/mariano/Documentos/Facultad/InfoII/WorkspaceLPC845/24LC256/Teclado" -I"/home/mariano/Documentos/Facultad/InfoII/WorkspaceLPC845/24LC256/Timers" -I"/home/mariano/Documentos/Facultad/InfoII/WorkspaceLPC845/24LC256/UART" -I"/home/mariano/Documentos/Facultad/InfoII/WorkspaceLPC845/24LC256/I2C" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0 -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-Teclado

clean-Teclado:
	-$(RM) ./Teclado/teclado.d ./Teclado/teclado.o

.PHONY: clean-Teclado
