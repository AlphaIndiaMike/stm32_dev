
#[[-----------------------------------
Set Project Directories
-------------------------------------]]
set(bsp_directory_h7xx   ${CMAKE_CURRENT_SOURCE_DIR}/hal)
set(cmsis_directory_h7xx ${CMAKE_CURRENT_SOURCE_DIR}/cmsis)


#[[-----------------------------------
Global Include Directories
-------------------------------------]]
include_directories(
    ${cmsis_directory_h7xx}/arm/Core/Include
    ${cmsis_directory_h7xx}/cmsis-device-h7-master/Include
    ${bsp_directory_h7xx}/Inc/Config
    ${bsp_directory_h7xx}/Inc
    ${bsp_directory_h7xx}/Inc/Legacy
)

#[[-----------------------------------
Include CMakeFiles from various modules
-------------------------------------]]
add_subdirectory(${cmsis_directory_h7xx})
add_subdirectory(${bsp_directory_h7xx})