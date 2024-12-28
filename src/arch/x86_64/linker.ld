ENTRY(start)
OUTPUT_FORMAT(elf64-x86-64)

KERNEL_OFFSET = 0xffffffff80000000;
KERNEL_START = 2M;

SECTIONS {
	. = KERNEL_START + KERNEL_OFFSET;
	kernel_start = .;
    .multiboot ALIGN(4K) : AT(ADDR(.multiboot) - KERNEL_OFFSET)
	{
		*(.multiboot)
	}

	.text ALIGN(4K) : AT(ADDR(.text) - KERNEL_OFFSET)
	{
		*(.text)
		*(.gnu.linkonce.t*)
	}

	/* Read-only data. */
	.rodata ALIGN(4K) : AT(ADDR(.rodata) - KERNEL_OFFSET)
	{
		*(.rodata)
		*(.gnu.linkonce.r*)
	}

	/* Read-write data (initialized) */
	.data ALIGN(4K) : AT(ADDR(.data) - KERNEL_OFFSET)
	{
		*(.data)
		*(.gnu.linkonce.d*)
	}

	/* Read-write data (uninitialized) and stack */
	.bss ALIGN(4K) : AT(ADDR(.bss) - KERNEL_OFFSET)
	{
		*(COMMON)
		*(.bss)
		*(.gnu.linkonce.b*)
	}
	kernel_end = .;
}  
