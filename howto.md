Understood! Configuring a CMake-based project for STM32 controllers, especially targeting different variants like the dual-core STM32H7 series, requires careful setup to handle cross-compilation, specific toolchains, and hardware-specific configurations. Below is a comprehensive, step-by-step guide to help you set up such a project. This guide covers:

1. **Project Structure**
2. **Setup Script (`setup_project.sh`)**
3. **Build Script (`build.sh`)**
4. **CMake Configuration (`CMakeLists.txt` and Toolchain File)**
5. **Modular Configuration for Firmware Components**
6. **Custom Linker Scripts for Different Controllers**
7. **Sample Source Code for Dual-Core STM32H7 Series**
8. **Final Steps and Usage**

---

## 1. Project Structure

Organizing your project with a clear and scalable structure is crucial, especially when targeting multiple STM32 controllers. Here's a recommended directory layout:

```
STM32Project/
├── build.sh
├── setup_project.sh
├── CMakeLists.txt
├── toolchain-arm-none-eabi.cmake
├── linker_scripts/
│   ├── STM32H743ZITx_FLASH.ld
│   └── STM32H750XB_FLASH.ld
├── src/
│   ├── main.c
│   ├── dual_core/
│   │   ├── core1.c
│   │   └── core2.c
│   └── modules/
│       ├── ModuleA/
│       │   ├── ModuleA.c
│       │   └── ModuleA.h
│       └── ModuleB/
│           ├── ModuleB.c
│           └── ModuleB.h
├── include/
│   ├── stm32h7xx.h
│   ├── ModuleA.h
│   └── ModuleB.h
└── drivers/
    ├── CMSIS/
    └── HAL/
```

**Description:**

- **build.sh**: Script to build the project.
- **setup_project.sh**: Script to check dependencies.
- **CMakeLists.txt**: Root CMake configuration.
- **toolchain-arm-none-eabi.cmake**: CMake toolchain file for cross-compilation.
- **linker_scripts/**: Contains linker scripts for different STM32 controllers.
- **src/**: Source files, including main application, dual-core specific code, and modules.
- **include/**: Header files.
- **drivers/**: CMSIS and HAL drivers for STM32.

---

## 2. Setup Script (`setup_project.sh`)

This script ensures that all necessary tools are available on the user's system. Since it's targeted for macOS and Linux, it checks for required executables and prompts the user to install missing ones.

### Create `setup_project.sh`

Create a file named `setup_project.sh` in the root directory with the following content:

```bash
#!/bin/bash

# Exit immediately if a command exits with a non-zero status
set -e

echo "Starting STM32 project setup..."

# Determine the operating system
OS_TYPE="$(uname)"
echo "Detected OS: $OS_TYPE"

# Function to check if a command exists
command_exists() {
    command -v "$1" >/dev/null 2>&1
}

# List of required executables
REQUIRED_CMDS=("arm-none-eabi-gcc" "cmake" "make" "git")

MISSING_CMDS=()

# Check for each required command
for cmd in "${REQUIRED_CMDS[@]}"; do
    if ! command_exists "$cmd"; then
        MISSING_CMDS+=("$cmd")
    fi
done

# If any commands are missing, prompt the user to install them
if [ ${#MISSING_CMDS[@]} -ne 0 ]; then
    echo "The following required executables are missing:"
    for cmd in "${MISSING_CMDS[@]}"; do
        echo "  - $cmd"
    done
    echo ""
    echo "Please install the missing executables and ensure they are in your PATH."
    echo "Installation instructions:"
    if [ "$OS_TYPE" = "Linux" ]; then
        echo "  - Install arm-none-eabi-gcc:"
        echo "      sudo apt update && sudo apt install gcc-arm-none-eabi"
        echo "  - Install CMake and Make:"
        echo "      sudo apt install cmake make"
        echo "  - Install Git:"
        echo "      sudo apt install git"
    elif [ "$OS_TYPE" = "Darwin" ]; then
        echo "  - Install Homebrew if not already installed:"
        echo "      /bin/bash -c \"$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)\""
        echo "  - Install arm-none-eabi-gcc:"
        echo "      brew tap ArmMbed/homebrew-formulae && brew install arm-none-eabi-gcc"
        echo "  - Install CMake and Make:"
        echo "      brew install cmake make"
        echo "  - Install Git:"
        echo "      brew install git"
    else
        echo "  - Unsupported OS. Please install the required executables manually."
    fi
    exit 1
else
    echo "All required executables are present."
fi

# Ensure build.sh is executable
chmod +x build.sh

echo "Setup completed successfully."
```

### Make the Script Executable

Run the following command to make `setup_project.sh` executable:

```bash
chmod +x setup_project.sh
```

---

## 3. Build Script (`build.sh`)

This script manages the build process using CMake and Make. It allows specifying the target STM32 controller via an environment variable or a command-line argument.

### Create `build.sh`

Create a file named `build.sh` in the root directory with the following content:

```bash
#!/bin/bash

# Exit immediately if a command exits with a non-zero status
set -e

# Default target controller
DEFAULT_TARGET="STM32H743ZITx"

# Check if a target is provided as an argument
if [ -n "$1" ]; then
    TARGET="$1"
else
    TARGET="${DEFAULT_TARGET}"
fi

echo "Starting build process for target: $TARGET"

# Create a build directory specific to the target
BUILD_DIR="build/${TARGET}"
mkdir -p "${BUILD_DIR}"
cd "${BUILD_DIR}"

# Run CMake to configure the project with the specified target
cmake -DCMAKE_TOOLCHAIN_FILE=../../toolchain-arm-none-eabi.cmake -DTARGET_CONTROLLER=${TARGET} ../../

# Build the project using all available cores
make -j$(nproc)

echo "Build completed successfully for target: $TARGET"
```

### Make the Script Executable

Run the following command to make `build.sh` executable:

```bash
chmod +x build.sh
```

### Usage

To build the project for a specific target, run:

```bash
./build.sh STM32H750XB
```

If no target is specified, it defaults to `STM32H743ZITx`.

---

## 4. CMake Configuration

### a. Toolchain File (`toolchain-arm-none-eabi.cmake`)

The toolchain file tells CMake how to use the ARM cross-compiler.

#### Create `toolchain-arm-none-eabi.cmake`

Create a file named `toolchain-arm-none-eabi.cmake` in the root directory with the following content:

```cmake
# toolchain-arm-none-eabi.cmake

# Define the cross compiler
SET(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_SYSTEM_PROCESSOR arm)

# Specify the cross compiler binaries
SET(CMAKE_C_COMPILER arm-none-eabi-gcc)
SET(CMAKE_CXX_COMPILER arm-none-eabi-g++)

# Specify the target environment
SET(CMAKE_C_FLAGS "-mcpu=cortex-m7 -mthumb -O0 -g3")
SET(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=c++17")
SET(CMAKE_EXE_LINKER_FLAGS "-T${CMAKE_LINKER_SCRIPT} -mcpu=cortex-m7 -mthumb -nostdlib -Wl,--gc-sections")

# Specify the path to the linker script (to be set later)
SET(CMAKE_LINKER_SCRIPT "")

# Disable standard libraries
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
```

**Note:** The `CMAKE_LINKER_SCRIPT` variable will be set in the main `CMakeLists.txt` based on the target controller.

### b. Root `CMakeLists.txt`

This file sets up the project, handles different targets, includes modules, and configures the linker script.

#### Create `CMakeLists.txt`

Create `CMakeLists.txt` in the root directory with the following content:

```cmake
cmake_minimum_required(VERSION 3.15)

# Project name and version
project(STM32Project VERSION 1.0 LANGUAGES C CXX)

# Retrieve the target controller from the CMake variable
if(NOT DEFINED TARGET_CONTROLLER)
    message(FATAL_ERROR "TARGET_CONTROLLER is not defined. Use -DTARGET_CONTROLLER=<controller> when running CMake.")
endif()

message(STATUS "Configuring for target controller: ${TARGET_CONTROLLER}")

# Set linker script based on target controller
set(LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/linker_scripts/${TARGET_CONTROLLER}_FLASH.ld")

if(NOT EXISTS "${LINKER_SCRIPT}")
    message(FATAL_ERROR "Linker script not found for target controller: ${TARGET_CONTROLLER}")
endif()

# Pass the linker script path to the toolchain file
set(CMAKE_LINKER_SCRIPT "${LINKER_SCRIPT}" CACHE STRING "Linker script")

# Enable language standards
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED TRUE)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/drivers/CMSIS
    ${CMAKE_SOURCE_DIR}/drivers/HAL
)

# Add subdirectories for modules
add_subdirectory(src/modules/ModuleA)
add_subdirectory(src/modules/ModuleB)

# Add executable
add_executable(${PROJECT_NAME}.elf src/main.c src/dual_core/core1.c src/dual_core/core2.c)

# Link modules
target_link_libraries(${PROJECT_NAME}.elf PRIVATE ModuleA ModuleB)

# Specify the custom linker script
set_target_properties(${PROJECT_NAME}.elf PROPERTIES LINK_FLAGS "-T${LINKER_SCRIPT}")

# Specify the output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Add custom commands (e.g., post-build)
# Example: Generate binary from ELF
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND arm-none-eabi-objcopy -O binary ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}.elf ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}.bin
    COMMENT "Generating binary file"
)

# Print a message after configuration
message(STATUS "CMake configuration for ${TARGET_CONTROLLER} completed.")
```

**Explanation:**

- **TARGET_CONTROLLER**: Specifies the STM32 controller to target.
- **Linker Script**: Selected based on the target controller.
- **Include Directories**: Includes CMSIS and HAL drivers.
- **Modules**: Adds ModuleA and ModuleB.
- **Executable**: Combines `main.c` with dual-core specific files.
- **Custom Commands**: Converts ELF to binary after build.

### c. Linker Scripts (`linker_scripts/`)

Linker scripts define the memory layout for the microcontroller. You need to provide a linker script for each target controller.

#### Example Linker Script for STM32H743ZITx (`STM32H743ZITx_FLASH.ld`)

Create a file named `STM32H743ZITx_FLASH.ld` in the `linker_scripts/` directory with the following content:

```ld
/* STM32H743ZITx_FLASH.ld */

ENTRY(Reset_Handler)

MEMORY
{
  FLASH (rx)      : ORIGIN = 0x08000000, LENGTH = 2048K
  SRAM (xrw)      : ORIGIN = 0x20000000, LENGTH = 512K
}

SECTIONS
{
  .isr_vector :
  {
    KEEP(*(.isr_vector))
  } >FLASH

  .text :
  {
    *(.text*)
    *(.rodata*)
  } >FLASH

  .data :
  {
    *(.data*)
  } >SRAM AT >FLASH

  .bss :
  {
    *(.bss*)
  } >SRAM

  /DISCARD/ :
  {
    *(.note*)
    *(.comment)
  }
}
```

**Note:** Ensure that you have the correct memory addresses and sizes for each controller. You can obtain these from the STM32 reference manuals or vendor-provided linker scripts.

#### Example Linker Script for STM32H750XB (`STM32H750XB_FLASH.ld`)

Create a file named `STM32H750XB_FLASH.ld` in the `linker_scripts/` directory with the following content:

```ld
/* STM32H750XB_FLASH.ld */

ENTRY(Reset_Handler)

MEMORY
{
  FLASH (rx)      : ORIGIN = 0x08000000, LENGTH = 1024K
  SRAM1 (xrw)     : ORIGIN = 0x20000000, LENGTH = 384K
  SRAM2 (xrw)     : ORIGIN = 0x20060000, LENGTH = 64K
}

SECTIONS
{
  .isr_vector :
  {
    KEEP(*(.isr_vector))
  } >FLASH

  .text :
  {
    *(.text*)
    *(.rodata*)
  } >FLASH

  .data :
  {
    *(.data*)
  } >SRAM1 AT >FLASH

  .bss :
  {
    *(.bss*)
  } >SRAM1

  /DISCARD/ :
  {
    *(.note*)
    *(.comment)
  }
}
```

**Tip:** If you’re targeting additional controllers, create corresponding linker scripts following the controller’s memory specifications.

---

## 5. Modular Configuration for Firmware Components

Organizing firmware components as separate modules enhances maintainability and scalability. Below are examples for `ModuleA` and `ModuleB`.

### a. ModuleA

#### Directory Structure

```
src/modules/ModuleA/
├── CMakeLists.txt
├── ModuleA.c
└── ModuleA.h
```

#### Create `CMakeLists.txt` for ModuleA

Create a file named `CMakeLists.txt` in `src/modules/ModuleA/` with the following content:

```cmake
# src/modules/ModuleA/CMakeLists.txt

add_library(ModuleA STATIC ModuleA.c)

# Include directories for ModuleA
target_include_directories(ModuleA PUBLIC ${CMAKE_SOURCE_DIR}/include)

# Compiler options
target_compile_options(ModuleA PRIVATE -Wall -Wextra -pedantic)
```

#### Create `ModuleA.c`

Create a file named `ModuleA.c` in `src/modules/ModuleA/` with the following content:

```c
// src/modules/ModuleA/ModuleA.c

#include "ModuleA.h"
#include <stdio.h>

void ModuleA_Init(void) {
    // Initialization code for ModuleA
}

void ModuleA_PerformTask(void) {
    printf("ModuleA is performing its task.\n");
}
```

#### Create `ModuleA.h`

Create a file named `ModuleA.h` in `src/modules/ModuleA/` with the following content:

```c
// src/modules/ModuleA/ModuleA.h

#ifndef MODULEA_H
#define MODULEA_H

void ModuleA_Init(void);
void ModuleA_PerformTask(void);

#endif // MODULEA_H
```

### b. ModuleB

#### Directory Structure

```
src/modules/ModuleB/
├── CMakeLists.txt
├── ModuleB.c
└── ModuleB.h
```

#### Create `CMakeLists.txt` for ModuleB

Create a file named `CMakeLists.txt` in `src/modules/ModuleB/` with the following content:

```cmake
# src/modules/ModuleB/CMakeLists.txt

add_library(ModuleB STATIC ModuleB.c)

# Include directories for ModuleB
target_include_directories(ModuleB PUBLIC ${CMAKE_SOURCE_DIR}/include)

# Compiler options
target_compile_options(ModuleB PRIVATE -Wall -Wextra -pedantic)
```

#### Create `ModuleB.c`

Create a file named `ModuleB.c` in `src/modules/ModuleB/` with the following content:

```c
// src/modules/ModuleB/ModuleB.c

#include "ModuleB.h"
#include <stdio.h>

void ModuleB_Init(void) {
    // Initialization code for ModuleB
}

void ModuleB_PerformTask(void) {
    printf("ModuleB is performing its task.\n");
}
```

#### Create `ModuleB.h`

Create a file named `ModuleB.h` in `src/modules/ModuleB/` with the following content:

```c
// src/modules/ModuleB/ModuleB.h

#ifndef MODULEB_H
#define MODULEB_H

void ModuleB_Init(void);
void ModuleB_PerformTask(void);

#endif // MODULEB_H
```

---

## 6. Custom Linker Scripts for Different Controllers

As outlined earlier, linker scripts are crucial for defining the memory layout. Ensure each controller has its own linker script in the `linker_scripts/` directory.

**Tips:**

- **ISR Vector Table**: Ensure the `.isr_vector` section is correctly placed in flash memory.
- **Memory Addresses**: Verify the memory addresses and sizes against the controller's datasheet.
- **Dual-Core Considerations**: For dual-core controllers, you might need separate sections or memory allocations for each core.

---

## 7. Sample Source Code for Dual-Core STM32H7 Series

Dual-core STM32H7 controllers (e.g., STM32H743 with Cortex-M7 and Cortex-M4) require specific handling. Below is a simplified example demonstrating how to structure code for both cores.

### a. Main Application (`main.c`)

Create a file named `main.c` in `src/` with the following content:

```c
// src/main.c

#include "ModuleA.h"
#include "ModuleB.h"

int main(void) {
    // Initialize modules
    ModuleA_Init();
    ModuleB_Init();

    // Perform tasks
    ModuleA_PerformTask();
    ModuleB_PerformTask();

    // Enter infinite loop
    while (1) {
        // Main loop
    }

    return 0;
}
```

### b. Dual-Core Specific Code

Assuming a dual-core setup with Cortex-M7 (Core 1) and Cortex-M4 (Core 2):

#### Core1 (`core1.c`)

Create a file named `core1.c` in `src/dual_core/` with the following content:

```c
// src/dual_core/core1.c

#include "stm32h7xx.h"
#include "ModuleA.h"

void Core1_Init(void) {
    // Initialize peripherals for Core1 (Cortex-M7)
    ModuleA_Init();
}

void Core1_Run(void) {
    while (1) {
        ModuleA_PerformTask();
        // Additional Core1 tasks
    }
}
```

#### Core2 (`core2.c`)

Create a file named `core2.c` in `src/dual_core/` with the following content:

```c
// src/dual_core/core2.c

#include "stm32h7xx.h"
#include "ModuleB.h"

void Core2_Init(void) {
    // Initialize peripherals for Core2 (Cortex-M4)
    ModuleB_Init();
}

void Core2_Run(void) {
    while (1) {
        ModuleB_PerformTask();
        // Additional Core2 tasks
    }
}
```

### c. Header File for Dual-Core (`stm32h7xx.h`)

Create a file named `stm32h7xx.h` in `include/` with the following content:

```c
// include/stm32h7xx.h

#ifndef STM32H7XX_H
#define STM32H7XX_H

#include "stm32h7xx_hal.h"

// Function prototypes for dual-core initialization
void Core1_Init(void);
void Core1_Run(void);
void Core2_Init(void);
void Core2_Run(void);

#endif // STM32H7XX_H
```

**Note:** Replace `stm32h7xx_hal.h` with the actual HAL header file corresponding to your STM32H7 controller. Ensure that the `drivers/HAL/` directory contains the necessary HAL libraries.

---

## 8. Final Steps and Usage

### a. Initialize Git Repository (Optional)

If you wish to use Git for version control, follow these steps:

```bash
cd STM32Project
git init
echo "build/" > .gitignore
echo "CMakeFiles/" >> .gitignore
echo "cmake_install.cmake" >> .gitignore
echo "Makefile" >> .gitignore
echo "*.elf" >> .gitignore
echo "*.bin" >> .gitignore
echo "*.o" >> .gitignore
echo "*.d" >> .gitignore
echo "*.exe" >> .gitignore
```

### b. Run Setup Script

Execute the setup script to check for dependencies:

```bash
./setup_project.sh
```

- **If all dependencies are present**, it will proceed without issues.
- **If any dependencies are missing**, it will list them and provide installation instructions based on your operating system.

### c. Configure and Build the Project

Use the build script to configure and compile the project for your target controller.

#### Example: Building for STM32H743ZITx

```bash
./build.sh STM32H743ZITx
```

#### Example: Building for STM32H750XB

```bash
./build.sh STM32H750XB
```

- The script will create a `build/<TARGET>` directory, configure the project with CMake using the specified toolchain and linker script, and compile the code using all available CPU cores.

### d. Flashing the Firmware

After a successful build, you'll have the firmware binary (`.bin`) in the `build/<TARGET>/bin/` directory. You can flash it to your STM32 controller using tools like `openocd`, `st-flash`, or STM32CubeProgrammer.

#### Example Using OpenOCD

1. **Install OpenOCD** (if not already installed):

   - **Linux:**

     ```bash
     sudo apt install openocd
     ```

   - **macOS:**

     ```bash
     brew install openocd
     ```

2. **Connect Your STM32 Board** to your computer via USB.

3. **Flash the Binary:**

   ```bash
   openocd -f interface/stlink.cfg -f target/stm32h7x.cfg -c "program build/<TARGET>/bin/STM32Project.bin verify reset exit"
   ```

   - Replace `<TARGET>` with your target controller (e.g., `STM32H743ZITx`).
   - Ensure that the OpenOCD configuration files (`interface/stlink.cfg` and `target/stm32h7x.cfg`) are correctly specified based on your hardware.

### e. Running the Executable

For embedded systems like STM32, you typically don't "run" the executable on your computer. Instead, you flash it to the microcontroller and reset the device to execute the firmware.

**Expected Behavior:**

- **LEDs**: If your firmware toggles LEDs, you should observe the LEDs blinking as programmed.
- **Serial Output**: If using UART for debugging, you should see the printed messages from `ModuleA` and `ModuleB` on a serial terminal.

---

## 9. Additional Customizations

### a. Adding More Modules

To add additional firmware modules, follow these steps:

1. **Create Module Directory:**

   ```bash
   mkdir -p src/modules/ModuleC
   ```

2. **Add `CMakeLists.txt` in `ModuleC`:**

   ```cmake
   # src/modules/ModuleC/CMakeLists.txt

   add_library(ModuleC STATIC ModuleC.c)

   # Include directories for ModuleC
   target_include_directories(ModuleC PUBLIC ${CMAKE_SOURCE_DIR}/include)

   # Compiler options
   target_compile_options(ModuleC PRIVATE -Wall -Wextra -pedantic)
   ```

3. **Create Source and Header Files:**

   - **Source File:** `src/modules/ModuleC/ModuleC.c`

     ```c
     // src/modules/ModuleC/ModuleC.c

     #include "ModuleC.h"
     #include <stdio.h>

     void ModuleC_Init(void) {
         // Initialization code for ModuleC
     }

     void ModuleC_PerformTask(void) {
         printf("ModuleC is performing its task.\n");
     }
     ```

   - **Header File:** `src/modules/ModuleC/ModuleC.h`

     ```c
     // src/modules/ModuleC/ModuleC.h

     #ifndef MODULEC_H
     #define MODULEC_H

     void ModuleC_Init(void);
     void ModuleC_PerformTask(void);

     #endif // MODULEC_H
     ```

4. **Include the Module in Root `CMakeLists.txt`:**

   Add the following line in `CMakeLists.txt` after adding other subdirectories:

   ```cmake
   add_subdirectory(src/modules/ModuleC)
   ```

5. **Link the Module to the Executable:**

   Modify the `target_link_libraries` line in `CMakeLists.txt`:

   ```cmake
   target_link_libraries(${PROJECT_NAME}.elf PRIVATE ModuleA ModuleB ModuleC)
   ```

6. **Use the Module in `main.c`:**

   Update `main.c` to initialize and perform tasks for ModuleC:

   ```c
   // src/main.c

   #include "ModuleA.h"
   #include "ModuleB.h"
   #include "ModuleC.h"

   int main(void) {
       // Initialize modules
       ModuleA_Init();
       ModuleB_Init();
       ModuleC_Init();

       // Perform tasks
       ModuleA_PerformTask();
       ModuleB_PerformTask();
       ModuleC_PerformTask();

       // Enter infinite loop
       while (1) {
           // Main loop
       }

       return 0;
   }
   ```

### b. Custom Build Commands and Compile Options

You can customize build commands and compiler options in the respective `CMakeLists.txt` files.

- **Enable All Warnings Globally:**

  Modify the root `CMakeLists.txt`:

  ```cmake
  if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
      add_compile_options(-Wall -Wextra -pedantic)
  endif()
  ```

- **Add Preprocessor Definitions:**

  ```cmake
  target_compile_definitions(${PROJECT_NAME}.elf PRIVATE DEBUG_MODE=1)
  ```

### c. Handling External Dependencies

If your project relies on external libraries (e.g., FreeRTOS, LWIP), follow these steps:

1. **Include External Libraries:**

   - **Download or Add as Submodules:**
     
     Place the external library source code in a dedicated directory, e.g., `external/FreeRTOS/`.

   - **Add to CMake:**

     ```cmake
     add_subdirectory(external/FreeRTOS)
     target_link_libraries(${PROJECT_NAME}.elf PRIVATE FreeRTOS)
     ```

2. **Include Paths:**

   Ensure that the include directories for external libraries are added:

   ```cmake
   include_directories(${CMAKE_SOURCE_DIR}/external/FreeRTOS/include)
   ```

3. **Configure Library-Specific Settings:**

   Some libraries may require specific compiler flags or definitions. Handle these in the respective `CMakeLists.txt` files.

### d. Dual-Core Specific Considerations

For dual-core STM32H7 controllers:

- **Separate Initialization:**
  
  Initialize peripherals and modules specific to each core in `core1.c` and `core2.c`.

- **Inter-Core Communication:**
  
  Implement mechanisms like shared memory, semaphores, or message queues for inter-core communication if needed.

- **Synchronization:**
  
  Ensure proper synchronization between cores to prevent race conditions.

- **Separate Tasks:**
  
  Assign different tasks to each core to leverage the dual-core architecture effectively.

**Example:**

In `core1.c` (Cortex-M7):

```c
void Core1_Run(void) {
    while (1) {
        ModuleA_PerformTask();
        // Additional tasks for Core1
    }
}
```

In `core2.c` (Cortex-M4):

```c
void Core2_Run(void) {
    while (1) {
        ModuleB_PerformTask();
        // Additional tasks for Core2
    }
}
```

### e. Cross-Platform Considerations

Ensure that your code and build configurations account for differences between macOS and Linux, such as file paths and toolchain availability.

- **Path Separators:**

  Use forward slashes (`/`) in paths as they are compatible with both macOS and Linux.

- **Line Endings:**

  Ensure that scripts use Unix-style line endings (`LF`).

- **Toolchain Path:**

  If the ARM toolchain is installed in different locations, consider allowing the user to specify the path via an environment variable.

---

## 10. Troubleshooting Tips

- **Permission Issues:**

  Ensure that both `setup_project.sh` and `build.sh` are executable. If not, make them executable using:

  ```bash
  chmod +x setup_project.sh build.sh
  ```

- **Missing Dependencies:**

  If `setup_project.sh` reports missing executables, install them as per the provided instructions and ensure they are in your `PATH`.

- **CMake Errors:**

  - **Linker Script Not Found:**

    Ensure that the linker script exists in the `linker_scripts/` directory and matches the `TARGET_CONTROLLER` specified.

  - **Undefined References:**

    Check that all necessary source files are included and correctly linked.

- **Flashing Issues:**

  - **Connection Problems:**

    Ensure that the STM32 board is properly connected and recognized by your computer.

  - **Incorrect Configuration Files:**

    Verify that the OpenOCD configuration files (`interface` and `target`) match your hardware.

- **Compiler Compatibility:**

  Ensure that the `arm-none-eabi-gcc` version supports the C and C++ standards specified (C11 and C++17).

- **Dual-Core Synchronization:**

  If implementing inter-core communication, ensure proper synchronization to avoid race conditions or deadlocks.

---

## Conclusion

You've now set up a robust, modular CMake-based project tailored for STM32 controllers, including support for dual-core STM32H7 series. This setup allows you to:

- **Cross-Compile**: Using the ARM toolchain (`arm-none-eabi-gcc`).
- **Modular Development**: Easily add or remove firmware modules.
- **Multiple Targets**: Configure and build for different STM32 controllers by specifying the target during the build process.
- **Dual-Core Support**: Separate code for each core with potential for inter-core communication.

**Next Steps:**

1. **Expand Modules**: Add more firmware modules as needed.
2. **Implement Drivers**: Utilize STM32 HAL or write custom drivers for peripherals.
3. **Integrate RTOS**: Consider integrating an RTOS like FreeRTOS for task management.
4. **Debugging**: Use debugging tools (e.g., GDB with OpenOCD) to debug your firmware.
5. **Optimization**: Optimize compiler flags and linker scripts for performance and memory usage.

