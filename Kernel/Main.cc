#include "Lib/Multiboot.h"
#include "Dev/Video/VGA.h"
#include "Dev/PIC.h"
#include "Dev/PIT.h"
#include "Cpu/Tables/GDT.h"
#include "Cpu/Tables/IDT.h"
#include "Lib/Printf.h"

using namespace Kernel;

__attribute__((section(".multiboot")))
struct multiboot_header_t mboot_header = 
{
    .magic = MULTIBOOT_MAGIC,
    .flags = MULTIBOOT_FLAGS,
    .checksum = static_cast<uint32_t>(-(MULTIBOOT_MAGIC + MULTIBOOT_FLAGS))
};

extern "C" void _main(struct multiboot_info_t *mboot_info, uint32_t mboot_magic) 
{
    vga_clear();
    vga_enable_cursor();
    printf("\n  \033[97mWelcome to \033[94mMachina\033[97m!\033[0m\n\n");
    GDT::gdt_install();
    idt_install();
    PIC::pic_install();
    PIT::pit_install();
}
