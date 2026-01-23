# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Overview

QMK keymap for Mode SixtyFive keyboard (mode/m256wh). Builds two variants: Mac (default) and Windows (gaming-optimized with features disabled for lower latency).

## Build Commands

```bash
# Build both variants (requires Docker)
./build.sh

# Build specific variant
./build.sh mac
./build.sh windows

# Interactive QMK shell
docker-compose run --rm qmk-shell
```

Output files go to `build/` directory as `.bin` files.

## Key Files

- **keymap.c** - Mac variant keymap (GUI/Alt for Mac convention)
- **keymap_windows.c** - Windows variant keymap (standard Ctrl/Alt layout, Ctrl+C/V/Z shortcuts on Layer 2)
- **rules.mk** - Mac build features (RGB, Caps Word enabled)
- **rules_windows.mk** - Windows build features (minimal for low latency gaming)
- **config.h** - RGB defaults and Caps Word configuration

## Build System

The Docker build clones QMK firmware and copies this keymap into `keyboards/mode/m256wh/keymaps/markupboy/`. The build script swaps keymap and rules files based on the variant before building.

GitHub Actions builds both variants on push to main and creates releases with firmware binaries.

## Keymap Editing Rules

**Do not change existing key mappings without explicit user approval.** Overwriting no-op/transparent keys (`_______`, `KC_TRNS`, `KC_NO`, `XXXXXXX`) is allowed.

## Keymap Layers

- **Layer 0**: Base QWERTY layout
- **Layer 1**: Function keys, media controls, AS_TOGG (auto-shift toggle), QK_BOOT (bootloader)
- **Layer 2**: Navigation (arrow keys on WASD and HJKL), Ctrl+C/V/Z shortcuts (Windows), QK_BOOT
