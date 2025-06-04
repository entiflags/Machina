#ifndef __PRINTF_H
#define __PRINTF_H

#include <cstdarg>
#include <cstdint>

namespace Kernel {

int sprintf(char* s, const char* fmt, std::va_list args);
int vprintf(const char* fmt, std::va_list args);
int dprintf(const char *fmt, ...);
int printf(const char* fmt, ...);

} // namespace Kernel

#endif