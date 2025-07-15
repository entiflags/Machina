#include "../Cpu/IO.h"
#include "../Lib/Printf.h"
#include "PIC.h"

namespace Kernel {

void PIC::install() {
    outb(PIC1_CMD, ICW1_INIT | ICW1_ICW4);
    outb(PIC2_CMD, ICW1_INIT | ICW1_ICW4);
    outb(PIC1_DAT, 0x20);
    outb(PIC2_DAT, 0x28);
    outb(PIC1_DAT, 0x04);
    outb(PIC2_DAT, 0x02);
    outb(PIC1_DAT, ICW4_8086);
    outb(PIC2_DAT, ICW4_8086);
    outb(PIC1_DAT, 0x00);
    outb(PIC2_DAT, 0x00);
    asm volatile ("sti");
    printf(" \033[92m[OK]\033[97m Initialized interrupts\033[0m\n");
}

void PIC::disable() {
    outb(PIC1_DAT, 0xFF);
    outb(PIC2_DAT, 0xFF);
    outb(PIC1_DAT, PIC_EOI);
    outb(PIC2_DAT, PIC_EOI);
    dprintf("%s:%d: disabled 8259 PIC\n", __FILE__, __LINE__);
}

void PIC::sendEOI(uint8_t irq) {
    if (irq >= 8)
        outb(PIC2_CMD, PIC_EOI);
    outb(PIC1_CMD, PIC_EOI);
}

}