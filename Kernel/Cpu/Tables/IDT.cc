#include "IDT.h"
#include "../../Lib/LibCpp.h"
#include "../../Lib/Printf.h"

namespace Kernel {

struct idt_entry idt_entries[256];
struct idtr idt_descriptor;
extern "C" void* idt_int_table[];

void* irq_handlers[256];

const char* isr_errors[32] = {
    "division by zero",
    "debug",
    "non-maskable interrupt",
    "breakpoint",
    "detected overflow",
    "out-of-bounds",
    "invalid opcode",
    "no coprocessor",
    "double fault",
    "coprocessor segment overrun",
    "bad TSS",
    "segment not present",
    "stack fault",
    "general protection fault",
    "page fault",
    "unknown interrupt",
    "coprocessor fault",
    "alignment check",
    "machine check",
    "reserved",
    "reserved",
    "reserved",
    "reserved",
    "reserved",
    "reserved",
    "reserved",
    "reserved",
    "reserved",
    "reserved",
    "reserved",
    "reserved",
    "reserved"
};

extern "C" void idt_install() {
    for (uint16_t i = 0; i < 256; i++) {
        idt_set_entry(i, (uint32_t)idt_int_table[i], 0x08, 0x8E);
    }

    idt_descriptor.size = sizeof(struct idt_entry) * 256 - 1;
    idt_descriptor.offset = (uint32_t)idt_entries;

    asm volatile ("lidt %0" :: "m"(idt_descriptor));
    printf(" \033[92m[OK]\033[97m Initialized IDT at address 0x%x\033[0m\n", (uint32_t)&idt_descriptor);
}

extern "C" void idt_set_entry(uint8_t index, uint32_t base, uint16_t selector, uint8_t type) {
    idt_entries[index].base_low = base & 0xFFFF;
    idt_entries[index].selector = selector;
    idt_entries[index].zero = 0x00;
    idt_entries[index].type = type;
    idt_entries[index].base_high = (base >> 16) & 0xFFFF;
}

extern "C" void irq_register(uint8_t vector, void* handler) {
    irq_handlers[vector] = handler;
}

extern "C" void irq_unregister(uint8_t vector) {
    irq_handlers[vector] = nullptr;
}

extern "C" void isr_handler(struct registers* regs) {
    if (regs->int_no == 0xff) {
        return;
    }

    dprintf("%s:%d: %s\n", __FILE__, __LINE__, isr_errors[regs->int_no]);

    if (regs->int_no == 0x0e) {
        uint32_t cr2;
        asm volatile("mov %%cr2, %0" : "=r" (cr2));
        dprintf("%s:%d: faulting address: 0x%x\n", __FILE__, __LINE__, cr2);
    }

    asm volatile ("cli");
    for (;;) asm volatile ("hlt");
}

extern "C" void irq_handler(struct registers* regs) {
    void (*handler)(struct registers*);
    handler = (void (*)(struct registers*))irq_handlers[regs->int_no - 32];

    if (handler != nullptr) {
        handler(regs);
    }
}

} // namespace Kernel