qemu-system-x86_64 -s -S -m 4G -serial stdio -d guest_errors -no-shutdown -no-reboot -cdrom make_iso/corydon.iso &
gdb $1