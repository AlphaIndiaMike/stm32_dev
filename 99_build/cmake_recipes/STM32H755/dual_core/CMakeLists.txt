cmake_minimum_required(VERSION 3.17 FATAL_ERROR)

# -----------------------------
# (1) Add two simple options
# -----------------------------
option(BUILD_CM7 "Build for STM32H755 CM7" OFF)
option(BUILD_CM4 "Build for STM32H755 CM4" OFF)

#[[-----------------------------------
Project Settings
-------------------------------------]]
# We keep these exactly as you had:
set(CMAKE_PROJECT_NAME          "STM32 FBL")
set(PROJECT_NAME_EXE            "FBL_STM32")
set(CMAKE_PROJECT_DESCRIPTION   "FBL project")
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
# set(CONTROLLER_ID             "STM32F746NG_FLASH")

#-------------------------------------
# The original "default" lines:
#   set(CONTROLLER_ID "stm32h755xx_flash_CM7") # CM7 CORE H755
#   set(CONTROLLER_ID "stm32h755xx_flash_CM4") # CM4 CORE H755
#
# We'll let the new logic below override or define it.
#-------------------------------------

string(MAKE_C_IDENTIFIER PROJECT_ID "${CMAKE_PROJECT_NAME}")
string(TOUPPER ${CMAKE_PROJECT_NAME} ${CMAKE_PROJECT_DESCRIPTION})

#[[-----------------------------------
System
-------------------------------------]]
set(CMAKE_TOOLCHAIN_FILE "./99_build/cmake/gcc-arm-none-eabi.cmake") # No change
set(CMAKE_BINARY_DIR     "${CMAKE_CURRENT_SOURCE_DIR}/build")
set(CMAKE_PROJECT_ROOT   "${CMAKE_CURRENT_SOURCE_DIR}")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/99_build/cmake")
include(99_build/cmake/compile-options.cmake)

project(${CMAKE_PROJECT_NAME} C ASM)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
enable_language(ASM)

# -----------------------------
# (2) Decide which core to build
# -----------------------------
# If user did NOT enable any -DBUILD_CM7=ON or -DBUILD_CM4=ON, default to the old CM7 behavior.
if (NOT BUILD_CM7 AND NOT BUILD_CM4)
    set(CONTROLLER_ID "stm32h755xx_flash_CM7")
else()
    # If user tries to enable both, produce an error or pick one
    if (BUILD_CM7 AND BUILD_CM4)
        message(FATAL_ERROR "Please pick only one core for a single build, e.g. -DBUILD_CM7=ON -DBUILD_CM4=OFF.")
    elseif(BUILD_CM7)
        set(CONTROLLER_ID "stm32h755xx_flash_CM7")
    elseif(BUILD_CM4)
        set(CONTROLLER_ID "stm32h755xx_flash_CM4")
    endif()
endif()

message(STATUS "Selected CONTROLLER_ID = ${CONTROLLER_ID}")

# -----------------------------
# (3) Keep your original logic
#     unchanged below
# -----------------------------
if ((CONTROLLER_ID STREQUAL "STM32F746NG_FLASH") OR (CONTROLLER_ID STREQUAL "stm32h755xx_flash_CM7"))
    set(SYSTEM_COMPILE_OPTIONS ${DBG_COMPILE_OPTIONS_M7})

    set(LINKER_EXT_OPTIONS
        -mfpu=fpv4-sp-d16
        -mcpu=cortex-m7
        -mfloat-abi=hard
        -mthumb
        -lc
        -lm
        -lnosys
        -Wl,--print-memory-usage
    )

    if (CONTROLLER_ID STREQUAL "STM32F746NG_FLASH")
        set(TARGET_COMPILE_DEFS
            USE_HAL_DRIVER
            STM32F746xx
        )
    elseif (CONTROLLER_ID STREQUAL "stm32h755xx_flash_CM7")
        set(TARGET_COMPILE_DEFS
            CORE_CM7
            USE_HAL_DRIVER
            MANUAL_BOOT_CM4
            STM32H755xx
            USE_NUCLEO_64
            $<$<CONFIG:Debug>:DEBUG>
        )
        set(PROJECT_NAME_EXE "FBL_STM32_CM7")
    endif()

else()

    set(SYSTEM_COMPILE_OPTIONS ${DBG_COMPILE_OPTIONS_M4})

    set(LINKER_EXT_OPTIONS
        -mfpu=fpv4-sp-d16
        -mcpu=cortex-m4
        -mfloat-abi=hard
        -mthumb
        -lc
        -lm
        -lnosys
        -Wl,--print-memory-usage
    )

    if (CONTROLLER_ID STREQUAL "STM32F411CEUx_FLASH")
        set(TARGET_COMPILE_DEFS
            CORE_CM4
            USE_HAL_DRIVER
            STM32F4xx
        )
    elseif (CONTROLLER_ID STREQUAL "stm32h755xx_flash_CM4")
        set(TARGET_COMPILE_DEFS
            CORE_CM4
            USE_HAL_DRIVER
            # MANUAL_BOOT_CM4
            STM32H755xx
            USE_NUCLEO_64
            $<$<CONFIG:Debug>:DEBUG>
        )
        set(PROJECT_NAME_EXE "FBL_STM32_CM4")
    endif()
endif()

message("Compiler options: ${SYSTEM_COMPILE_OPTIONS}")
message("*******************************************")
message("Target compile defs: ${TARGET_COMPILE_DEFS}")
message("*******************************************")
message("Linker options: ${LINKER_EXT_OPTIONS}")
message("*******************************************")

set(SYSTEM_LINKER_FILE "${CONTROLLER_ID}.ld")

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

#[[-----------------------------------
Add what to compile
-------------------------------------]]
if (CONTROLLER_ID STREQUAL "STM32F411CEUx_FLASH")
    message("*** Building STM32F4 ***")
    add_subdirectory(11_asw_f4)

elseif (CONTROLLER_ID STREQUAL "stm32h755xx_flash_CM7")
    message("*** Building STM32H7 CORE M7 ***")
    if (EXISTS 21_asw_h7)
        add_subdirectory(21_asw_h7)
    else()
        add_subdirectory(20_fbl_h7)
    endif()

elseif (CONTROLLER_ID STREQUAL "stm32h755xx_flash_CM4")
    message("*** Building STM32H7 CORE M4 ***")
    if (EXISTS 21_asw_h7)
        add_subdirectory(21_asw_h7)
    else()
        add_subdirectory(20_fbl_h7)
    endif()

elseif (CONTROLLER_ID STREQUAL "STM32F746NG_FLASH")
    message("*** Building STM32F7 ***")
    add_subdirectory(31_asw_f7)

endif()
