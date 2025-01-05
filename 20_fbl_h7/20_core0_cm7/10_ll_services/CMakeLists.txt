#[[-----------------------------------
Component name
-------------------------------------]]

set(component_name ll_svc_component)
set(component_src ${component_name}_source)

message("Compiling ${component_name}, bsp_directory: ${bsp_directory}")

#[[-----------------------------------
Build setup
-------------------------------------]]

#[[-----------------------------------
CM7 CORE
-------------------------------------]]

set(${component_src}                       
    svo_svc/Src/svo_svc.c
)

set_source_files_properties( ${${component_src}}
    COMPILE_FLAGS "${SYSTEM_COMPILE_OPTIONS}")

add_library(${component_name} OBJECT ${${component_src}})

target_compile_definitions(${component_name} PRIVATE 
	${TARGET_COMPILE_DEFS}
)

target_include_directories( ${component_name} PRIVATE
    ${bsp_directory}/cmsis/arm/Core/Include
    ${bsp_directory}/cmsis/cmsis-device-h7-master/Include
    ${bsp_directory}/hal/stm32h7xx-hal-driver/Inc/Config
    ${bsp_directory}/hal/stm32h7xx-hal-driver/Inc
    ${bsp_directory}/nucleo_h7
    svo_svc/Inc
)

#[[-----------------------------------
END
-------------------------------------]]
