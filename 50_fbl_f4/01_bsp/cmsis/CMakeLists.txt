#[[-----------------------------------
Component name
-------------------------------------]]

set(component_name cmsis_f4xx_component)
set(component_src cmsis_component_f4xx_source)

message("Compiling ${component_name}, bsp_directory: ${bsp_directory}")

#[[-----------------------------------
Build setup
-------------------------------------]]

set(${component_src}                      
    cmsis-device-f4-master/Source/Templates/gcc/startup_stm32f411xe.s
    #Source/Templates/system_stm32f4xx.c
)

set_source_files_properties(
  cmsis-device-f4-master/Source/Templates/gcc/startup_stm32f411xe.s
  PROPERTIES LANGUAGE ASM
)

set_source_files_properties( ${${component_src}}
    COMPILE_FLAGS "${SYSTEM_COMPILE_OPTIONS}")

#[[-----------------------------------
Add library
-------------------------------------]]

add_library(${component_name} OBJECT ${${component_src}})

target_compile_definitions(${component_name} PRIVATE 
	CORE_CM4
	USE_HAL_DRIVER 
	STM32F411xE
)

target_include_directories( ${component_name} PRIVATE
    arm/Core/Include
    cmsis-device-f4-master/Include
)