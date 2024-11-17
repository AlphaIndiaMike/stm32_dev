cmake_minimum_required(VERSION 3.17 FATAL_ERROR)

#[[-----------------------------------
Project Settings
-------------------------------------]]
set(CMAKE_TOOLCHAIN_FILE "./99_build/cmake/gcc-arm-none-eabi.cmake") # Can have platform specific cmake
set(CMAKE_BINARY_DIR     "${CMAKE_CURRENT_SOURCE_DIR}/build")
set(CMAKE_PROJECT_ROOT   "${CMAKE_CURRENT_SOURCE_DIR}")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/99_build/cmake")

include(99_build/cmake/compile-options.cmake)

set(CMAKE_PROJECT_NAME          "STM32 build demo")
set(PROJECT_NAME_EXE            "DEMO_STM32")
set(CMAKE_PROJECT_DESCRIPTION   "Demo project")
set(CMAKE_AUTHOR                "@alphaindiamike")
set(VERSION_MAJOR               "0")
set(VERSION_MINOR               "1")
set(VERSION_PATCH               "0")
set(CMAKE_INSTALL_SUFIX         "${CMAKE_PROJECT_NAME}")
set(CMAKE_INCLUDE_INSTALL_PATH  "${CMAKE_INSTALL_INCLUDEDIR}/${CMAKE_INSTALL_SUFIX}")

string(MAKE_C_IDENTIFIER PROJECT_ID "${CMAKE_PROJECT_NAME}")
string(TOUPPER ${CMAKE_PROJECT_NAME} ${CMAKE_PROJECT_DESCRIPTION})

project(${CMAKE_PROJECT_NAME} C ASM)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
enable_language(ASM)
#[[-----------------------------------
System settings
-------------------------------------]]
set(SYSTEM_LINKER_FILE
#"STM32F746NGHx_FLASH.ld" #enable when using, disable others
"STM32F411CEUx_FLASH.ld")

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
add_subdirectory(00_code)