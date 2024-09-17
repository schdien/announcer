################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/ILI9341.c \
../Core/Src/JQ8900.c \
../Core/Src/bus.c \
../Core/Src/exti.c \
../Core/Src/font_lib.c \
../Core/Src/main.c \
../Core/Src/stm32f4xx_it.c \
../Core/Src/sys_clk.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f4xx.c 

OBJS += \
./Core/Src/ILI9341.o \
./Core/Src/JQ8900.o \
./Core/Src/bus.o \
./Core/Src/exti.o \
./Core/Src/font_lib.o \
./Core/Src/main.o \
./Core/Src/stm32f4xx_it.o \
./Core/Src/sys_clk.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f4xx.o 

C_DEPS += \
./Core/Src/ILI9341.d \
./Core/Src/JQ8900.d \
./Core/Src/bus.d \
./Core/Src/exti.d \
./Core/Src/font_lib.d \
./Core/Src/main.d \
./Core/Src/stm32f4xx_it.d \
./Core/Src/sys_clk.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f4xx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32F411xE -DUSE_FULL_LL_DRIVER -DHSE_VALUE=25000000 -DHSE_STARTUP_TIMEOUT=100 -DLSE_STARTUP_TIMEOUT=5000 -DLSE_VALUE=32768 -DEXTERNAL_CLOCK_VALUE=12288000 -DHSI_VALUE=16000000 -DLSI_VALUE=32000 -DVDD_VALUE=3300 -DPREFETCH_ENABLE=1 -DINSTRUCTION_CACHE_ENABLE=1 -DDATA_CACHE_ENABLE=1 -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/ILI9341.d ./Core/Src/ILI9341.o ./Core/Src/ILI9341.su ./Core/Src/JQ8900.d ./Core/Src/JQ8900.o ./Core/Src/JQ8900.su ./Core/Src/bus.d ./Core/Src/bus.o ./Core/Src/bus.su ./Core/Src/exti.d ./Core/Src/exti.o ./Core/Src/exti.su ./Core/Src/font_lib.d ./Core/Src/font_lib.o ./Core/Src/font_lib.su ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/stm32f4xx_it.d ./Core/Src/stm32f4xx_it.o ./Core/Src/stm32f4xx_it.su ./Core/Src/sys_clk.d ./Core/Src/sys_clk.o ./Core/Src/sys_clk.su ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32f4xx.d ./Core/Src/system_stm32f4xx.o ./Core/Src/system_stm32f4xx.su

.PHONY: clean-Core-2f-Src

