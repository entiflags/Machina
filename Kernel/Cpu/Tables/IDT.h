#ifndef __IDT_H
#define __IDT_H

#include <cstdint>

namespace Kernel {

struct idt_entry {
    uint16_t base_low;
    uint16_t selector;
    uint8_t  zero;
    uint8_t  type;
    uint16_t base_high;
} __attribute__((packed));

struct idtr {
    uint16_t size;
    uint32_t offset;
} __attribute__((packed));

struct registers {
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t esp;
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;
    uint32_t int_no;
    uint32_t err_code;
    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;
    uint32_t user_esp;
    uint32_t ss;
} __attribute__((packed));

extern "C" void idt_install();
extern "C" void idt_set_entry(uint8_t index, uint32_t base, uint16_t selector, uint8_t type);
extern "C" void irq_register(uint8_t vector, void* handler);
extern "C" void irq_unregister(uint8_t vector);

} // namespace Kernel

#endif