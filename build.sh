#!/bin/bash

# QMK Keymap Build Script
set -e

VARIANT=${1:-"all"}

build_keymap() {
    local variant=$1
    local keymap_file=$2
    
    echo "Building QMK firmware for Mode M256WH with ${variant} keymap..."
    
    # Copy the specific keymap file to keymap.c if provided, otherwise use default
    if [ -n "$keymap_file" ]; then
        cp "${keymap_file}" keymap.c
    fi
    
    # Copy variant-specific rules.mk if it exists
    if [ -f "rules_${variant}.mk" ]; then
        echo "Using gaming-optimized rules for ${variant} variant..."
        cp "rules_${variant}.mk" rules.mk
    fi
    
    # Build using Docker
    docker run --rm -v ${PWD}:/qmk_firmware/keyboards/mode/m256wh/keymaps/markupboy qmk-markupboy bash -c "qmk compile -kb mode/m256wh -km markupboy && cp mode_m256wh_markupboy.* /qmk_firmware/keyboards/mode/m256wh/keymaps/markupboy/build/ 2>/dev/null || find . -name 'mode_m256wh_markupboy.*' -exec cp {} /qmk_firmware/keyboards/mode/m256wh/keymaps/markupboy/build/ \;"
    
    # Restore original rules.mk after Windows build
    if [ -f "rules_${variant}.mk" ] && [ "$variant" = "windows" ]; then
        git checkout rules.mk 2>/dev/null || echo "TAP_DANCE_ENABLE = yes\nRGBLIGHT_ENABLE = yes\nAUTO_SHIFT_ENABLE = yes\nCAPS_WORD_ENABLE = yes\n" > rules.mk
    fi
    
    # Rename files with variant suffix
    for file in build/mode_m256wh_markupboy.*; do
        if [ -f "$file" ]; then
            extension="${file##*.}"
            mv "$file" "build/mode_m256wh_markupboy_${variant}.${extension}"
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