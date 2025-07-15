#ifndef __PIC_H
#define __PIC_H

#include <cstdint>

namespace Kernel {

class PIC {
public:
    static void install();
    static void disable();
    static void sendEOI(uint8_t irq);

private:
    static constexpr uint16_t PIC1 = 0x20;        // IO base address for master PIC
    static constexpr uint16_t PIC2 = 0xA0;        // IO base address for slave PIC
    static constexpr uint16_t PIC1_CMD = PIC1;
    static constexpr uint16_t PIC1_DAT = PIC1 + 1;
    static constexpr uint16_t PIC2_CMD = PIC2;
    static constexpr uint16_t PIC2_DAT = PIC2 + 1;

    static constexpr uint8_t ICW1_ICW4 = 0x01;    // Indicates that ICW4 will be present
    static constexpr uint8_t ICW1_SINGLE = 0x02;  // Single (cascade) mode
    static constexpr uint8_t ICW1_INTERVAL4 = 0x04; // Call address interval 4 (8)
    static constexpr uint8_t ICW1_LEVEL = 0x08;   // Level triggered (edge) mode
    static constexpr uint8_t ICW1_INIT = 0x10;   // Initialization

    static constexpr uint8_t ICW4_8086 = 0x01;    // 8086/88 (MCS-80/85) mode
    static constexpr uint8_t ICW4_AUTO = 0x02;    // Auto (normal) EOI
    static constexpr uint8_t ICW4_BUF_SLAVE = 0x08;  // Buffered mode/slave
    static constexpr uint8_t ICW4_BUF_MASTER = 0x0C; // Buffered mode/master
    static constexpr uint8_t ICW4_SFNM = 0x10;    // Special fully nested (not)

    static constexpr uint8_t PIC_EOI = 0x20;     // End of interrupt
};

} // namespace Kernel

#endif // __PIC_H