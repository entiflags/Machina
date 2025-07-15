#include "PMM.h"
#include "../Lib/LibCpp.h"
#include "../Lib/Panic.h"
#include "../Lib/Printf.h"
#include "../Lib/Bitmap.h"
#include "../Lib/Multiboot.h"

extern "C" void *end;

namespace Kernel {

uint8_t* PMM::bitmap = nullptr;
uint32_t PMM::last_page = 0;
uint32_t PMM::used_pages = 0;
uint32_t PMM::page_count = 0;
uint32_t PMM::usable_mem = 0;
uint32_t PMM::bitmap_size = 0;

void PMM::install(multiboot_info_t& mbd) {
        if (!(mbd.flags >> 6 & 0x1))
                panic(__FILE__, __LINE__, "invalid multiboot memory map");

        uint32_t higher_address = 0;
        uint32_t top_address = 0;
        multiboot_memory_map_t* kernel_mmmt = nullptr;

        for (uint32_t i = 0; i < mbd.mmap_length; i += sizeof(multiboot_memory_map_t)) {
                auto* mmmt = reinterpret_cast<multiboot_memory_map_t*>(mbd.mmap_addr + i);

                if (mmmt->type == MULTIBOOT_MEMORY_AVAILABLE) {
                        if (mmmt->len_low > usable_mem) {
                                kernel_mmmt = mmmt;
                                usable_mem = mmmt->len_low;
                        }

                        top_address = mmmt->addr_low + mmmt->len_low;
                        if (top_address > higher_address)
                                higher_address = top_address;
                }
        }

        bitmap = reinterpret_cast<uint8_t*>(&end);
        page_count = higher_address / PAGE_SIZE;
        bitmap_size = ALIGN_UP(page_count / 8, PAGE_SIZE);
        memset(bitmap, 0xff, bitmap_size);

        kernel_mmmt->len_low -= reinterpret_cast<uint32_t>(bitmap) - kernel_mmmt->addr_low;
        kernel_mmmt->addr_low = reinterpret_cast<uint32_t>(bitmap) + bitmap_size;

        for (uint32_t i = 0; i < mbd.mmap_length; i += sizeof(multiboot_memory_map_t)) {
                auto* mmmt = reinterpret_cast<multiboot_memory_map_t*>(mbd.mmap_addr + i);

                for (uint32_t o = 0; o < mmmt->len_low; o += PAGE_SIZE) {
                        if (mmmt->type == MULTIBOOT_MEMORY_AVAILABLE && mmmt->addr_low >= 0x100000) {
                                Bitmap::bitmap_clear(bitmap, (mmmt->addr_low + o) / PAGE_SIZE);
                        }
                }
        }

        printf(" \033[92m[OK]\033[97m Initialized bitmap allocator, %dM usable \033[0m\n", page_count / 256);
}

uint32_t PMM::find_pages(uint32_t page_count) {
        uint32_t pages = 0;
        uint32_t first_page = last_page;

        while (pages < page_count) {
                if (last_page == PMM::page_count)
                        return 0; /* out of memory */

                if (!Bitmap::bitmap_get(bitmap, last_page + pages)) {
                        pages++;
                        if (pages == page_count) {
                                for (uint32_t i = 0; i < pages; i++)
                                        Bitmap::bitmap_set(bitmap, first_page + i);
                                last_page += pages;
                                return first_page;
                        }
                } else {
                        last_page += !pages ? 1 : pages;
                        first_page = last_page;
                        pages = 0;
                }
        }
        return 0;
}

void* PMM::alloc(size_t page_count) {
        uint32_t pages = find_pages(static_cast<uint32_t>(page_count));

        if (!pages) {
                last_page = 0;
                pages = find_pages(static_cast<uint32_t>(page_count));

                if (!pages) {
                        dprintf("%s:%d: \033[33mwarning:\033[0m out of memory\n", __FILE__, __LINE__);
                        return nullptr;
                }
        }

        return reinterpret_cast<void*>(pages * PAGE_SIZE);
}

void PMM::free(void* ptr, size_t page_count) {
        uint32_t page = reinterpret_cast<uint32_t>(ptr) / PAGE_SIZE;

        for (uint32_t i = 0; i < page_count; i++)
                Bitmap::bitmap_clear(bitmap, page + i);
}

}