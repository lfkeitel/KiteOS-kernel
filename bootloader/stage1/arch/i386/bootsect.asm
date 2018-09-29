%define memmap_enable 1
MEM_MAP_START equ 0x800
KERNEL_OFFSET equ 0x1000 ; The same one we used when linking the kernel
STACK_BASE equ 0x9000

[org 0x7c00]
    mov [BOOT_DRIVE], dl ; Remember that the BIOS sets us the boot drive in 'dl' on boot

    mov ax, 0            ; Ensure extra segment is zero
    mov es, ax

%ifdef memmap_enable
    mov di, MEM_MAP_START        ; Store memory map at 0x800
    call memory_map      ; Get memory map
    mov [MEM_MAP_START-2], bp      ; Store record count just before memory map
%else
    mov byte [MEM_MAP_START-2], 0
%endif

    mov bp, STACK_BASE   ; Setup starting stack
    mov sp, bp

    call load_kernel     ; read the kernel from disk
    call switch_to_pm    ; disable interrupts, load GDT,  etc. Finally jumps to 'BEGIN_PM'
    jmp $                ; Never executed

%ifdef memmap_enable
%include "arch/i386/memory_map.asm"
%endif
%include "arch/i386/print.asm"
%include "arch/i386/print_hex.asm"
%include "arch/i386/disk.asm"
%include "arch/i386/gdt.asm"
%include "arch/i386/switch_pm.asm"

[bits 16]
load_kernel:
    mov bx, MSG_LOAD_KERNEL
    call print
    call print_nl

    mov bx, KERNEL_OFFSET ; Read from disk and store in 0x1000
    mov dh, 31 ; Our future kernel will be larger, make this big
    mov dl, [BOOT_DRIVE]
    call disk_load
    ret

[bits 32]
BEGIN_PM:
    call KERNEL_OFFSET ; Give control to the kernel
    jmp $ ; Stay here when the kernel returns control to us (if ever)


BOOT_DRIVE db 0 ; It is a good idea to store it in memory because 'dl' may get overwritten
MSG_LOAD_KERNEL db "Loading kernel", 0

; padding
%ifdef memmap_enable
times 510 - ($-$$) db 0
%else
times 436 - ($-$$) db 0

UID times 10 db 0             ; Unique Disk ID
PT1 times 16 db 0             ; First Partition Entry
PT2 times 16 db 0             ; Second Partition Entry
PT3 times 16 db 0             ; Third Partition Entry
PT4 times 16 db 0             ; Fourth Partition Entry
%endif

dw 0xaa55
