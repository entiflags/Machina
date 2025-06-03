#ifndef __VGA_H
#define __VGA_H

#include <cstdint>

namespace Kernel {

void vga_clear();
void vga_puts(const char* str);
void vga_putchar(char c);
void vga_scroll();
void vga_enable_cursor();
void vga_disable_cursor();
void vga_update_cursor();

} // namespace Kernel

#endif