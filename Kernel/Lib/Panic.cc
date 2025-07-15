#include "Panic.h"
#include <cstdarg>
#include "Printf.h"

namespace Kernel
{
[[noreturn]] void panic(const char* file, int line, const char* fmt, ...)
{
        dprintf("%s:%d: kernel panic: ", file, line);

        va_list args;
        va_start(args, fmt);
        vprintf(fmt, args);
        va_end(args);

        dprintf("\n");

        asm volatile ("cli");
        for (;;)
        {
                asm volatile ("hlt");
        }
}
};