#[[-----------------------------------
Component name
-------------------------------------]]

set(component_name lib_component)
set(component_src lib_component_source)

message("Compiling ${component_name}, bsp_directory: ${bsp_directory}")

#[[-----------------------------------
Build setup
-------------------------------------]]

set(${component_src}                      
    STemWin/OS/GUI_X_OS.c
) 

set_source_files_properties( ${${component_src}}
    COMPILE_FLAGS "${SYSTEM_COMPILE_OPTIONS}")

add_library(${component_name} OBJECT ${${component_src}})

target_compile_definitions(${component_name} PRIVATE 
	${TARGET_COMPILE_DEFS}
)

target_include_directories( ${component_name} PRIVATE
    STemWin/inc
    STemWin/Config
    ${os_directory}/FreeRTOS/Source/include
    ${os_directory}/FreeRTOS/Source/portable/GCC/ARM_CM7
    ${os_directory}/FreeRTOS/Config
    ${os_directory}/CMSIS_RTOS
)

#[[-----------------------------------
END
-------------------------------------]]

