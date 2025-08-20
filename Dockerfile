# QMK Firmware Build Container for Mode M256WH Markupboy Keymap
FROM ghcr.io/qmk/qmk_cli:latest

# Set working directory
WORKDIR /qmk_firmware

# Clone QMK firmware repository
RUN git clone --recurse-submodules --depth 1 https://github.com/qmk/qmk_firmware.git .

# Copy our keymap files
COPY . /qmk_firmware/keyboards/mode/m256wh/keymaps/markupboy/

# Set default command to build the keymap
CMD ["qmk", "compile", "-kb", "mode/m256wh", "-km", "markupboy"]