
#[[-----------------------------------
Set Project Directories
-------------------------------------]]
set(bsp_directory_f4xx   ${CMAKE_CURRENT_SOURCE_DIR}/hal)
set(cmsis_directory_f4xx ${CMAKE_CURRENT_SOURCE_DIR}/cmsis)


#[[-----------------------------------
Global Include Directories
-------------------------------------]]
include_directories(
    ${cmsis_directory_f4xx}/arm/Core/Include
    ${cmsis_directory_f4xx}/cmsis-device-f4-master/Include
    ${bsp_directory_f4xx}/Inc/Config
    ${bsp_directory_f4xx}/Inc
    ${bsp_directory_f4xx}/Inc/Legacy
)

#[[-----------------------------------
Include CMakeFiles from various modules
-------------------------------------]]
add_subdirectory(${cmsis_directory_f4xx})
add_subdirectory(${bsp_directory_f4xx})