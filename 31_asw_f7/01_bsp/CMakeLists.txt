
#[[-----------------------------------
Set Project Directories
-------------------------------------]]
set(bsp_directory_f7xx   ${CMAKE_CURRENT_SOURCE_DIR}/hal)
set(cmsis_directory_f7xx ${CMAKE_CURRENT_SOURCE_DIR}/cmsis)


#[[-----------------------------------
Global Include Directories
-------------------------------------]]
include_directories(
    ${cmsis_directory_f7xx}/arm/Core/Include
    ${cmsis_directory_f7xx}/cmsis-device-f7-master/Include
    ${bsp_directory_f7xx}/Inc/Config
    ${bsp_directory_f7xx}/Inc
    ${bsp_directory_f7xx}/Inc/Legacy
)

#[[-----------------------------------
Include CMakeFiles from various modules
-------------------------------------]]
add_subdirectory(${cmsis_directory_f7xx})
add_subdirectory(${bsp_directory_f7xx})