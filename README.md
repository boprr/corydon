# Corydon kernel
A simple x86_64 kernel

### How to build?
##### you will need:
+ cmake
+ wget
+ tar
+ [gcc build depends](https://gcc.gnu.org/install/prerequisites.html)
+ git
+ xorriso
+ qemu (optional)

```bash
git clone https://github.com/boprr/corydon
cd corydon
mkdir build
cd build
cmake ..
make toolchain
make iso
```
