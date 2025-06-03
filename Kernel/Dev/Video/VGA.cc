#include "VGA.h"
#include "../../Cpu/IO.h"
#include "../../Lib/LibCpp.h"

namespace Kernel {

namespace {
    uint8_t vga_x = 0;
    uint8_t vga_y = 0;
    uint8_t vga_color = 0x07;
    uint16_t* vga_buffer = reinterpret_cast<uint16_t*>(0xB8000);

    uint8_t ansi_to_vga(int32_t ansi) {
        static const uint8_t table[] = {
            0x00, 0x04, 0x02, 0x06, 0x01, 0x05, 0x03, 0x07,
            0x08, 0x0C, 0x0A, 0x0E, 0x09, 0x0D, 0x0B, 0x0F
        };

        if (ansi >= 30 && ansi <= 37)
            return table[ansi - 30];
        else if (ansi >= 40 && ansi <= 47)
            return table[ansi - 40];
        else if (ansi >= 90 && ansi <= 97)
            return table[ansi - 82];
        else if (ansi >= 100 && ansi <= 107)
            return table[ansi - 92];
        return 0;
    }
}

void vga_clear() {
    memset(vga_buffer, vga_color << 8, 80 * 25 * sizeof(uint16_t));
    vga_update_cursor();
}

void vga_puts(const char* str) {
    while (*str) {
        if (*str == '\033' && *(str + 1) == '[') {
            str += 2;

            char buf[4] = {0};
            int num = 0;
            while (*str != 'm' && num < 3) {
                buf[num++] = *str++;
            }
            str++;

            int code = atoi(buf);
            if (code == 0) {
                vga_color = 0x07;
            } else if ((code >= 30 && code <= 37) || (code >= 90 && code <= 97)) {
                vga_color = (vga_color & 0xF0) | (ansi_to_vga(code) & 0x0F);
            } else if ((code >= 40 && code <= 47) || (code >= 100 && code <= 107)) {
                vga_color = (vga_color & 0x0F) | (ansi_to_vga(code) & 0xF0);
            }
        } else {
            vga_putchar(*str++);
        }
    }
}

void vga_putchar(char c) {
    switch (c) {
        case '\n':
            vga_x = 80;
            break;
        case '\b':
            if (vga_x == 0) {
                vga_x = 79;
                vga_y--;
            } else {
                vga_x--;
            }
            vga_buffer[vga_y * 80 + vga_x] = vga_color << 8;
            break;
        case '\t':
            vga_puts("    ");
            break;
        default:
            vga_buffer[vga_y * 80 + vga_x] = (vga_color << 8) | static_cast<uint8_t>(c);
            vga_x++;
            break;
    }

    if (vga_x >= 80) {
        vga_x = 0;
        vga_y++;
    }

    if (vga_y >= 25)
        vga_scroll();

    vga_update_cursor();
}

void vga_scroll() {
    memmove(vga_buffer, vga_buffer + 80, 80 * 24 * sizeof(uint16_t));
    memset(vga_buffer + 80 * 24, vga_color << 8, 80 * sizeof(uint16_t));
    vga_y--;
}


void vga_enable_cursor(void) {
    outb(0x3D4, 0x0A);
    outb(0x3D5, (inb(0x3D5) & 0xC0) | 0);
    outb(0x3D4, 0x0B);
    outb(0x3D5, (inb(0x3D5) & 0xE0) | 15);
}

void vga_disable_cursor(void) {
    outb(0x3D4, 0x0A);
    outb(0x3D5, 0x20);
}

void vga_update_cursor(void) {
	uint16_t pos = vga_y * 80 + vga_x;
 
	outb(0x3D4, 0x0F);
	outb(0x3D5, pos & 0xFF);
	outb(0x3D4, 0x0E);
	outb(0x3D5, (pos >> 8) & 0xFF);
}

} // namespace Kernel