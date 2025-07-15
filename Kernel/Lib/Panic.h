#ifndef __PANIC_H
#define __PANIC_H

namespace Kernel
{
[[noreturn]] void panic(const char* file, int line, const char* fmt, ...);
}

#endif