#!/usr/bin/env bash

# =============================================================================
# build.sh
# =============================================================================
#
# Description:
#   A build script for macOS that uses CMake with the Ninja generator to
#   configure and build a project. It uses a single 'build' directory, deletes
#   its contents before each build to avoid generator mismatches, and ensures
#   that the user remains in the original directory after the build completes.
#
# Usage:
#   ./build.sh [build_type] [additional_cmake_options]
#
#   - build_type: Optional. Defaults to "Release". Examples: Debug, RelWithDebInfo, MinSizeRel.
#   - additional_cmake_options: Any additional options you want to pass to CMake.
#
# Requirements:
#   - CMake installed (https://cmake.org/download/)
#   - Ninja installed (https://ninja-build.org/)
#
# =============================================================================

# Exit immediately if a command exits with a non-zero status
set -e

# Function to display error messages and exit
function error_exit {
    echo "Error: $1" >&2
    exit 1
}

# Default build type
BUILD_TYPE="Release"

# Parse command-line arguments
if [[ $# -ge 1 ]]; then
    case $1 in
        Debug|Release|RelWithDebInfo|MinSizeRel)
            BUILD_TYPE="$1"
            shift
            ;;
        *)
            echo "Unknown build type: $1"
            echo "Valid build types: Debug, Release, RelWithDebInfo, MinSizeRel"
            exit 1
            ;;
    esac
fi

# Remaining arguments are additional CMake options
ADDITIONAL_OPTIONS="$@"

# Check if CMake is installed
if ! command -v cmake &> /dev/null
then
    error_exit "CMake could not be found. Please install CMake and try again."
fi

# Check if Ninja is installed
if ! command -v ninja &> /dev/null
then
    error_exit "Ninja could not be found. Please install Ninja and try again."
fi

# Get the absolute path of the directory where the script is located
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/99_build/cmake_recipes/STM32F413/fbl"
PROJ_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Define the build directory (e.g., "build" inside the script's directory)
BUILD_DIR="${PROJ_DIR}/build"

# Inform the user about the build directory and build type
echo "Build type: ${BUILD_TYPE}"
echo "Using build directory: ${BUILD_DIR}"

# Check if the build directory exists
if [ -d "${BUILD_DIR}" ]; then
    echo "Removing existing build directory contents to avoid generator mismatch..."
    rm -rf "${BUILD_DIR:?}/"*
else
    echo "Creating build directory..."
    mkdir -p "${BUILD_DIR}"
fi

# Change to the build directory
cd "${BUILD_DIR}"

# -----------------------------------------------------------------------------
# 1) Configure for CM7, then immediately build
# -----------------------------------------------------------------------------
echo "Configuring the project for CM7..."
cmake -G Ninja \
      -DCMAKE_BUILD_TYPE="${BUILD_TYPE}" \
      -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
      ${ADDITIONAL_OPTIONS} \
      "${SCRIPT_DIR}"

# Build using all available CPU cores
CPU_CORES=$(sysctl -n hw.ncpu)
echo "Building the project (CM7) with ${CPU_CORES} parallel jobs..."
cmake --build . -- -j"${CPU_CORES}"

# Optionally rename/copy the artifacts so they don't get overwritten
# if [ -f "FBL_STM32_CM7.hex" ]; then
#    cp "FBL_STM32_CM7.hex" "FBL_STM32_CM7_${BUILD_TYPE}.hex"
# fi
# if [ -f "FBL_STM32_CM7.bin" ]; then
#    cp "FBL_STM32_CM7.bin" "FBL_STM32_CM7_${BUILD_TYPE}.bin"
# fi

# Inform the user that the build was successful
echo "Build completed successfully."