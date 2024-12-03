ENTRY(start)
OUTPUT_FORMAT(elf64-x86-64)

SECTIONS {
	. = 0x100000;
    .multiboot BLOCK(4K) : ALIGN(4K)
	{
		*(.multiboot)
	}

	.start BLOCK(4K) : ALIGN(4K)
	{
		*(.text)
	}

	/* Read-only data. */
	.rodata BLOCK(4K) : ALIGN(4K)
	{
		*(.rodata)
	}

	/* Read-write data (initialized) */
	.data BLOCK(4K) : ALIGN(4K)
	{
		*(.data)
	}

	/* Read-write data (uninitialized) and stack */
	.bss BLOCK(4K) : ALIGN(4K)
	{
		*(COMMON)
		*(.bss)
	}
}  