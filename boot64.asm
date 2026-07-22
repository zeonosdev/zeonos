; ZeonOS x64 Bootloader
; Bootloader untuk 64-bit kernel
; Target: BIOS, x86-64

BITS 16
ORG 0x7c00

start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7c00
    sti

    mov si, boot_msg
    call print_string_16

    call check_long_mode
    cmp eax, 0
    je no_long_mode

    call load_gdt

    mov eax, cr0
    or eax, 1
    mov cr0, eax

    jmp 0x08:protected_mode

check_long_mode:
    mov eax, 0x80000000
    cpuid
    cmp eax, 0x80000001
    jl no_long_mode_ret
    
    mov eax, 0x80000001
    cpuid
    test edx, 1 << 29
    jz no_long_mode_ret
    
    mov eax, 1
    ret

no_long_mode_ret:
    xor eax, eax
    ret

no_long_mode:
    mov si, no_lm_msg
    call print_string_16
    hlt

print_string_16:
    lodsb
    or al, al
    jz .done
    mov ah, 0x0e
    int 0x10
    jmp print_string_16
.done:
    ret

load_gdt:
    lgdt [gdt_descriptor]
    ret

BITS 32
protected_mode:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov ss, ax

    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax

    call setup_page_tables

    mov ecx, 0xc0000080
    rdmsr
    or eax, 1 << 8
    wrmsr

    mov eax, cr0
    or eax, 1 << 31
    mov cr0, eax

    jmp 0x08:long_mode_start

setup_page_tables:
    mov eax, 0x2003
    mov dword [0x1000], eax

    mov eax, 0x3003
    mov dword [0x2000], eax

    mov eax, 0x4003
    mov dword [0x3000], eax

    mov eax, 0x4005
    mov ecx, 512
.map_loop:
    mov dword [0x4000 + ecx*8 - 8], eax
    add eax, 0x1000
    loop .map_loop

    mov eax, 0x1000
    mov cr3, eax
    ret

BITS 64
long_mode_start:
    mov rsp, 0x90000

    mov rax, 0x0f200f200f200f20
    mov rcx, 500
    mov rdi, 0xb8000
    rep stosq

    jmp 0x10000

boot_msg: db "ZeonOS Bootloader v1.0", 0x0d, 0x0a, 0
no_lm_msg: db "Error: Long mode not supported!", 0x0d, 0x0a, 0

gdt_start:
    dq 0x0000000000000000
    dq 0x00209a0000000000
    dq 0x0020920000000000
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

times 510 - ($ - $$) db 0
dw 0xaa55
