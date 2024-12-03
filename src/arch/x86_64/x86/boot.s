bits 32

PAGE_ALIGN EQU 1 << 0
MEM_INFO EQU 1 << 1

MBOOT_FLAGS EQU PAGE_ALIGN | MEM_INFO

section .multiboot
header_start:
	dd 0xe85250d6 
	dd MBOOT_FLAGS 
	dd header_end - header_start
	dd 0x100000000 - (0xe85250d6 + MBOOT_FLAGS + (header_end - header_start))
	dw 0
	dw 0
	dd 0
header_end:

section .bss
align 4096
page_table_l4:
	resb 4096
page_table_l3:
	resb 4096
page_table_l2:
	resb 4096
stack_bottom:
	resb 4096 * 4
stack_top:

;section .rodata
section .data
gdt64:
	dq 0
.code_segment: equ $ - gdt64
	dq (1 << 43) | (1 << 44) | (1 << 47) | (1 << 53)
.pointer:
	dw $ - gdt64 - 1
	dq gdt64

section .text
; https://github.com/davidcallanan/os-series/blob/96f838b100b2c1c5800294618b4e8273f411b8ea/src/impl/x86_64/boot/main.asm
setup_page_tables:
	mov eax, page_table_l3
	or eax, 0b11 ; present, writable
	mov [page_table_l4], eax
	
	mov eax, page_table_l2
	or eax, 0b11 ; present, writable
	mov [page_table_l3], eax

	mov ecx, 0 ; counter
.loop:

	mov eax, 0x200000 ; 2MiB
	mul ecx
	or eax, 0b10000011 ; present, writable, huge page
	mov [page_table_l2 + ecx * 8], eax

	inc ecx ; increment counter
	cmp ecx, 512 ; checks if the whole table is mapped
	jne .loop ; if not, continue

	ret

enable_paging:
	; pass page table location to cpu
	mov eax, page_table_l4
	mov cr3, eax

	; enable PAE
	mov eax, cr4
	or eax, 1 << 5
	mov cr4, eax

	; enable long mode
	mov ecx, 0xC0000080
	rdmsr
	or eax, 1 << 8
	wrmsr

	; enable paging
	mov eax, cr0
	or eax, 1 << 31
	mov cr0, eax

	ret

global start
start:
	mov esp, stack_top
	call setup_page_tables
	call enable_paging
	
	lgdt [gdt64.pointer]
	jmp gdt64.code_segment:long_start

    nop
    hlt

bits 64
extern kmain
long_start:
	cli
	mov ax, 0
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
	call kmain
	nop
	hlt