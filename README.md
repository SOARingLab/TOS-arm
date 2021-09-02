# Teaching Operating System (TOS) - arm version

Yet another Unix-like teaching (or toy, if you prefer) operating system kernel for the ARM ISA (named as TOS-arm).  It was initially ported from MIT's [xv6](https://github.com/mit-pdos/xv6-public/) for [OS Labs](https://github.com/FDUCSLG/OS-2020Fall-Fudan/) at Fudan University (Fall, 2020-2021). It will be maintained and developed for the OS teaching at Fudan. 

Tested on Raspberry Pi 3A+, 3B+, 4B, and QEMU.

## Reference

- [linux](https://github.com/raspberrypi/linux): a real world operating system
- [xv6](https://github.com/mit-pdos/xv6-public/): a simple teaching operating system
- [circle](https://github.com/rsta2/circle): contains lots of portable drivers
- [s-matyukevich's](https://github.com/s-matyukevich/raspberry-pi-os)
- [bztsrc's](https://github.com/bztsrc/raspi3-tutorial)

## What's different?

- We use [musl](https://musl.libc.org/) as user programs libc instead of reinventing one.
- The set of syscalls supported by our kernel is a subset of linux's.
- Compared to xv6, we use a queue-based scheduler and hash pid for process sleeping and waking.

## Features

- [x] AArch64 only
- [x] Basic multi-core support
- [x] Memory management
- [x] Virtual memory without swapping
- [x] Process management
- [x] Disk driver(EMMC): ported from [circle](https://github.com/rsta2/circle/tree/master/addon/SDCard)
- [x] File system: ported from xv6
- [x] C library: [musl](https://musl.libc.org/)
- [x] Shell: ported from xv6
  - [x] Support argc, envp
  - [x] Support pipe

## Prerequisite

For Ubuntu 20.04 on x86, just run `make init` and skip this section.

### GCC toolchain

If your Linux is running natively on ARMv8 CPU, change `CROSS := aarch64-linux-gnu-`
in config.mk to `CROSS := ` to use local gcc.

If your aarch-linux-gnu-gcc(or gcc) version is less than 9.3.0, remove `-mno-outline-atomics` from Makefile.

### QEMU

You can install QEMU from your package manager or compile it from source such as

```
git clone https://github.com/qemu/qemu.git
mkdir -p qemu/build
(cd qemu/build && ../configure --target-list=aarch64-softmmu && make -j8)
```

On some OS such as openEuler or CentOS, you may also need to install the following dependencies

```
yum install ninja-build
yum install pixman-devel.aarch64
```

Then add the generated `qemu-system-aarch64` to PATH or just modify the `QEMU` variable in config.mk.

### Build musl

First, fetch musl by `git submodule update --init --recursive`.

Then if you are cross compiling, run `(cd libc && export CROSS_COMPILE=aarch64-linux-gnu- && ./configure --target=aarch64)`.
Otherwise, run `(cd libc && ./configure)`.

## Development

- `make qemu`: Emulate the kernel at `obj/kernel8.img`.
- `make`: Create a bootable sd card image at `obj/sd.img` for Raspberry Pi 3, which can be burned to a tf card using [Raspberry Pi Imager](https://www.raspberrypi.org/software/).
- `make lint`: Lint source code of kernel and user programs.

### Raspberry Pi 4

It works on Pi 4 as well. Change `RASPI := 3` to `RASPI := 4` in Makefile, run `make clean && make`
and have fun with your Pi 4.

### Logging level

Logging level is controlled via compiler option `-DLOG_XXX` in Makefile, where `XXX` can be one of

- `ERROR`
- `WARN`
- `INFO`
- `DEBUG`
- `TRACE`

Defaults to `-DLOG_INFO`.

### Debug mode

Enabling debug mode via compiler option `-DDEBUG` in Makefile will incorportate runtime assertions,
testing and memory usage profiling(see below). Defaults to `-DNOT_DEBUG`.

### Profile

We can inspect the information of processes and memory usage(shown when `-DDEBUG`) by `Ctrl+P`.
This may output something like

```
1 sleep  init
2 run    idle
3 runble idle
4 run    idle
5 run    idle
6 sleep  sh fa: 1  
```

where each row contains the pid, state, name and father's pid of each process.

## Project structure

```
.
├── Makefile
├── mksd.mk: Part of Makefile for generating bootable image.
|
├── boot: Official boot loader.
├── libc: C library musl.
|
├── inc: Kernel headers.
├── kern: Kernel source code.
└── usr: User programs.
```

## Credit:
- Originally ported by: Hongqing LI, ZhifengHU (2020-2021)
- Maintained by: Xiaoyu HAN, Runyu Peng, Yifan TAN, Zhenliang XUE, Liang ZHANG
