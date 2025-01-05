#[[-----------------------------------
Component name
-------------------------------------]]

set(component_name free_rtos_10_3_component)
set(component_src free_rtos_10_3_source)

message("Compiling ${component_name}, bsp_directory: ${bsp_directory}")

#[[-----------------------------------
Build setup
-------------------------------------]]

set(${component_src}                      
FreeRTOS/Source/timers.c
FreeRTOS/Source/tasks.c
FreeRTOS/Source/queue.c
FreeRTOS/Source/list.c
FreeRTOS/Source/event_groups.c
FreeRTOS/Source/croutine.c
FreeRTOS/Source/portable/GCC/ARM_CM7/port.c
FreeRTOS/Source/portable/MemMang/heap_4.c
CMSIS_RTOS/cmsis_os.c
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
    FreeRTOS/Source/include
    FreeRTOS/Source/portable/GCC/ARM_CM7
    FreeRTOS/Config
    CMSIS_RTOS
)

#[[-----------------------------------
END
-------------------------------------]]

