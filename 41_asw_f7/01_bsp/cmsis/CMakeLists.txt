#[[-----------------------------------
Component name
-------------------------------------]]

set(component_name cmsis_f7xx_component)
set(component_src cmsis_component_source)

message("Compiling ${component_name}, bsp_directory: ${bsp_directory}")

#[[-----------------------------------
Build setup
-------------------------------------]]

set (startup_script startup_stm32f746xx.s)

set(${component_src}                      
    cmsis-device-f7-master/Source/Templates/gcc/${startup_script}
)
set_source_files_properties(
  cmsis-device-f7-master/Source/Templates/gcc/${startup_script}
  PROPERTIES LANGUAGE ASM
)

set_source_files_properties( ${${component_src}}
    COMPILE_FLAGS "${SYSTEM_COMPILE_OPTIONS}")

#[[-----------------------------------
Add library
-------------------------------------]]

add_library(${component_name} OBJECT ${${component_src}})

target_compile_definitions(${component_name} PRIVATE 
	${TARGET_COMPILE_DEFS}
)

target_include_directories( ${component_name} PRIVATE
    arm/Core/Include
    cmsis-device-f7-master/Include
)