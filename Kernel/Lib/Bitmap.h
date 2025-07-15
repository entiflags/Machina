#ifndef __BITMAP_H
#define __BITMAP_H

#include <cstdint>

namespace Kernel 
{
class Bitmap
{
public:
        static inline void bitmap_set(uint8_t* bitmap, uint32_t bit)
        {
                bitmap[bit / 8] |= 1 << (bit % 8);
        }

        static inline void bitmap_clear(uint8_t* bitmap, uint32_t bit)
        {
                bitmap[bit / 8] &= ~(1 << (bit % 8));
        }

        static inline bool bitmap_get(const uint8_t* bitmap, uint32_t bit)
        {
                return bitmap[bit / 8] & (1 << (bit % 8));
        }
};
}

#endif