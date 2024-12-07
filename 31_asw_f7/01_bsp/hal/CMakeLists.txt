#[[-----------------------------------
Component name
-------------------------------------]]

set(component_name stm32f7xx_hal_driver_component)
set(component_src bsp_component_source)

message("Compiling ${component_name}, bsp_directory: ${bsp_directory}")

#[[-----------------------------------
Build setup
-------------------------------------]]

#[[-----------------------------------
Components
-------------------------------------]]
set(${component_src}                   
stm32f7xx-hal-driver-master/Src/stm32f7xx_hal_adc.c
stm32f7xx-hal-driver-master/Src/stm32f7xx_hal_adc_ex.c
stm32f7xx-hal-driver-master/Src/stm32f7xx_hal_rcc.c
stm32f7xx-hal-driver-master/Src/stm32f7xx_hal_rcc_ex.c
stm32f7xx-hal-driver-master/Src/stm32f7xx_hal_flash.c
stm32f7xx-hal-driver-master/Src/stm32f7xx_hal_flash_ex.c
stm32f7xx-hal-driver-master/Src/stm32f7xx_hal_gpio.c
stm32f7xx-hal-driver-master/Src/stm32f7xx_hal_dma.c
stm32f7xx-hal-driver-master/Src/stm32f7xx_hal_dma_ex.c
stm32f7xx-hal-driver-master/Src/stm32f7xx_hal_pwr.c
stm32f7xx-hal-driver-master/Src/stm32f7xx_hal_pwr_ex.c
stm32f7xx-hal-driver-master/Src/stm32f7xx_hal_cortex.c
stm32f7xx-hal-driver-master/Src/stm32f7xx_hal.c
stm32f7xx-hal-driver-master/Src/stm32f7xx_hal_i2c.c
stm32f7xx-hal-driver-master/Src/stm32f7xx_hal_i2c_ex.c
stm32f7xx-hal-driver-master/Src/stm32f7xx_hal_exti.c
stm32f7xx-hal-driver-master/Src/stm32f7xx_hal_crc.c
stm32f7xx-hal-driver-master/Src/stm32f7xx_hal_crc_ex.c
stm32f7xx-hal-driver-master/Src/stm32f7xx_hal_dcmi.c
stm32f7xx-hal-driver-master/Src/stm32f7xx_hal_dcmi_ex.c
stm32f7xx-hal-driver-master/Src/stm32f7xx_hal_dma2d.c
stm32f7xx-hal-driver-master/Src/stm32f7xx_hal_eth.c
stm32f7xx-hal-driver-master/Src/stm32f7xx_ll_fmc.c
stm32f7xx-hal-driver-master/Src/stm32f7xx_hal_sdram.c
stm32f7xx-hal-driver-master/Src/stm32f7xx_hal_ltdc.c
stm32f7xx-hal-driver-master/Src/stm32f7xx_hal_ltdc_ex.c
stm32f7xx-hal-driver-master/Src/stm32f7xx_hal_dsi.c
stm32f7xx-hal-driver-master/Src/stm32f7xx_hal_qspi.c
stm32f7xx-hal-driver-master/Src/stm32f7xx_hal_rtc.c
stm32f7xx-hal-driver-master/Src/stm32f7xx_hal_rtc_ex.c
stm32f7xx-hal-driver-master/Src/stm32f7xx_hal_sai.c
stm32f7xx-hal-driver-master/Src/stm32f7xx_hal_sai_ex.c
stm32f7xx-hal-driver-master/Src/stm32f7xx_ll_sdmmc.c
stm32f7xx-hal-driver-master/Src/stm32f7xx_hal_sd.c
stm32f7xx-hal-driver-master/Src/stm32f7xx_hal_spdifrx.c
stm32f7xx-hal-driver-master/Src/stm32f7xx_hal_spi.c
stm32f7xx-hal-driver-master/Src/stm32f7xx_hal_spi_ex.c
stm32f7xx-hal-driver-master/Src/stm32f7xx_hal_tim.c
stm32f7xx-hal-driver-master/Src/stm32f7xx_hal_tim_ex.c
stm32f7xx-hal-driver-master/Src/stm32f7xx_hal_uart.c
stm32f7xx-hal-driver-master/Src/stm32f7xx_hal_uart_ex.c
stm32f7xx-hal-driver-master/Src/stm32f7xx_hal_hcd.c
stm32f7xx-hal-driver-master/Src/stm32f7xx_ll_usb.c
)

#[[-----------------------------------
Compiler flag
-------------------------------------]]

set_source_files_properties( ${${component_src}}
    COMPILE_FLAGS "${SYSTEM_COMPILE_OPTIONS}")

#[[-----------------------------------
Add library
-------------------------------------]]

add_library(${component_name} OBJECT ${${component_src}})

target_compile_definitions(${component_name} PRIVATE 
	${TARGET_COMPILE_DEFS}
)

target_include_directories(${component_name} PRIVATE
    stm32f7xx-hal-driver-master/Inc
    stm32f7xx-hal-driver-master/Inc/Config
    stm32f7xx-hal-driver-master/Inc/Legacy
)

#[[-----------------------------------
END
-------------------------------------]]

