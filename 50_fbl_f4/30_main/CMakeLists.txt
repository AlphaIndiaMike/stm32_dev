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
    Src/adc.c                           
    Src/gpio.c                          
    Src/stm32f4xx_hal_msp.c             
    Src/stm32f4xx_it.c                  
    Src/sysmem.c
    #Src/freertos.c                      
    Src/main.c                          
    Src/stm32f4xx_hal_timebase_tim.c    
    #Src/syscalls.c                      
    Src/system_stm32f4xx.c
)

set_source_files_properties( ${${component_src}}
    COMPILE_FLAGS "${SYSTEM_COMPILE_OPTIONS}")

add_library(${component_name} OBJECT ${${component_src}})

target_compile_definitions(${component_name} PRIVATE 
	CORE_CM4
	USE_HAL_DRIVER 
	STM32F411xE 
)

target_include_directories( ${component_name} PRIVATE
    ${bsp_directory}/cmsis/arm/Core/Include
    ${bsp_directory}/cmsis/cmsis-device-f4-master/Include
    ${bsp_directory}/hal/stm32f4xx-hal-driver/Inc/Config
    ${bsp_directory}/hal/stm32f4xx-hal-driver/Inc
    Inc
)

#[[-----------------------------------
END
-------------------------------------]]

