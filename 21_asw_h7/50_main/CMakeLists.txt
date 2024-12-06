#[[-----------------------------------
Component name
-------------------------------------]]

set(component_name main_component)
set(component_src main_component_source)

message("Compiling ${component_name}, bsp_directory: ${bsp_directory}")

#[[-----------------------------------
Build setup
-------------------------------------]]

if (${CONTROLLER_ID} STREQUAL "stm32h755xx_flash_CM4")

#[[-----------------------------------
CM4 CORE
-------------------------------------]]

set(${component_src}                      
    01_core_m4/Src/main.c
    01_core_m4/Src/stm32h7xx_hal_msp.c
    01_core_m4/Src/stm32h7xx_it.c
    # 01_core_m4/Src/syscalls.c
    01_core_m4/Src/sysmem.c
    10_common/Src/system_stm32h7xx_dualcore_boot_cm4_cm7.c
    ${bsp_directory}/nucleo_h7/stm32h7xx_nucleo.c
)

set_source_files_properties( ${${component_src}}
    COMPILE_FLAGS "${SYSTEM_COMPILE_OPTIONS}")

add_library(${component_name} OBJECT ${${component_src}})

target_compile_definitions(${component_name} PRIVATE 
	CORE_CM4
	USE_HAL_DRIVER 
	STM32H755xx 
	USE_NUCLEO_64
    $<$<CONFIG:Debug>:DEBUG>
)

target_include_directories( ${component_name} PRIVATE
    ${bsp_directory}/cmsis/arm/Core/Include
    ${bsp_directory}/cmsis/cmsis-device-h7-master/Include
    ${bsp_directory}/hal/stm32h7xx-hal-driver/Inc/Config
    ${bsp_directory}/hal/stm32h7xx-hal-driver/Inc
    ${bsp_directory}/nucleo_h7
    01_core_m4/Inc
)

elseif (${CONTROLLER_ID} STREQUAL "stm32h755xx_flash_CM7")

#[[-----------------------------------
CM7 CORE
-------------------------------------]]

set(${component_src}                       
    02_core_m7/Src/main.c
    02_core_m7/Src/stm32h7xx_hal_msp.c
    02_core_m7/Src/stm32h7xx_it.c
    # 02_core_m7/Src/syscalls.c
    02_core_m7/Src/sysmem.c
    10_common/Src/system_stm32h7xx_dualcore_boot_cm4_cm7.c
    ${bsp_directory}/nucleo_h7/stm32h7xx_nucleo.c
)

set_source_files_properties( ${${component_src}}
    COMPILE_FLAGS "${SYSTEM_COMPILE_OPTIONS}")

add_library(${component_name} OBJECT ${${component_src}})

target_compile_definitions(${component_name} PRIVATE 
	CORE_CM7
	USE_HAL_DRIVER 
	STM32H755xx 
	USE_NUCLEO_64
    $<$<CONFIG:Debug>:DEBUG>
)

target_include_directories( ${component_name} PRIVATE
    ${bsp_directory}/cmsis/arm/Core/Include
    ${bsp_directory}/cmsis/cmsis-device-h7-master/Include
    ${bsp_directory}/hal/stm32h7xx-hal-driver/Inc/Config
    ${bsp_directory}/hal/stm32h7xx-hal-driver/Inc
    ${bsp_directory}/nucleo_h7
    02_core_m7/Inc
)

endif()

#[[-----------------------------------
END
-------------------------------------]]
