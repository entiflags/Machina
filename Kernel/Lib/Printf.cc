#include "Printf.h"
#include <cstdarg>
#include <cstdint>
#include "../Cpu/IO.h"
#include "../Dev/Video/VGA.h"
#include "../Dev/Char/Serial.h"
#include "LibCpp.h"

namespace Kernel {

void parse_num(char* s, int& ptr, uint32_t val, uint32_t base, int width, bool zero_padding) {
    char buf[32];
    int i = 0;

    if (val == 0) {
        while (width-- > 1) s[ptr++] = zero_padding ? '0' : ' ';
        s[ptr++] = '0';
        return;
    }

    while (val) {
        buf[i++] = (val % base) < 10 ? (val % base) + '0' : (val % base) - 10 + 'A';
        val /= base;
    }

    int padding = width - i;
    while (padding-- > 0) s[ptr++] = zero_padding ? '0' : ' ';

    while (i > 0) s[ptr++] = buf[--i];
}

void parse_hex(char* s, int& ptr, uint32_t val) {
    int i = 8;
    while (i-- > 0) {
        s[ptr++] = "0123456789abcdef"[val >> (i * 4) & 0x0F];
    }
}

void parse_string(char* s, int& ptr, const char* str) {
    if (!str) {
        memcpy(s + ptr, "(null)", 6);
        ptr += 6;
        return;
    }

    while (*str) {
        s[ptr++] = *str++;
    }
}

int sprintf(char* s, const char* fmt, std::va_list args) {
    int ptr = 0;

    while (*fmt) {
        if (*fmt == '%') {
            fmt++;
            int width = 0;
            bool zero_padding = false;

            if (*fmt == '0') {
                zero_padding = true;
                fmt++;
            }

            while (*fmt >= '0' && *fmt <= '9') {
                width = width * 10 + (*fmt - '0');
                fmt++;
            }

            switch (*fmt) {
                case 'd':
                    parse_num(s, ptr, va_arg(args, int), 10, width, zero_padding);
                    break;
                case 'x':
                    parse_hex(s, ptr, va_arg(args, uint32_t));
                    break;
                case 's':
                    parse_string(s, ptr, va_arg(args, const char*));
                    break;
                case 'c':
                    s[ptr++] = static_cast<char>(va_arg(args, int));
                    break;
            }
        } else {
            s[ptr++] = *fmt;
        }
        fmt++;
    }
    s[ptr] = '\0';
    return ptr;
}

int vprintf(const char* fmt, std::va_list args) {
    char buf[1024] = {0};
    int ret = sprintf(buf, fmt, args);
    
    vga_puts(buf);
    return ret;
}

int printf(const char* fmt, ...) {
    std::va_list args;
    va_start(args, fmt);

    char buf[1024] = {0};
    int ret = sprintf(buf, fmt, args);

    vga_puts(buf);

    va_end(args);
    return ret;
}

int dprintf(const char* fmt, ...) {
    std::va_list args;
    va_start(args, fmt);
    char buf[1024] = {-1};
    int ret = sprintf(buf, fmt, args);

    serial_puts(buf);

    va_end(args);
    return ret;
}

} // namespace Kernel