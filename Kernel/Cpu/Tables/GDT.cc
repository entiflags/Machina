#include "GDT.h"
#include "../../Lib/Printf.h"

using namespace Kernel;

extern "C" void gdt_flush(void);
GDT::Descriptor gdt_descriptor;

void GDT::gdt_install() {
    setEntry(0, 0x0000, 0x00000000, 0b00000000, 0b00000000);
    setEntry(1, 0xFFFF, 0x00000000, 0b10011011, 0b11001111);
    setEntry(2, 0xFFFF, 0x00000000, 0b10010011, 0b11001111);

    gdt_descriptor = Descriptor{
        .size = sizeof(Entry) * 3 - 1,
        .offset = reinterpret_cast<uint32_t>(entries)
    };

    gdt_flush();
    printf(" \033[92m[OK]\033[97m Initialized GDT at address 0x%x\033[0m\n", (uint32_t)&gdt_descriptor);
}

void GDT::setEntry(uint8_t index, uint16_t limit, uint32_t base, uint8_t access, uint8_t gran) {
    entries[index].limit = limit;
    entries[index].base_low = base & 0xFFFF;
    entries[index].base_mid = (base >> 16) & 0xFF;
    entries[index].access = access;
    entries[index].gran = gran;
    entries[index].base_high = (base >> 24) & 0xFF;
}