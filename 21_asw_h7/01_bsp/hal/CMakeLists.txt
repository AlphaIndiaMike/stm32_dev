#[[-----------------------------------
Component name
-------------------------------------]]

set(component_name stm32h7xx_hal_driver_component)
set(component_src bsp_component_source)

message("Compiling ${component_name}, bsp_directory: ${bsp_directory}")

#[[-----------------------------------
Build setup
-------------------------------------]]

#[[-----------------------------------
Components
-------------------------------------]]
set(${component_src}                   
stm32h7xx-hal-driver/Src/stm32h7xx_hal_cortex.c
stm32h7xx-hal-driver/Src/stm32h7xx_hal_rcc.c
stm32h7xx-hal-driver/Src/stm32h7xx_hal_rcc_ex.c
stm32h7xx-hal-driver/Src/stm32h7xx_hal_flash.c
stm32h7xx-hal-driver/Src/stm32h7xx_hal_flash_ex.c
stm32h7xx-hal-driver/Src/stm32h7xx_hal_gpio.c
stm32h7xx-hal-driver/Src/stm32h7xx_hal_hsem.c
stm32h7xx-hal-driver/Src/stm32h7xx_hal_dma.c
stm32h7xx-hal-driver/Src/stm32h7xx_hal_dma_ex.c
stm32h7xx-hal-driver/Src/stm32h7xx_hal_mdma.c
stm32h7xx-hal-driver/Src/stm32h7xx_hal_pwr.c
stm32h7xx-hal-driver/Src/stm32h7xx_hal_pwr_ex.c
stm32h7xx-hal-driver/Src/stm32h7xx_hal.c
stm32h7xx-hal-driver/Src/stm32h7xx_hal_i2c.c
stm32h7xx-hal-driver/Src/stm32h7xx_hal_i2c_ex.c
stm32h7xx-hal-driver/Src/stm32h7xx_hal_exti.c
stm32h7xx-hal-driver/Src/stm32h7xx_hal_tim.c
stm32h7xx-hal-driver/Src/stm32h7xx_hal_tim_ex.c
stm32h7xx-hal-driver/Src/stm32h7xx_hal_usart.c
stm32h7xx-hal-driver/Src/stm32h7xx_hal_usart_ex.c
stm32h7xx-hal-driver/Src/stm32h7xx_hal_uart_ex.c
stm32h7xx-hal-driver/Src/stm32h7xx_hal_uart.c
)

#[[-----------------------------------
Compiler flag
-------------------------------------]]

if (${CONTROLLER_ID} STREQUAL "stm32h755xx_flash_CM7")

#[[-----------------------------------
CM7 CORE
-------------------------------------]]

set_source_files_properties( ${${component_src}}
    COMPILE_FLAGS "${SYSTEM_COMPILE_OPTIONS}")

#[[-----------------------------------
ELSE
-------------------------------------]]

elseif (${CONTROLLER_ID} STREQUAL "stm32h755xx_flash_CM4")

#[[-----------------------------------
CM4 CORE
-------------------------------------]]

set_source_files_properties( ${${component_src}}
    COMPILE_FLAGS "${SYSTEM_COMPILE_OPTIONS}")

endif()

#[[-----------------------------------
Add library
-------------------------------------]]

add_library(${component_name} OBJECT ${${component_src}})

if (${CONTROLLER_ID} STREQUAL "stm32h755xx_flash_CM7")
target_compile_definitions(${component_name} PRIVATE 
	CORE_CM7
	USE_HAL_DRIVER 
	STM32H755xx 
	USE_NUCLEO_64
    $<$<CONFIG:Debug>:DEBUG>
)
elseif (${CONTROLLER_ID} STREQUAL "stm32h755xx_flash_CM4")
target_compile_definitions(${component_name} PRIVATE 
	CORE_CM4
	USE_HAL_DRIVER 
	STM32H755xx 
	USE_NUCLEO_64
    $<$<CONFIG:Debug>:DEBUG>
)
endif()


target_include_directories(${component_name} PRIVATE
    stm32h7xx-hal-driver/Inc
    stm32h7xx-hal-driver/Inc/Config
    stm32h7xx-hal-driver/Inc/Legacy
)

#[[-----------------------------------
END
-------------------------------------]]

