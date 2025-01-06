[BITS 32]

section .multiboot
header_start:
    dd 0xe85250d6
    dd 0
    dd header_end - header_start
    dd 0x100000000 - (0xe85250d6 + 0 + (header_end - header_start))

    dw 0
    dw 0
    dd 0
header_end:

KERNEL_OFFSET equ 0xffffffff80000000

section .bss

align 4096
page:
    .p4:
        resb 4096
    .p3:
        resb 4096
    .p2:
        resb 4096

stack_bottom:
	resb 4096 * 4
stack_top:

section .rodata
gdt64:
	dq 0
.code: equ $ - gdt64
	dq (1 << 43) | (1 << 44) | (1 << 47) | (1 << 53)
.pointer:
	dw $ - gdt64 - 1
	dq gdt64 - KERNEL_OFFSET

section .text
global start
start:
    cli
    mov edi, eax
    mov esi, ebx
    mov esp, (stack_top - KERNEL_OFFSET)

    call setup_page_tables
    call enable_paging
    
    lgdt [(gdt64.pointer - KERNEL_OFFSET)]
    jmp gdt64.code:(longmode_entry - KERNEL_OFFSET)

    cli
    hlt

setup_page_tables:
    mov eax, (page.p3 - KERNEL_OFFSET)
    or eax, 0b11 ; present + writable
    mov [((page.p4 - KERNEL_OFFSET)+0 * 8)], eax 
    mov [((page.p4 - KERNEL_OFFSET)+511 * 8)], eax 

    mov eax, (page.p2 - KERNEL_OFFSET)
    or eax, 0b11 ; present + writable
    mov [((page.p3 - KERNEL_OFFSET)+0 * 8)], eax 
    mov [((page.p3 - KERNEL_OFFSET)+510 * 8)], eax 
    
    mov ecx, 0 ; loop counter
        .loop:
            mov eax, 0x200000 ; 2MiB
            mul ecx
            or eax, 0b10000011 ; present + writable + huge page
            mov [(page.p2 - KERNEL_OFFSET) + ecx * 8], eax
            inc ecx
            cmp ecx, 512
            jne .loop
    ret

enable_paging:
    mov eax, (page.p4 - KERNEL_OFFSET)
    mov cr3, eax

   	mov eax, cr4
	or eax, 1 << 5
	mov cr4, eax

    ; long mode enable (LME)
    mov ecx, 0xC0000080
	rdmsr
	or eax, 1 << 8
	wrmsr

    mov eax, cr0
	or eax, 1 << 31
	mov cr0, eax

    ret

[BITS 64]
extern kmain
longmode_entry:
    mov ax, 0
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    add rsp, KERNEL_OFFSET

    call kmain
    hlt
    
.loop:
    hlt
    jmp $