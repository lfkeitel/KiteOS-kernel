%ifndef stage2_sect_count
%define stage2_sect_count 1
%endif

STAGE2_ADDR             equ 0x7c00
STACK_BASE              equ 0x9000
MBR_MAGIC               equ 0xaa55
STAGE2_MBR_MAGIC_ADDR   equ 0x7dfe

[org 0x0600]
start:
    cli               ; We do not want to be interrupted
    xor ax, ax        ; 0 AX
    mov ds, ax        ; Set Data Segment to 0
    mov es, ax        ; Set Extra Segment to 0
    mov ss, ax        ; Set Stack Segment to 0
    mov sp, ax        ; Set Stack Pointer to 0

copy_down:
    mov cx, 0x0100    ; 256 words in MBR
    mov si, 0x7c00    ; Current MBR Address
    mov di, 0x0600    ; New MBR Address
    rep movsw         ; Copy MBR
    jmp 0:bootloader  ; Jump to new Address

bootloader:
    sti
    mov [BOOT_DRIVE], dl ; BIOS sets the boot drive in 'dl'

    mov bp, STACK_BASE   ; Setup stack
    mov sp, bp

    call load_stage2     ; read stage 2 from disk

    ; cmp WORD [STAGE2_MBR_MAGIC_ADDR], MBR_MAGIC ; Check for valid boot record
    ; jne bootloader_error
    mov si, PT1                  ; Give stage 2 address to partition table
    mov dl, [BOOT_DRIVE]         ; Give stage 2 the same boot drive
    jmp 0:STAGE2_ADDR            ; Hand the reins to stage 2

%include "arch/i386/print.asm"
%include "arch/i386/print_hex.asm"
%include "arch/i386/disk.asm"

load_stage2:
    mov bx, MSG_LOAD_STAGE2
    call print

    mov bx, STAGE2_ADDR       ; Buffer to read into
    mov dh, stage2_sect_count ; Read stage 2 sector size. TODO: Replace with actual sector size of stage 2
    mov dl, [BOOT_DRIVE]      ; From the same drive as we booted from
    call disk_load
    ret

bootloader_error:
    mov bx, MSG_BOOTLOADER_ERROR
    call print
    jmp $

BOOT_DRIVE db 0
MSG_LOAD_STAGE2 db "Loading stage 2", 0
MSG_BOOTLOADER_ERROR db "Bad boot sector", 0

times 436 - ($-$$) db 0 ; Pad to partition table

UID times 10 db 0  ; Unique Disk ID
PT1 times 16 db 0  ; First Partition Entry
PT2 times 16 db 0  ; Second Partition Entry
PT3 times 16 db 0  ; Third Partition Entry
PT4 times 16 db 0  ; Fourth Partition Entry

dw MBR_MAGIC
