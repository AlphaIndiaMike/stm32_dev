#[[-----------------------------------
Component name
-------------------------------------]]

set(component_name cmsis_h7xx_component)
set(component_src cmsis_component_source)

message("Compiling ${component_name}, bsp_directory: ${bsp_directory}")

#[[-----------------------------------
Build setup
-------------------------------------]]

if (BUILD_CM7)

#[[-----------------------------------
CM7 CORE
-------------------------------------]]

set (startup_script startup_stm32h755xx_CM7.s)

set(${component_src}                      
    cmsis-device-h7-master/Source/Templates/gcc/${startup_script}
)
set_source_files_properties(
  cmsis-device-h7-master/Source/Templates/gcc/${startup_script}
  PROPERTIES LANGUAGE ASM
)

set_source_files_properties( ${${component_src}}
    COMPILE_FLAGS "${SYSTEM_COMPILE_OPTIONS}")


#[[-----------------------------------
ELSE
-------------------------------------]]

elseif (BUILD_CM4)

#[[-----------------------------------
CM4 CORE
-------------------------------------]]

set (startup_script startup_stm32h755xx_CM4.s)

set(${component_src}                       
    cmsis-device-h7-master/Source/Templates/gcc/${startup_script}
    #Source/Templates/system_stm32f4xx.c
)
set_source_files_properties(
  cmsis-device-h7-master/Source/Templates/gcc/${startup_script}
  PROPERTIES LANGUAGE ASM
)

set_source_files_properties( ${${component_src}}
    COMPILE_FLAGS "${SYSTEM_COMPILE_OPTIONS}")

endif()

#[[-----------------------------------
Add library
-------------------------------------]]

add_library(${component_name} OBJECT ${${component_src}})

target_compile_definitions(${component_name} PRIVATE 
	${TARGET_COMPILE_DEFS}
)

target_include_directories( ${component_name} PRIVATE
    arm/Core/Include
    cmsis-device-h7-master/Include
)