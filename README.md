# Atari ST creative playground

This is my personal playground to work with Atari ST computers.

## Requirements

This repo will require following tools

- `direnv` (or source `.envrc` when you need to work on this project)
- cmake
- [M68K atari mint cross tools](http://vincent.riviere.free.fr/soft/m68k-atari-mint/)
- zip
- mkfs.fat
- hatari
- root privileges

## How to build

So simple! Run following commands:

```bash
mkdir build
cd build
cmake ..
make
```

## How to run

Also super simple! Run following commands:

```bash
./tools/run.sh
```
