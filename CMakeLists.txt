cmake_minimum_required(VERSION 3.17 FATAL_ERROR)

#[[-----------------------------------
Project Settings
-------------------------------------]]
set(CMAKE_PROJECT_NAME          "STM32 build demo")
set(PROJECT_NAME_EXE            "DEMO_STM32")
set(CMAKE_PROJECT_DESCRIPTION   "Demo project")
set(CMAKE_AUTHOR                "@alphaindiamike")
set(VERSION_MAJOR               "0")
set(VERSION_MINOR               "1")
set(VERSION_PATCH               "0")
set(CMAKE_INSTALL_SUFIX         "${CMAKE_PROJECT_NAME}")
set(CMAKE_INCLUDE_INSTALL_PATH  "${CMAKE_INSTALL_INCLUDEDIR}/${CMAKE_INSTALL_SUFIX}")


#[[-----------------------------------
Available controllers by Linker
-------------------------------------]]
# set(CONTROLLER_ID             "STM32F411CEUx_FLASH")
#[[-----------------------------------
Dual core controllers:
compile each core at the time
-------------------------------------]]
# set(CONTROLLER_ID               "stm32h755xx_flash_CM7") # CM7 CORE H755
set(CONTROLLER_ID             "stm32h755xx_flash_CM4") # CM4 CORE H755

string(MAKE_C_IDENTIFIER PROJECT_ID "${CMAKE_PROJECT_NAME}")
string(TOUPPER ${CMAKE_PROJECT_NAME} ${CMAKE_PROJECT_DESCRIPTION})

#[[-----------------------------------
System
-------------------------------------]]
set(CMAKE_TOOLCHAIN_FILE "./99_build/cmake/gcc-arm-none-eabi.cmake") # Can have platform specific cmake
set(CMAKE_BINARY_DIR     "${CMAKE_CURRENT_SOURCE_DIR}/build")
set(CMAKE_PROJECT_ROOT   "${CMAKE_CURRENT_SOURCE_DIR}")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/99_build/cmake")

include(99_build/cmake/compile-options.cmake)

project(${CMAKE_PROJECT_NAME} C ASM)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
enable_language(ASM)

if ((${CONTROLLER_ID} STREQUAL "STM32F746NGHx_FLASH") OR (${CONTROLLER_ID} STREQUAL "stm32h755xx_flash_CM7"))
set(SYSTEM_COMPILE_OPTIONS ${DEFAULT_COMPILE_OPTIONS_M7})
else()
set(SYSTEM_COMPILE_OPTIONS ${DEFAULT_COMPILE_OPTIONS_M4})
endif()

set(SYSTEM_LINKER_FILE
"${CONTROLLER_ID}.ld"
)

#[[-----------------------------------
Target settings
-------------------------------------]]
set(target        ${PROJECT_NAME_EXE}.out)
set(map_file      ${PROJECT_NAME_EXE}.map)
set(hex_file      ${PROJECT_NAME_EXE}.hex)
set(bin_file      ${PROJECT_NAME_EXE}.bin)

#[[-----------------------------------
Cmake Settings
-------------------------------------]]

set(CMAKE_SKIP_BUILD_RPATH              FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH      FALSE)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH   FALSE)

#[[-----------------------------------
Add what to compile
-------------------------------------]]
if (${CONTROLLER_ID} STREQUAL "STM32F411CEUx_FLASH")
    message("*** Building STM32F4 ***")
    add_subdirectory(11_asw_f4)

elseif (${CONTROLLER_ID} STREQUAL "stm32h755xx_flash_CM7")
    message("*** Building STM32H7 CORE M7 ***")
    
    add_subdirectory(21_asw_h7)

elseif (${CONTROLLER_ID} STREQUAL "stm32h755xx_flash_CM4")
    message("*** Building STM32H7 CORE M4 ***")
    add_subdirectory(21_asw_h7)

endif()
