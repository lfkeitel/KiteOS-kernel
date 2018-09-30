; Load 'dh' sectors from drive 'dl' starting at sector 'cl' into buffer ES:BX
;
; disk_load(dh=num_sectors, dl=drive, cl=sector_start, bx=dest_buffer)
disk_load:
    pusha
    ; reading from disk requires setting specific values in all registers
    ; so we will overwrite our input parameters from 'dx'. Let's save it
    ; to the stack for later use.
    push dx

    mov ah, 0x02    ; ah <- int 0x13 function. 0x02 = 'read'
    mov al, dh      ; al <- number of sectors to read (0x01 .. 0x80)
    mov ch, 0x00    ; ch <- cylinder (0x0 .. 0x3FF, upper 2 bits in 'cl')
                    ; cl <- starting sector
                    ; dl <- drive number. Our caller sets it as a parameter and gets it from BIOS
                    ; (0 = floppy, 1 = floppy2, 0x80 = hdd, 0x81 = hdd2)
    mov dh, 0x00    ; dh <- head number (0x0 .. 0xF)

    ; [es:bx] <- pointer to buffer where the data will be stored
    ; caller sets it up for us, and it is actually the standard location for int 13h
    int BIOS_DISK_INT
    jc disk_load_error ; if error (stored in the carry bit)

    pop dx
    cmp al, dh    ; BIOS also sets 'al' to the # of sectors read. Compare it.
    jne disk_load_sectors_error
    popa
    ret

disk_load_error:
    mov bx, DISK_ERROR
    call print
    mov dh, ah ; ah = error code, dl = disk drive that dropped the error
    call print_hex ; check out the code at http://stanislavs.org/helppc/int_13-1.html
    jmp disk_load_loop

disk_load_sectors_error:
    mov bx, SECTORS_ERROR
    call print

disk_load_loop:
    jmp $

DISK_ERROR: db "Disk read error", 0
SECTORS_ERROR: db "Incorrect number of sectors read", 0
