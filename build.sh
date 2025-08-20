#!/bin/bash

# QMK Keymap Build Script
set -e

echo "Building QMK firmware for Mode M256WH with markupboy keymap..."

# Create build directory
mkdir -p build

# Build using Docker
echo "Building with Docker..."
docker build -t qmk-markupboy .
docker run --rm -v ${PWD}/build:/qmk_firmware/build qmk-markupboy bash -c "qmk compile -kb mode/m256wh -km markupboy && cp mode_m256wh_markupboy.* /qmk_firmware/build/ 2>/dev/null || find . -name 'mode_m256wh_markupboy.*' -exec cp {} /qmk_firmware/build/ \;"

echo "Build complete! Check the 'build' directory for firmware files."
echo "Built files:"
find build/ -name "mode_m256wh_markupboy.*" -type f 2>/dev/null || echo "No firmware files found in build directory"