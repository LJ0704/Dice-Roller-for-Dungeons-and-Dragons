################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/EEPROM.c.c \
../Core/Src/LCD_Fill_shapes.c \
../Core/Src/LCD_Game_Display.c \
../Core/Src/LCD_Image_Display.c \
../Core/Src/LCD_Pixel_Draw.c \
../Core/Src/LCD_ST7789_Init.c \
../Core/Src/LCD_Test_Func.c \
../Core/Src/LCD_Text_Display.c \
../Core/Src/LCD_write.c \
../Core/Src/Onboard_LED_Init.c \
../Core/Src/SPI_Init.c \
../Core/Src/button_interrupts.c \
../Core/Src/command.c \
../Core/Src/game_definitions.c \
../Core/Src/game_logic.c \
../Core/Src/i2c.c \
../Core/Src/main.c \
../Core/Src/stm32f4xx_hal_msp.c \
../Core/Src/stm32f4xx_it.c \
../Core/Src/sys_timer_delay.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f4xx.c \
../Core/Src/uart.c 

OBJS += \
./Core/Src/EEPROM.c.o \
./Core/Src/LCD_Fill_shapes.o \
./Core/Src/LCD_Game_Display.o \
./Core/Src/LCD_Image_Display.o \
./Core/Src/LCD_Pixel_Draw.o \
./Core/Src/LCD_ST7789_Init.o \
./Core/Src/LCD_Test_Func.o \
./Core/Src/LCD_Text_Display.o \
./Core/Src/LCD_write.o \
./Core/Src/Onboard_LED_Init.o \
./Core/Src/SPI_Init.o \
./Core/Src/button_interrupts.o \
./Core/Src/command.o \
./Core/Src/game_definitions.o \
./Core/Src/game_logic.o \
./Core/Src/i2c.o \
./Core/Src/main.o \
./Core/Src/stm32f4xx_hal_msp.o \
./Core/Src/stm32f4xx_it.o \
./Core/Src/sys_timer_delay.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f4xx.o \
./Core/Src/uart.o 

C_DEPS += \
./Core/Src/EEPROM.c.d \
./Core/Src/LCD_Fill_shapes.d \
./Core/Src/LCD_Game_Display.d \
./Core/Src/LCD_Image_Display.d \
./Core/Src/LCD_Pixel_Draw.d \
./Core/Src/LCD_ST7789_Init.d \
./Core/Src/LCD_Test_Func.d \
./Core/Src/LCD_Text_Display.d \
./Core/Src/LCD_write.d \
./Core/Src/Onboard_LED_Init.d \
./Core/Src/SPI_Init.d \
./Core/Src/button_interrupts.d \
./Core/Src/command.d \
./Core/Src/game_definitions.d \
./Core/Src/game_logic.d \
./Core/Src/i2c.d \
./Core/Src/main.d \
./Core/Src/stm32f4xx_hal_msp.d \
./Core/Src/stm32f4xx_it.d \
./Core/Src/sys_timer_delay.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f4xx.d \
./Core/Src/uart.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/EEPROM.c.cyclo ./Core/Src/EEPROM.c.d ./Core/Src/EEPROM.c.o ./Core/Src/EEPROM.c.su ./Core/Src/LCD_Fill_shapes.cyclo ./Core/Src/LCD_Fill_shapes.d ./Core/Src/LCD_Fill_shapes.o ./Core/Src/LCD_Fill_shapes.su ./Core/Src/LCD_Game_Display.cyclo ./Core/Src/LCD_Game_Display.d ./Core/Src/LCD_Game_Display.o ./Core/Src/LCD_Game_Display.su ./Core/Src/LCD_Image_Display.cyclo ./Core/Src/LCD_Image_Display.d ./Core/Src/LCD_Image_Display.o ./Core/Src/LCD_Image_Display.su ./Core/Src/LCD_Pixel_Draw.cyclo ./Core/Src/LCD_Pixel_Draw.d ./Core/Src/LCD_Pixel_Draw.o ./Core/Src/LCD_Pixel_Draw.su ./Core/Src/LCD_ST7789_Init.cyclo ./Core/Src/LCD_ST7789_Init.d ./Core/Src/LCD_ST7789_Init.o ./Core/Src/LCD_ST7789_Init.su ./Core/Src/LCD_Test_Func.cyclo ./Core/Src/LCD_Test_Func.d ./Core/Src/LCD_Test_Func.o ./Core/Src/LCD_Test_Func.su ./Core/Src/LCD_Text_Display.cyclo ./Core/Src/LCD_Text_Display.d ./Core/Src/LCD_Text_Display.o ./Core/Src/LCD_Text_Display.su ./Core/Src/LCD_write.cyclo ./Core/Src/LCD_write.d ./Core/Src/LCD_write.o ./Core/Src/LCD_write.su ./Core/Src/Onboard_LED_Init.cyclo ./Core/Src/Onboard_LED_Init.d ./Core/Src/Onboard_LED_Init.o ./Core/Src/Onboard_LED_Init.su ./Core/Src/SPI_Init.cyclo ./Core/Src/SPI_Init.d ./Core/Src/SPI_Init.o ./Core/Src/SPI_Init.su ./Core/Src/button_interrupts.cyclo ./Core/Src/button_interrupts.d ./Core/Src/button_interrupts.o ./Core/Src/button_interrupts.su ./Core/Src/command.cyclo ./Core/Src/command.d ./Core/Src/command.o ./Core/Src/command.su ./Core/Src/game_definitions.cyclo ./Core/Src/game_definitions.d ./Core/Src/game_definitions.o ./Core/Src/game_definitions.su ./Core/Src/game_logic.cyclo ./Core/Src/game_logic.d ./Core/Src/game_logic.o ./Core/Src/game_logic.su ./Core/Src/i2c.cyclo ./Core/Src/i2c.d ./Core/Src/i2c.o ./Core/Src/i2c.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/stm32f4xx_hal_msp.cyclo ./Core/Src/stm32f4xx_hal_msp.d ./Core/Src/stm32f4xx_hal_msp.o ./Core/Src/stm32f4xx_hal_msp.su ./Core/Src/stm32f4xx_it.cyclo ./Core/Src/stm32f4xx_it.d ./Core/Src/stm32f4xx_it.o ./Core/Src/stm32f4xx_it.su ./Core/Src/sys_timer_delay.cyclo ./Core/Src/sys_timer_delay.d ./Core/Src/sys_timer_delay.o ./Core/Src/sys_timer_delay.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32f4xx.cyclo ./Core/Src/system_stm32f4xx.d ./Core/Src/system_stm32f4xx.o ./Core/Src/system_stm32f4xx.su ./Core/Src/uart.cyclo ./Core/Src/uart.d ./Core/Src/uart.o ./Core/Src/uart.su

.PHONY: clean-Core-2f-Src

