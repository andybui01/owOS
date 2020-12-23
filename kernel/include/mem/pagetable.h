#ifndef _MEM_PAGETABLE_H
#define _MEM_PAGETABLE_H

#include <stdint.h>
#include <stdbool.h>

struct page_directory_entry {
    union {
        uint32_t entry;
        struct {
            uint32_t present :1;
            uint32_t writable :1;
            uint32_t user :1;
            uint32_t write_through :1;
            uint32_t cache_disable :1;
            uint32_t accessed :1;
            uint32_t zero :1;
            uint32_t page_size :1;
            uint32_t global :1;
            uint32_t reserved :3;
            uint32_t addr :20;
        };
    };
};

struct page_table_entry {
    union {
        uint32_t entry;
        struct {
            uint32_t present :1;
            uint32_t writable :1;
            uint32_t user :1;
            uint32_t write_through :1;
            uint32_t cache_disable :1;
            uint32_t accessed :1;
            uint32_t dirty :1;
            uint32_t pat_bit_3 :1;
            uint32_t global :1;
            uint32_t reserved :3;
            uint32_t addr :20;
        };
    };
};

struct page_directory {
    struct page_directory_entry entries[1024];
};

struct page_table {
    struct page_table_entry entries[1024];
};

typedef struct page_directory pgd_t;
typedef struct page_table pgt_t;

typedef struct page_directory_entry pde_t;
typedef struct page_table_entry pte_t;

#endif
