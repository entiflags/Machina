#ifndef GDT_H
#define GDT_H

#include <cstdint>

class GDT {
public:
    struct Entry {
        uint16_t limit;
        uint16_t base_low;
        uint8_t  base_mid;
        uint8_t  access;
        uint8_t  gran;
        uint8_t  base_high;
    } __attribute__((packed));

    struct Descriptor {
        uint16_t size;
        uint32_t offset;
    } __attribute__((packed));

    static void install();
    static void setEntry(uint8_t index, uint16_t limit, uint32_t base, uint8_t access, uint8_t gran);

private:
    static inline Entry entries[3];
};

extern "C" GDT::Descriptor gdt_descriptor;

#endif