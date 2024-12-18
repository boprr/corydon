mkdir make_iso
cd make_iso
git clone https://github.com/limine-bootloader/limine.git --branch=v8.x-binary --depth=1
make -C limine
mkdir iso_root

mkdir iso_root/boot
cp -v $1 iso_root/boot/
mkdir iso_root/boot/limine
cp -v ../config/limine.conf limine/limine-bios.sys limine/limine-bios-cd.bin limine/limine-uefi-cd.bin iso_root/boot/limine/

mkdir -p iso_root/EFI/BOOT
cp -v limine/BOOTX64.EFI iso_root/EFI/BOOT/
cp -v limine/BOOTIA32.EFI iso_root/EFI/BOOT/

xorriso -as mkisofs -R -r -J -b boot/limine/limine-bios-cd.bin \
        -no-emul-boot -boot-load-size 4 -boot-info-table -hfsplus \
        -apm-block-size 2048 --efi-boot boot/limine/limine-uefi-cd.bin \
        -efi-boot-part --efi-boot-image --protective-msdos-label \
        iso_root -o corydon.iso

./limine/limine bios-install corydon.iso