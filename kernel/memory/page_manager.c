#include <kernel/memory.h>
#include <stdint.h>

// 32 bytes = 256 pages monitoring = 1 MB
// 0 = free, 1 = used
static uint8_t page_bitmap[32];

int init_page_manager() {
    // Reserve first 10 pages
    for (int i = 0; i < 10; i++) {
        page_bitmap[i] = 0xFF;
    }

    return 0;
}

/*
** Return page number and set USED flag,
** if FREE page isn't available, -1 is returned
*/
int get_free_page() {
    int i, x;

    for (i = 0; i < sizeof(page_bitmap) ; i++) {
        if (page_bitmap[i] == 0xFF) { continue; }

        for (x = 0 ; x < 8 ; x++) {
            if (!((page_bitmap[i] >> x) & 1)) {
                return ((i * 8) + x);
            }
        }
    }
    return -1;
}

/* Return 0 to FREE, otherwise 1 to USED */
int get_page_status(int pageNumber) {
    if (pageNumber < 8) {
        return ((page_bitmap[0] >> pageNumber) & 1);
    }
    return ((page_bitmap[pageNumber/8] >> pageNumber % 8) & 1);
}

/* Set USED flag on pageNumber, -1 is returned if page is already USED */
int set_page_is_used(int pageNumber) {
    if (get_page_status(pageNumber)) { return -1; }
    page_bitmap[pageNumber/8] |= 1 << (pageNumber % 8);
    return 0;
}

/* Set FREE flag on pageNumber, -1 is returned if page is already FREE */
int set_page_is_free(int pageNumber) {
    if (!get_page_status(pageNumber)) { return -1; }
    page_bitmap[pageNumber/8] &= ~(1 << (pageNumber % 8));
    return 0;
}

/* Return number of pages which is Used */
int how_many_pages_is_used() {
    int i, x, cpt = 0;

    for (i = 0; i < sizeof(page_bitmap) ; i++) {
        for (x = 0 ; x < 8 ; x++) {
            cpt += (page_bitmap[i] >> x) & 1;
        }
    }
    return cpt;
}

/* Return number of pages which is Free */
int how_many_pages_is_free() {
    int i, x, cpt = 0;

    for (i = 0; i < sizeof(page_bitmap) ; i++) {
        for (x = 0 ; x < 8 ; x++) {
            cpt += !((page_bitmap[i] >> x) & 1);
        }
    }
    return cpt;
}
