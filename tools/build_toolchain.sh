mkdir toolchain
cd toolchain
mkdir src
cd src

OPTDIR=$1"/x86_64/"
mkdir -p "$OPTDIR"

#binutils
wget -nc https://ftp.gnu.org/gnu/binutils/binutils-2.43.1.tar.xz
tar -xvf binutils-2.43.1.tar.xz
cd binutils-2.43.1
mkdir build
cd build
../configure --target=x86_64-elf --prefix="$OPTDIR" --disable-nls --with-sysroot --disable-werror
make -j$(nproc)
make install
cd ../../

#gcc
wget -nc https://ftp.gnu.org/gnu/gcc/gcc-14.2.0/gcc-14.2.0.tar.xz
tar -xvf gcc-14.2.0.tar.xz
cd gcc-14.2.0
mkdir build
cd build
../configure --target=x86_64-elf --prefix="$OPTDIR" --disable-nls --enable-languages=c,c++ --without-headers --disable-shared --enable-static
make all-gcc -j$(nproc)
make all-target-libgcc -j$(nproc)
make install-gcc
make install-target-libgcc
cd ../../

#nasm
wget -nc https://www.nasm.us/pub/nasm/releasebuilds/2.16.03/nasm-2.16.03.tar.xz
tar -xvf nasm-2.16.03.tar.xz
cd nasm-2.16.03
./autogen.sh
./configure --prefix="$OPTDIR"
make -j$(nproc)
make install
cd ../../