
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