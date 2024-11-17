
#[[-----------------------------------
Set Project Directories
-------------------------------------]]
set(bsp_directory_f4xx   ${CMAKE_CURRENT_SOURCE_DIR}/hal/stm32f4xx-hal-driver)
set(cmsis_directory_f4xx ${CMAKE_CURRENT_SOURCE_DIR}/cmsis/cmsis-device-f4-master)


#[[-----------------------------------
Global Include Directories
-------------------------------------]]
include_directories(
    ${cmsis_directory_f4xx}/../arm/Core/Include
    ${cmsis_directory_f4xx}/Include
    ${bsp_directory_f4xx}/Inc/Config
    ${bsp_directory_f4xx}/Inc
    ${bsp_directory_f4xx}/Inc/Legacy
)

#[[-----------------------------------
Include CMakeFiles from various modules
-------------------------------------]]
add_subdirectory(${cmsis_directory_f4xx})
add_subdirectory(${bsp_directory_f4xx})

#[[-----------------------------------
Define bsp_component as an OBJECT library

# Define an empty OBJECT library for bsp_component
add_library(bsp_component OBJECT)

# Include the object files from both components
target_sources(bsp_component PUBLIC
    $<TARGET_OBJECTS:cmsis_f4xx_component>
    $<TARGET_OBJECTS:stm32f4xx_hal_driver_component>
)

# Include directories for bsp_component
target_include_directories(bsp_component PUBLIC
    ${cmsis_directory_f4xx}/../arm/Core/Include
    ${cmsis_directory_f4xx}/Include
    ${bsp_directory_f4xx}/Inc/Config
    ${bsp_directory_f4xx}/Inc
    ${bsp_directory_f4xx}/Inc/Legacy
)

# Set compile options if necessary
target_compile_options(bsp_component PUBLIC ${DBG_COMPILE_OPTIONS_M4})

# Ensure dependencies are built in the correct order
add_dependencies(bsp_component cmsis_f4xx_component stm32f4xx_hal_driver_component)
-------------------------------------]]