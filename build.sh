#!/bin/bash

# QMK Keymap Build Script
set -e

VARIANT=${1:-"all"}

build_keymap() {
    local variant=$1
    local keymap_file=${2:-"keymap.c"}

    echo "Building QMK firmware for Mode M256WH with ${variant} keymap..."

    # Create temporary build directory to avoid modifying source files
    local build_context=$(mktemp -d)
    trap "rm -rf $build_context" RETURN

    # Copy source files to temp directory
    cp config.h "$build_context/"
    cp "$keymap_file" "$build_context/keymap.c"

    # Use variant-specific rules.mk if it exists, otherwise use default
    if [ -f "rules_${variant}.mk" ]; then
        echo "Using gaming-optimized rules for ${variant} variant..."
        cp "rules_${variant}.mk" "$build_context/rules.mk"
    else
        cp rules.mk "$build_context/"
    fi

    # Create build output directory in temp context
    mkdir -p "$build_context/build"

    # Build using Docker with temp directory
    docker run --rm -v "${build_context}:/qmk_firmware/keyboards/mode/m256wh/keymaps/markupboy" qmk-markupboy bash -c "qmk compile -kb mode/m256wh -km markupboy && cp mode_m256wh_markupboy.* /qmk_firmware/keyboards/mode/m256wh/keymaps/markupboy/build/ 2>/dev/null || find . -name 'mode_m256wh_markupboy.*' -exec cp {} /qmk_firmware/keyboards/mode/m256wh/keymaps/markupboy/build/ \;"

    # Copy build output back and rename with variant suffix
    for file in "$build_context"/build/mode_m256wh_markupboy.*; do
        if [ -f "$file" ]; then
            extension="${file##*.}"
            cp "$file" "build/mode_m256wh_markupboy_${variant}.${extension}"
        fi
    done
}

# Create build directory
mkdir -p build

# Build Docker image once
echo "Building Docker image..."
docker build -t qmk-markupboy .

case $VARIANT in
    "windows")
        build_keymap "windows" "keymap_windows.c"
        ;;
    "mac")
        build_keymap "mac"
        ;;
    "all")
        build_keymap "windows" "keymap_windows.c"
        build_keymap "mac"
        ;;
    *)
        echo "Usage: $0 [windows|mac|all]"
        echo "  windows - Build Windows variant only"
        echo "  mac     - Build Mac variant (default keymap)"  
        echo "  all     - Build both variants (default)"
        exit 1
        ;;
esac

echo "Build complete! Check the 'build' directory for firmware files."
echo "Built files:"
find build/ -name "mode_m256wh_markupboy_*.*" -type f 2>/dev/null || echo "No firmware files found in build directory"