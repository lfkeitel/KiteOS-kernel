#include <kiteos/screen.h>
#include <kiteos/ports.h>
#include <string.h>
#include <stdint.h>

/* Declaration of private functions */
int get_cursor_offset();
void set_cursor_offset(int offset);
int print_char(char c, int col, int row, char attr);
int scroll_screen_buffer(int cursor_offset);
int get_offset(int col, int row);
int get_offset_row(int offset);
int get_offset_col(int offset);

enum vga_color default_fg = VGA_COLOR_WHITE;
enum vga_color default_bg = VGA_COLOR_BLACK;

/**********************************************************
 * Public Kernel API functions                            *
 **********************************************************/

/**
 * Print a message on the specified location
 * If col, row, are negative, we will use the current offset
 */
void kprint_at(const char *message, int col, int row) {
    /* Set cursor if col/row are negative */
    int offset;
    if (col >= 0 && row >= 0)
        offset = get_offset(col, row);
    else {
        offset = get_cursor_offset();
        row = get_offset_row(offset);
        col = get_offset_col(offset);
    }

    /* Loop through message and print it */
    int i = 0;
    while (message[i] != 0) {
        offset = print_char(message[i++], col, row, vga_entry_color(default_fg, default_bg));
        /* Compute row/col for next iteration */
        row = get_offset_row(offset);
        col = get_offset_col(offset);
    }
}

void kprint(const char *message) {
    kprint_at(message, -1, -1);
}

void kprint_backspace() {
    int offset = get_cursor_offset()-2;
    int row = get_offset_row(offset);
    int col = get_offset_col(offset);
    print_char(' ', col, row, vga_entry_color(default_fg, default_bg));
    set_cursor_offset(offset);
}

void kprint_char(char c) {
    int offset = get_cursor_offset();
    int col = get_offset_col(offset);
    int row = get_offset_row(offset);
    print_char(c, col, row, vga_entry_color(default_fg, default_bg));
}

void kterminal_color(enum vga_color fg, enum vga_color bg) {
    default_fg = fg;
    default_bg = bg;
}

/**********************************************************
 * Private kernel functions                               *
 **********************************************************/


/**
 * Innermost print function for our kernel, directly accesses the video memory
 *
 * If 'col' and 'row' are negative, we will print at current cursor location
 * If 'attr' is zero it will use 'white on black' as default
 * Returns the offset of the next character
 * Sets the video cursor to the returned offset
 */
int print_char(char c, int col, int row, char attr) {
    uint8_t *vidmem = (uint8_t*) VIDEO_ADDRESS;
    if (!attr) attr = vga_entry_color(default_fg, default_bg);

    /* Error control: print a red 'E' if the coords aren't right */
    if (col >= MAX_COLS || row >= MAX_ROWS) {
        vidmem[2*(MAX_COLS)*(MAX_ROWS)-2] = 'E';
        vidmem[2*(MAX_COLS)*(MAX_ROWS)-1] = vga_entry_color(VGA_COLOR_RED, default_fg);
        return get_offset(col, row);
    }

    int offset;
    if (col >= 0 && row >= 0) offset = get_offset(col, row);
    else offset = get_cursor_offset();

    if (c == '\n') {
        row = get_offset_row(offset);
        offset = get_offset(0, row+1);
    } else {
        vidmem[offset] = c;
        vidmem[offset+1] = attr;
        offset += 2;
    }

    /* Check if the offset is over screen size and scroll */
    offset = scroll_screen_buffer(offset);

    set_cursor_offset(offset);
    return offset;
}

int scroll_screen_buffer(int cursor_offset) {
    if (cursor_offset < MAX_ROWS * MAX_COLS * 2) {
        return cursor_offset;
    }

    int i;
    for (i = 1; i < MAX_ROWS; i++) {
        memcpy((uint8_t*)get_offset(0, i-1) + VIDEO_ADDRESS,
                    (uint8_t*)get_offset(0, i) + VIDEO_ADDRESS,
                    MAX_COLS * 2
        );
    }

    /* Blank last line */
    char *last_line = (char*)get_offset(0, MAX_ROWS-1) + VIDEO_ADDRESS;
    for (i = 0; i < MAX_COLS * 2; i++) last_line[i] = 0;

    cursor_offset -= 2 * MAX_COLS;
    return cursor_offset;
}

int get_cursor_offset() {
    /* Use the VGA ports to get the current cursor position
     * 1. Ask for high byte of the cursor offset (data 14)
     * 2. Ask for low byte (data 15)
     */
    port_byte_write(REG_SCREEN_CTRL, 14);
    int offset = port_byte_read(REG_SCREEN_DATA) << 8; /* High byte: << 8 */
    port_byte_write(REG_SCREEN_CTRL, 15);
    offset += port_byte_read(REG_SCREEN_DATA);
    return offset * 2; /* Position * size of character cell */
}

void set_cursor_offset(int offset) {
    /* Similar to get_cursor_offset, but instead of reading we write data */
    offset /= 2;
    port_byte_write(REG_SCREEN_CTRL, 14);
    port_byte_write(REG_SCREEN_DATA, (uint8_t)(offset >> 8));
    port_byte_write(REG_SCREEN_CTRL, 15);
    port_byte_write(REG_SCREEN_DATA, (uint8_t)(offset & 0xff));
}

void clear_screen() {
    int screen_size = MAX_COLS * MAX_ROWS;
    int i;
    char *screen = (char*)VIDEO_ADDRESS;

    for (i = 0; i < screen_size; i++) {
        screen[i*2] = ' ';
        screen[i*2+1] = vga_entry_color(default_fg, default_bg);
    }
    set_cursor_offset(get_offset(0, 0));
}


int get_offset(int col, int row) { return 2 * (row * MAX_COLS + col); }
int get_offset_row(int offset) { return offset / (2 * MAX_COLS); }
int get_offset_col(int offset) { return (offset - (get_offset_row(offset)*2*MAX_COLS))/2; }
