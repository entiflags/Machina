#include "../../Cpu/IO.h"

namespace Kernel
{

void serial_puts(char *str)
{
    while (*str)
        outb(0xe9, *str++);
}

}