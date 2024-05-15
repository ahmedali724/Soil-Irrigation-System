################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../MCAL/EXTERNAL_INTERRUPT/external_interrupt.c 

OBJS += \
./MCAL/EXTERNAL_INTERRUPT/external_interrupt.o 

C_DEPS += \
./MCAL/EXTERNAL_INTERRUPT/external_interrupt.d 


# Each subdirectory must supply rules for building sources it contributes
MCAL/EXTERNAL_INTERRUPT/%.o: ../MCAL/EXTERNAL_INTERRUPT/%.c MCAL/EXTERNAL_INTERRUPT/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


