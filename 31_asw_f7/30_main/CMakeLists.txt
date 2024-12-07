#[[-----------------------------------
Component name
-------------------------------------]]

set(component_name main_component)
set(component_src main_component_source)

message("Compiling ${component_name}, bsp_directory: ${bsp_directory}")

#[[-----------------------------------
Build setup
-------------------------------------]]

set(${component_src}                      
    Src/main.c
    Src/gpio.c
    Src/adc.c
    Src/crc.c
    Src/dcmi.c
    Src/dma2d.c
    Src/eth.c
    Src/fmc.c
    Src/i2c.c
    Src/ltdc.c
    Src/quadspi.c
    Src/rtc.c
    Src/sai.c
    Src/sdmmc.c
    Src/spdifrx.c
    Src/spi.c
    Src/tim.c
    Src/usart.c
    Src/usb_otg.c
    Src/stm32f7xx_it.c
    Src/stm32f7xx_hal_msp.c
    Src/stm32f7xx_hal_timebase_tim.c
    Src/system_stm32f7xx.c
    Src/sysmem.c
    # Src/syscalls.c
) 

set_source_files_properties( ${${component_src}}
    COMPILE_FLAGS "${SYSTEM_COMPILE_OPTIONS}")

add_library(${component_name} OBJECT ${${component_src}})

target_compile_definitions(${component_name} PRIVATE 
	${TARGET_COMPILE_DEFS}
)

target_include_directories( ${component_name} PRIVATE
    ${bsp_directory}/cmsis/arm/Core/Include
    ${bsp_directory}/cmsis/cmsis-device-f7-master/Include
    ${bsp_directory}/hal/stm32f7xx-hal-driver-master/Inc/Config
    ${bsp_directory}/hal/stm32f7xx-hal-driver-master/Inc
    Inc
)

#[[-----------------------------------
END
-------------------------------------]]

