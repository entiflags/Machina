#ifndef __LIBCPP_H
#define __LIBCPP_H

#include <cstdint>
#include <cstddef>

namespace Kernel {
extern "C" {
    void *memcpy(void* dest, const void* src, std::size_t n);
    void *memset(void *s, int c, size_t n);
    void *memmove(void* dest, const void* src, std::size_t n);
    int memcmp(const void* s1, const void* s2, std::size_t n);
    std::size_t strlen(const char* s);
    int strcmp(const char* a, const char* b);
    int strncmp(const char* x, const char* y, std::size_t n);
    char* strcpy(char* dest, const char* src);
    const char* strchr(const char* s, int c);
    const char* strstr(const char* haystack, const char* needle);
    int atoi(const char* s);
}
} // namespace Kernel

#endif