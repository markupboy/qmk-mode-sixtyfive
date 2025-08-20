# Building QMK Firmware with Docker

This directory contains a complete Docker and GitHub Actions setup for building the QMK firmware for the Mode M256WH keyboard with the markupboy keymap.

## Quick Start

### Using Docker Compose (Recommended)

```bash
# Build the firmware
docker-compose up qmk-build

# Or run an interactive shell for development
docker-compose run --rm qmk-shell
```

### Using the Build Script

```bash
# Make the script executable (first time only)
chmod +x build.sh

# Run the build
./build.sh
```

### Using Docker Directly

```bash
# Build the container
docker build -t qmk-markupboy .

# Run the build
docker run --rm -v ${PWD}/build:/qmk_firmware/build qmk-markupboy bash -c "qmk compile -kb mode/m256wh -km markupboy && cp mode_m256wh_markupboy.* /qmk_firmware/build/ 2>/dev/null || find . -name 'mode_m256wh_markupboy.*' -exec cp {} /qmk_firmware/build/ \;"
```

## Output

After building, you'll find the firmware files in the `build/` directory:

- `mode_m256wh_markupboy.bin` - Binary firmware file for flashing

## GitHub Actions

The repository includes a GitHub Actions workflow that will automatically build the firmware on:
- Push to main/master branch
- Pull requests
- Manual workflow dispatch

The built firmware will be available as downloadable artifacts from the Actions tab.

## Files Created

- `Dockerfile` - Container definition using official QMK CLI image
- `docker-compose.yml` - Docker Compose configuration with build and shell services
- `build.sh` - Convenience build script
- `.dockerignore` - Files to exclude from Docker build context
- `.github/workflows/build-keymap.yml` - GitHub Actions CI workflow
- `BUILD.md` - This documentation file

## Requirements

- Docker
- Docker Compose (optional, but recommended)

## Troubleshooting

### Build Failures

If the build fails, check:

1. Docker is running
2. You have internet access (to download the QMK CLI image)
3. The keymap files are valid QMK configuration

### Permission Issues

On Linux, you may need to adjust Docker permissions or run with `sudo`.

### Container Updates

To get the latest QMK CLI image:

```bash
docker pull ghcr.io/qmk/qmk_cli:latest
docker-compose build --no-cache
```