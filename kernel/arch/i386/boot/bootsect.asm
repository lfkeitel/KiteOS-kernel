KERNEL_OFFSET equ 0x1000 ; The same one we used when linking the kernel
STACK_BASE equ 0x9000

[org 0x7c00]
    mov [BOOT_DRIVE], dl ; Remember that the BIOS sets us the boot drive in 'dl' on boot

    mov ax, 0            ; Ensure extra segment is zero
    mov es, ax

    mov di, 0x800        ; Store memory map at 0x800
    call memory_map      ; Get memory map
    mov [0x7fe], bp      ; Store record count just before memory map

    mov bp, STACK_BASE   ; Setup starting stack
    mov sp, bp

    call load_kernel     ; read the kernel from disk
    call switch_to_pm    ; disable interrupts, load GDT,  etc. Finally jumps to 'BEGIN_PM'
    jmp $                ; Never executed

%include "arch/i386/boot/memory_map.asm"
%include "arch/i386/boot/print.asm"
%include "arch/i386/boot/print_hex.asm"
%include "arch/i386/boot/disk.asm"
%include "arch/i386/boot/gdt.asm"
%include "arch/i386/boot/switch_pm.asm"

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
times 510 - ($-$$) db 0
dw 0xaa55
