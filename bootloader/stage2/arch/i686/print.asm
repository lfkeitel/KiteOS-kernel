print:
    pusha

; keep this in mind:
; while (string[i] != 0) { print string[i]; i++ }

; the comparison for string end (null byte)
print_loop:
    mov al, [bx] ; 'bx' is the base address for the string
    cmp al, 0
    je print_done

    ; the part where we print with the BIOS help
    mov ah, 0x0e
    int BIOS_VIDEO_INT ; 'al' already contains the char

    ; increment pointer and do next loop
    add bx, 1
    jmp print_loop

print_done:
    mov ah, 0x0e
    mov al, 0x0a ; newline char
    int BIOS_VIDEO_INT
    mov al, 0x0d ; carriage return
    int BIOS_VIDEO_INT

    popa
    ret
