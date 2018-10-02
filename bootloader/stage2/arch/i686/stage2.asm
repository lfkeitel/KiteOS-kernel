%ifndef disk_sect_start
%define disk_sect_start 3
%endif
%ifndef kern_sect_num
%define kern_sect_num 50
%endif

%include "arch/i686/bios_interrupts.asm"

MEM_MAP_START equ 0x800
KERNEL_OFFSET equ 0x1000 ; The same one we used when linking the kernel
STACK_BASE equ 0x9000

[org 0x7c00]
    mov [BOOT_DRIVE], dl ; Remember that the BIOS sets us the boot drive in 'dl' on boot

    mov di, MEM_MAP_START+2     ; Store memory map entries at 0x802
    call memory_map             ; Get memory map
    mov [MEM_MAP_START], bp     ; Store record count before entries

    mov bp, STACK_BASE          ; Setup starting stack
    mov sp, bp

    call load_kernel     ; read the kernel from disk
    call switch_to_pm    ; disable interrupts, load GDT,  etc. Finally jumps to 'BEGIN_PM'
    jmp $                ; Never executed

load_kernel:
    mov bx, MSG_LOAD_KERNEL
    call print

    ; TODO: Replace with reading a filesystem and getting real sector size
    mov bx, KERNEL_OFFSET   ; Destination buffer
    mov dh, kern_sect_num   ; Number of sectors to read
    mov dl, [BOOT_DRIVE]    ; Storage device to load from
    mov cl, disk_sect_start ; Starting sector
    call disk_load
    ret

%include "arch/i686/memory_map.asm"
%include "arch/i686/print.asm"
%include "arch/i686/print_hex.asm"
%include "arch/i686/disk.asm"
%include "arch/i686/gdt.asm"
%include "arch/i686/switch_pm.asm"

BOOT_DRIVE db 0 ; It is a good idea to store it in memory because 'dl' may get overwritten
MSG_LOAD_KERNEL db "Loading kernel", 0
