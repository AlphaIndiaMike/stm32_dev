set(main_component_source                      
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

set_source_files_properties( ${main_component_source}
    COMPILE_FLAGS "${DEFAULT_COMPILE_OPTIONS_M4}")

add_library(main_component OBJECT ${main_component_source})

target_include_directories( main_component PRIVATE
    ${bsp_directory_f4xx}/Inc
    Inc
)