################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../SPI/spi.c 

C_DEPS += \
./SPI/spi.d 

OBJS += \
./SPI/spi.o 


# Each subdirectory must supply rules for building sources it contributes
SPI/%.o: ../SPI/%.c SPI/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__CODE_RED -DCORE_M0PLUS -D__MTB_BUFFER_SIZE=256 -D__USE_ROMDIVIDE -D__LPC84X__ -D__REDLIB__ -I"/home/mariano/Documentos/Facultad/InfoII/WorkspaceLPC845/RC522/SCTIMER" -I"/home/mariano/Documentos/Facultad/InfoII/WorkspaceLPC845/RC522/CTIMER" -I"/home/mariano/Documentos/Facultad/InfoII/WorkspaceLPC845/RC522/Drivers" -I"/home/mariano/Documentos/Facultad/InfoII/WorkspaceLPC845/RC522/GPIO" -I"/home/mariano/Documentos/Facultad/InfoII/WorkspaceLPC845/RC522/IO" -I"/home/mariano/Documentos/Facultad/InfoII/WorkspaceLPC845/RC522/LCD" -I"/home/mariano/Documentos/Facultad/InfoII/WorkspaceLPC845/RC522/src" -I"/home/mariano/Documentos/Facultad/InfoII/WorkspaceLPC845/RC522/Teclado" -I"/home/mariano/Documentos/Facultad/InfoII/WorkspaceLPC845/RC522/Timers" -I"/home/mariano/Documentos/Facultad/InfoII/WorkspaceLPC845/RC522/UART" -I"/home/mariano/Documentos/Facultad/InfoII/WorkspaceLPC845/RC522/RC522" -I"/home/mariano/Documentos/Facultad/InfoII/WorkspaceLPC845/RC522/SPI" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0 -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-SPI

clean-SPI:
	-$(RM) ./SPI/spi.d ./SPI/spi.o

.PHONY: clean-SPI

