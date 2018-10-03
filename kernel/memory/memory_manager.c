#include <kiteos/memory.h>
#include <sys/function.h>

void *mem_req_pages(int pages) {
    UNUSED(pages); // TODO: Allocate multiple consecutive pages
    int page = get_free_page();
    if (page == -1) {
        return NULL;
    }
    set_page_is_used(page);
    return (void*)PAGE_TO_ADDR(page);
}

void mem_free_page(void *addr) {
    int page = ADDR_TO_PAGE(addr);
    set_page_is_free(page);
}
