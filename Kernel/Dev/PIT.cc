#include "PIT.h"
#include "../Cpu/Tables/IDT.h"
#include "../Cpu/IO.h"
#include "../Lib/Printf.h"

namespace Kernel {

size_t PIT::pit_ticks = 0;

void PIT::handler(struct registers* regs) {
    pit_ticks++;
}

void PIT::pit_install() {
    uint16_t divisor = static_cast<uint16_t>(PIT_BASE_FREQ / PIT_FREQ);

    outb(0x43, 0x36);
    outb(0x40, static_cast<uint8_t>(divisor));
    outb(0x40, static_cast<uint8_t>(divisor >> 8));

    irq_register(0, reinterpret_cast<void*>(handler));
    printf(" \033[92m[OK]\033[97m Initialized timer at %dkhz\033[0m\n", PIT_FREQ / 1000);
}

void PIT::pit_reinstall()
{
    irq_register(0, reinterpret_cast<void*>(handler));
}

void PIT::pit_sleep(size_t ms) {
    size_t start_ticks = pit_ticks;
    size_t end_ticks = start_ticks + ms;

    while (pit_ticks < end_ticks) {
        asm volatile("hlt");
    }
}

} // namespace Kernel
