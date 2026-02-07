cmake_minimum_required(VERSION 3.17 FATAL_ERROR)

# -----------------------------
# Add compile options
# -----------------------------

# -----------------------------
# Define the project root as the parent directory of recipes
# -----------------------------
set(PROJECT_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../../..")
# Resolve the absolute path
get_filename_component(PROJECT_ROOT_ABS "${PROJECT_ROOT}" ABSOLUTE)
message("*******************************************")
message("Building project: ${PROJECT_ROOT_ABS}")
message("*******************************************")

#[[-----------------------------------
Project Settings
-------------------------------------]]
set(CMAKE_PROJECT_NAME          "STM32 FBL")
set(PROJECT_NAME_EXE            "FBL_STM32")
set(CMAKE_PROJECT_DESCRIPTION   "FBL project")
set(CMAKE_AUTHOR                "@alphaindiamike")
set(VERSION_MAJOR               "0")
set(VERSION_MINOR               "1")
set(VERSION_PATCH               "0")
set(CMAKE_INSTALL_SUFIX         "${CMAKE_PROJECT_NAME}")
set(CMAKE_INCLUDE_INSTALL_PATH  "${CMAKE_INSTALL_INCLUDEDIR}/${CMAKE_INSTALL_SUFIX}")
string(MAKE_C_IDENTIFIER PROJECT_ID "${CMAKE_PROJECT_NAME}")
string(TOUPPER ${CMAKE_PROJECT_NAME} ${CMAKE_PROJECT_DESCRIPTION})

#[[-----------------------------------
System
-------------------------------------]]
set(CMAKE_TOOLCHAIN_FILE "${PROJECT_ROOT}/99_build/cmake/gcc-arm-none-eabi.cmake") # No change
set(CMAKE_BINARY_DIR     "${PROJECT_ROOT}/build")
list(APPEND CMAKE_MODULE_PATH "${PROJECT_ROOT}/99_build/cmake")
include(${PROJECT_ROOT}/99_build/cmake/compile-options.cmake)

project(${CMAKE_PROJECT_NAME} C ASM)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
enable_language(ASM)

# -----------------------------
# (2) Decide which core to build
# -----------------------------
set(PROJECT_NAME_EXE "FBL_STM32_F413")
set(SYSTEM_LINKER_FILE "STM32F413_fbl.ld")

#[[-----------------------------------
Target settings
-------------------------------------]]
set(target   "${PROJECT_NAME_EXE}.out")
set(map_file "${PROJECT_NAME_EXE}.map")
set(hex_file "${PROJECT_NAME_EXE}.hex")
set(bin_file "${PROJECT_NAME_EXE}.bin")

#[[-----------------------------------
Cmake Settings
-------------------------------------]]
set(CMAKE_SKIP_BUILD_RPATH              FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH      FALSE)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH   FALSE)

# -----------------------------
# Setup compile / linking options
# -----------------------------

set(SYSTEM_COMPILE_OPTIONS ${DBG_COMPILE_OPTIONS_M7})

set(PROJ_TARGET_LINK_OPTIONS
    -T${PROJECT_ROOT}/90_config/10_linker/${SYSTEM_LINKER_FILE}
    -mfpu=fpv4-sp-d16
    -mcpu=cortex-m4
    -mfloat-abi=hard
    -mthumb
    -lc
    -lm
    -lnosys
    -Wl,--print-memory-usage
    -Wl,-Map=${map_file},--cref
    -Wl,--gc-sections
)

set(TARGET_COMPILE_DEFS
    CORE_CM4
    USE_HAL_DRIVER 
    STM32F4xx 
    USE_NUCLEO_64
    $<$<CONFIG:Debug>:DEBUG>
)

message("*******************************************")
message("Compiler options: ${SYSTEM_COMPILE_OPTIONS}")
message("*******************************************")
message("Target compile defs: ${TARGET_COMPILE_DEFS}")
message("*******************************************")
message("Linker options: ${PROJ_TARGET_LINK_OPTIONS}")
message("*******************************************")

#[[-----------------------------------
Add what to compile
-------------------------------------]]
add_subdirectory(${PROJECT_ROOT_ABS}/50_fbl_f4 ${PROJECT_ROOT_ABS}/build/50_fbl_f4)
