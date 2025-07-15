#ifndef __PIT_H
#define __PIT_H

#include <cstddef>
#include <cstdint>

namespace Kernel {

class PIT {
public:
    static constexpr uint32_t PIT_BASE_FREQ = 1193180;
    static constexpr uint32_t PIT_FREQ = 1000;

    static size_t pit_ticks;

    static void install();
    static void reinstall();
    static void sleep(size_t ms);

private:
    static void handler(struct registers* regs);
};

} // namespace Kernel

#endif // __PIT_H
