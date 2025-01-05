#!/usr/bin/env bash
#
# build.sh

# Exit immediately if a command exits with a non-zero status
set -e

# Optional: fail on first command in a pipeline that errors
# set -o pipefail

TEMP_DIR=".temp_build"

BUILD_DIR="build"

echo "=== Building FBL ==="
./build_cm7_fbl.sh || { echo "FBL build failed"; exit 1; }

#read -rp "Press Enter to continue..."

echo "=== Store FBL to: $TEMP_DIR ==="
mkdir -p "$TEMP_DIR"

cp "$BUILD_DIR"/*.map "$TEMP_DIR" 2>/dev/null || true
cp "$BUILD_DIR"/*.out "$TEMP_DIR" 2>/dev/null || true
cp "$BUILD_DIR"/*.hex "$TEMP_DIR" 2>/dev/null || true
cp "$BUILD_DIR"/*.bin "$TEMP_DIR" 2>/dev/null || true

# read -rp "Press Enter to continue..."

echo "=== Building ASW ==="
./build_cm7_asw.sh || { echo "ASW build failed"; exit 1; }

# read -rp "Press Enter to continue..."

echo "=== Copying temporary files from $TEMP_DIR back to $BUILD_DIR ==="
cp "$TEMP_DIR"/* "$BUILD_DIR" 2>/dev/null || true

# read -rp "Press Enter to continue..."

echo "=== Cleaning up: removing $TEMP_DIR ==="
rm -rf "$TEMP_DIR"

# read -rp "Press Enter to continue..."

echo "=== Build script completed successfully! ==="
