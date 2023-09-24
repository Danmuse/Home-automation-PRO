################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../SG90/sg90.c 

C_DEPS += \
./SG90/sg90.d 

OBJS += \
./SG90/sg90.o 


# Each subdirectory must supply rules for building sources it contributes
SG90/%.o: ../SG90/%.c SG90/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__CODE_RED -DCORE_M0PLUS -D__MTB_BUFFER_SIZE=256 -D__USE_ROMDIVIDE -D__LPC84X__ -D__REDLIB__ -I"/home/mariano/Documentos/Facultad/InfoII/WorkspaceLPC845/HMC5883L/SCTIMER" -I"/home/mariano/Documentos/Facultad/InfoII/WorkspaceLPC845/HMC5883L/SG90" -I"/home/mariano/Documentos/Facultad/InfoII/WorkspaceLPC845/HMC5883L/CTIMER" -I"/home/mariano/Documentos/Facultad/InfoII/WorkspaceLPC845/HMC5883L/Drivers" -I"/home/mariano/Documentos/Facultad/InfoII/WorkspaceLPC845/HMC5883L/GPIO" -I"/home/mariano/Documentos/Facultad/InfoII/WorkspaceLPC845/HMC5883L/IO" -I"/home/mariano/Documentos/Facultad/InfoII/WorkspaceLPC845/HMC5883L/LCD" -I"/home/mariano/Documentos/Facultad/InfoII/WorkspaceLPC845/HMC5883L/src" -I"/home/mariano/Documentos/Facultad/InfoII/WorkspaceLPC845/HMC5883L/Teclado" -I"/home/mariano/Documentos/Facultad/InfoII/WorkspaceLPC845/HMC5883L/Timers" -I"/home/mariano/Documentos/Facultad/InfoII/WorkspaceLPC845/HMC5883L/UART" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0 -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-SG90

clean-SG90:
	-$(RM) ./SG90/sg90.d ./SG90/sg90.o

.PHONY: clean-SG90

