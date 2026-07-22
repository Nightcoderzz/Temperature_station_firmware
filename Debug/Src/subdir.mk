################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/adc.c \
../Src/clock.c \
../Src/gpio.c \
../Src/main.c \
../Src/one_wire_DS18B20.c \
../Src/syscalls.c \
../Src/sysmem.c \
../Src/system_stm32c0xx.c \
../Src/timers.c \
../Src/uart.c 

OBJS += \
./Src/adc.o \
./Src/clock.o \
./Src/gpio.o \
./Src/main.o \
./Src/one_wire_DS18B20.o \
./Src/syscalls.o \
./Src/sysmem.o \
./Src/system_stm32c0xx.o \
./Src/timers.o \
./Src/uart.o 

C_DEPS += \
./Src/adc.d \
./Src/clock.d \
./Src/gpio.d \
./Src/main.d \
./Src/one_wire_DS18B20.d \
./Src/syscalls.d \
./Src/sysmem.d \
./Src/system_stm32c0xx.d \
./Src/timers.d \
./Src/uart.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32C031xx -c -I../Inc -I../Drivers/CMSIS/Device/ST/STM32C0xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/adc.cyclo ./Src/adc.d ./Src/adc.o ./Src/adc.su ./Src/clock.cyclo ./Src/clock.d ./Src/clock.o ./Src/clock.su ./Src/gpio.cyclo ./Src/gpio.d ./Src/gpio.o ./Src/gpio.su ./Src/main.cyclo ./Src/main.d ./Src/main.o ./Src/main.su ./Src/one_wire_DS18B20.cyclo ./Src/one_wire_DS18B20.d ./Src/one_wire_DS18B20.o ./Src/one_wire_DS18B20.su ./Src/syscalls.cyclo ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.cyclo ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su ./Src/system_stm32c0xx.cyclo ./Src/system_stm32c0xx.d ./Src/system_stm32c0xx.o ./Src/system_stm32c0xx.su ./Src/timers.cyclo ./Src/timers.d ./Src/timers.o ./Src/timers.su ./Src/uart.cyclo ./Src/uart.d ./Src/uart.o ./Src/uart.su

.PHONY: clean-Src

