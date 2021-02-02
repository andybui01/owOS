#include <mem/heap.h>
#include <mem/mem.h>
#include <mem/memlayout.h>
#include <mem/paging.h>

#include <types.h>
#include <stdio.h>

extern uint32_t _kernel_end;
static vaddr_t kend_addr = &_kernel_end;

static heap_t heap;

void heap_bootstrap(void)
{
    heap.curr = PG_ALIGN(kend_addr);
    heap.end = (vaddr_t) KHEAP_END;
    heap.last_page = heap.curr;
}

void *kmalloc(uint32_t size)
{
    void *tmp = heap.curr;
    heap.curr += size;

    // if heap.curr past page boundary, allocate page
    while (heap.curr >= heap.last_page + PAGE_SIZE) {
        heap.last_page += PAGE_SIZE;
        map_page(heap.last_page);
    }

    return tmp;
}

void kfree(void *ptr)
{
    // free does nothing in bump allocator
    (void) ptr;
}
