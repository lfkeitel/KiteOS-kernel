#include <kiteos/memory.h>
#include <sys/function.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int alloc_new_page();

typedef struct header_t header_t;
struct header_t {
    size_t length;
    uint8_t used;
    header_t *next;
    header_t *last;
} __attribute((packed));

static header_t *start_header;
static header_t *end_header;

/*
    The heap is header based. Each slice contains a header indicating the sector, size, and next and last headers.
    The headers are doubly-linked so the list can be traverrsed in either direction.
    The available memory begins directly after the header and is length bytes long.
    Large chuncks of memory can be divided to provide more granular allocations. A new header will be made
    for the remaining space and the links updated accordingly.
*/

int init_kern_mem() {
    void *init_page = mem_req_pages(1);
    if (init_page == NULL) {
        return -1;
    }

    header_t *header = (header_t*) init_page;
    header->length = PAGE_SIZE - sizeof(header_t);
    header->used = 0;
    header->next = NULL;
    header->last = NULL;

    start_header = end_header = header;
    return 0;
}

void* kmalloc(size_t size) {
    if (size > (PAGE_SIZE-sizeof(header_t))) {
        puts("Attempted to allocate more than a page\n");
        abort();
    }

    header_t *header = start_header;

try_again:
    while (header != NULL) {
        if (header->used == 1 || header->length < size) { // Segment is too small, go to the next one
            header = header->next;
            continue;
        }

        header->used = 1;

        size_t old_length = header->length;
        if (old_length - size < sizeof(header_t)) {
            // Not enough space in this segment to have another header
            // Just allocate the whole segment
            return (void*)((char*)header) + sizeof(header_t);
        }

        // There's enough space to make a new header so let's split this segment
        header->length = size;

        // Type cast for byte-precision pointer arithmetic
        char *mem = (char*)header;

        // Create new header after allocated space
        header_t *new_header = (header_t*)(mem + sizeof(header_t) + size);
        new_header->length = old_length - sizeof(header_t);
        new_header->used = 0;
        new_header->next = header->next;
        new_header->last = header;

        // Update existing header with new header as next
        header->next = new_header;

        if (header == end_header) {
            end_header = new_header;
        }

        return (void*)(mem + sizeof(header_t));
    }

    if (!alloc_new_page(size)) {
        return NULL;
    }

    if (end_header->length < size) { // TODO: Need to handle sizes larger than a page
        return NULL;
    }

    header = end_header;
    goto try_again;
    return NULL;
}

int round_up(size_t num, size_t multiple) {
    return (num + multiple - 1) & -multiple;
}

int alloc_new_page(size_t size) {
    size += sizeof(header_t); // Any new memory needs to account for at least one header

    int num_pages = 1;
    // if (size > PAGE_SIZE) { // Need to determine how many pages we need
    //     num_pages = round_up(size, PAGE_SIZE) / PAGE_SIZE;
    // }

    void *init_page = mem_req_pages(num_pages);
    if (init_page == NULL) {
        return -1;
    }

    header_t *header = (header_t*) init_page;
    header->length = (num_pages * PAGE_SIZE) - sizeof(header_t);
    header->used = 0;
    header->next = NULL;

    header->last = end_header;
    end_header->next = header;
    end_header = header;
    return 0;
}

void kfree(void *ptr) {
    header_t *header = ptr - sizeof(header_t);
    header->used = 0;

    if (rand() % 100 <= 10) {
        cleanup_heap();
    }
}

void cleanup_heap() {
    // TODO: Consolidate consecutive memory regions
    // Be cautious of non-consecutive pages. Headers can only span consecutive
    // pages otherwise memory will not be in our page space.
    // Also, should order headers unless this is done when a new page is allocated.
}
