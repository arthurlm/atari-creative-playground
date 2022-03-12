#!/bin/bash
set -ex

typeset program_name="app.prg"
typeset build_folder="./build"
typeset image_folder="./image"
typeset image_name="image.st"

typeset hatari_options="--confirm-quit 0 --fast-boot 1"

# Make image FS
# See mkfs options: https://linux.die.net/man/8/mkfs.msdos (atari + create + fat12 + image name + block count)
rm -rf "$image_name"
mkfs.fat -ACF 12 "$image_name" 720

# Mount and copy program to image
mkdir -p "$image_folder"
sudo mount -o loop "$image_name" "$image_folder"

sudo mkdir -p "$image_folder/AUTO"
sudo cp "$build_folder/$program_name" "$image_folder/AUTO/$program_name"

sudo umount "$image_folder"

# Debug
set +x
echo
echo "====== BUILD INFO ====="
ls -lah "$build_folder/$program_name"
echo "====== IMAGE INFO ====="
ls -lah *.st
file    *.st
echo "======================="
echo
set -x

# Run image
hatari $hatari_options --disk-a "$image_name"
