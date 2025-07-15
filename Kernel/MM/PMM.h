#ifndef __PMM_H
#define __PMM_H

#include <cstddef>
#include <cstdint>
#include "../Lib/Multiboot.h"

namespace Kernel
{
constexpr size_t PAGE_SIZE = 4096;

constexpr size_t DIV_CEILING(size_t x, size_t y) { return (x + (y-1)) / y; }
constexpr size_t ALIGN_UP(size_t x, size_t y) { return DIV_CEILING(x, y) * y; }

class PMM
{
public:
        static void install(multiboot_info_t& mbd);
        static void* alloc(size_t page_count);
        static void free(void *ptr, size_t page_count);

private:
        static uint8_t* bitmap;
        static uint32_t last_page;
        static uint32_t used_pages;
        static uint32_t page_count;
        static uint32_t usable_mem;
        static uint32_t bitmap_size;

        static uint32_t find_pages(uint32_t page_count);
};
}

#endif