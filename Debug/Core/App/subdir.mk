################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/App/Bootloader.c \
../Core/App/FDCAN.c \
../Core/App/Flash.c 

C_DEPS += \
./Core/App/Bootloader.d \
./Core/App/FDCAN.d \
./Core/App/Flash.d 

OBJS += \
./Core/App/Bootloader.o \
./Core/App/FDCAN.o \
./Core/App/Flash.o 


# Each subdirectory must supply rules for building sources it contributes
Core/App/%.o Core/App/%.su: ../Core/App/%.c Core/App/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H723xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-App

clean-Core-2f-App:
	-$(RM) ./Core/App/Bootloader.d ./Core/App/Bootloader.o ./Core/App/Bootloader.su ./Core/App/FDCAN.d ./Core/App/FDCAN.o ./Core/App/FDCAN.su ./Core/App/Flash.d ./Core/App/Flash.o ./Core/App/Flash.su

.PHONY: clean-Core-2f-App

