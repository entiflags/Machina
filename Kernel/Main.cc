#include "Lib/Multiboot.h"

__attribute__((section(".multiboot")))
struct multiboot_header_t mboot_header = 
{
    .magic = MULTIBOOT_MAGIC,
    .flags = MULTIBOOT_FLAGS,
    .checksum = static_cast<uint32_t>(-(MULTIBOOT_MAGIC + MULTIBOOT_FLAGS))
};

extern "C" void _main(struct multiboot_info_t *mboot_info, uint32_t mboot_magic) {}