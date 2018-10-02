#ifndef MEM_H
#define MEM_H

#include <stddef.h>

#define PAGE_SIZE 4096

#define PAGE_TO_ADDR(X) ((size_t)(X) * (PAGE_SIZE))
#define ADDR_TO_PAGE(X) ((size_t)(X) / (PAGE_SIZE))

// Aligns the supplied size to the specified PowerOfTwo
#define ALIGN_SIZE( sizeToAlign, PowerOfTwo )       \
        (((sizeToAlign) + (PowerOfTwo) - 1) & ~((PowerOfTwo) - 1))

// Checks whether the supplied size is aligned to the specified PowerOfTwo
#define IS_SIZE_ALIGNED( sizeToTest, PowerOfTwo )  \
        (((sizeToTest) & ((PowerOfTwo) - 1)) == 0)


void* kmalloc(size_t size);

void* mem_req_pages(int pages);
void mem_free_page(void *addr);

int get_free_page();
int set_page_is_used(int pageNumber);
int set_page_is_free(int pageNumber);

int init_page_manager();
int init_kern_mem();

#endif
