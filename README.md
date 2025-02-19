# Corydon
![GitHub License](https://img.shields.io/github/license/boprr/corydon)
![GitHub Release](https://img.shields.io/github/v/release/boprr/corydon)
![GitHub contributors](https://img.shields.io/github/contributors-anon/boprr/corydon)
![GitHub repo size](https://img.shields.io/github/repo-size/boprr/corydon)
![GitHub commit activity](https://img.shields.io/github/commit-activity/m/boprr/corydon)
![GitHub top language](https://img.shields.io/github/languages/top/boprr/corydon)

![corydon-preview](https://github.com/user-attachments/assets/4304bdec-529d-4248-a759-55771aabda62)

## Roadmap
- architecture specific 
- [x] GDT, IDT, ISR
- [ ] Physical memory manager 
- [ ] Virtual memory manager
- [ ] Ring3 (ELF, user programs)
- [x] RTC (clock)
- drivers
- [x] PS/2 Keyboard
- [x] Serial
- [ ] USB
- [ ] Networking 
- graphics
- [ ] Framebuffer
- filesystem
- [ ] FAT32
- [ ] EXT2
- [ ] VFS
- userspace
- [ ] Syscall
- [ ] Libc
- [ ] Toolchain


## Dependencies
### Requirements
- cmake
- wget
- tar
- git
- xorriso
- system gcc/g++/nasm
- make
- bison
- flex
- gmp
- mpfr
- mpc
- texinfo

### GNU/Linux
- **gentoo** `emerge -a gcc nasm make bison flex gmp dev-libs/mpc mpfr sys-apps/texinfo isl cmake wget app-arch/tar dev-vcs/git libburn libisoburn`
- **arch** `pacman -S base-devel nasm gmp libmpc mpfr wget tar git libburn`
- **debian** `apt install gcc g++ nasm make bison flex libgmp-dev libmpc-dev libmpfr-dev texinfo libisl-dev cmake wget tar git xorriso`

### Windows
[Install WSL](https://learn.microsoft.com/en-us/linux/install#windows-subsystem-for-linux-wsl), then follow the instructions for your distribution.

## Building
```bash
git clone https://github.com/boprr/corydon
cd corydon
mkdir build
cd build
cmake ..
make toolchain
```
### ↳ Testing
```bash
make iso # Building an iso for external use 
make qemu # Testing with qemu
make gdb # Testing using qemu with gdb
```

## Contributing 
1. Fork the repository.
2. Create a new branch: `git checkout -b feature-name`.
3. Make your changes.
4. Push your branch: `git push origin feature-name`.
5. Format your code with clang-format
6. Create a pull request.

## License
This project is licensed under the GNU General Public License v3.0. See the [LICENSE](LICENSE) file for details.
