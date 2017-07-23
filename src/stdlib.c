/* neXtgen - stdlib */

#include "system.h"


void *memcpy(void *dest, const void *src, size_t count) {
    const char *sp = (const char *) src;
    char *dp = (char *) dest;
    for (; count != 0; count--) *dp++ = *sp++;
    return dest;
}

inline void *memset(void *s, char c, int count) {
    __asm__ __volatile__ (
    "cld\n\t"
            "rep\n\t"
            "stosb"::"a" (c), "D" (s), "c" (count));
    return s;
}

void *memset_old(void *dest, char val, size_t count) {
    char *temp = (char *) dest;
    for (; count != 0; count--) *temp++ = val;
    return dest;
}

unsigned short *memsetw(unsigned short *dest, unsigned short val, size_t count) {
    unsigned short *temp = (unsigned short *) dest;
    for (; count != 0; count--) *temp++ = val;
    return dest;
}

size_t strlen(const char *str) {
    size_t retval;
    for (retval = 0; *str != '\0'; str++) retval++;
    return retval;
}

char *strcpy(char *where, char *what) {
    while ((*where++ = *what++) != '\0')
        return what;
}

int strcmp(const char *s1, const char *s2) {
    for (; *s1 == *s2; ++s1, ++s2)
        if (*s1 == 0)
            return 0;
    return *(unsigned char *) s1 < *(unsigned char *) s2 ? -1 : 1;
}

char *strcat(char *dest, const char *src) {
    for (; *dest != '\0'; dest++);

    while (*src != '\0') {
        *(dest++) = *(src++);
        *dest = '\0';
    }
    return dest;
}

unsigned char inportb(unsigned short _port) {
    unsigned char rv;
    __asm__ __volatile__ ("inb %1, %0" : "=a" (rv) : "dN" (_port));
    return rv;
}

void outportb(unsigned short _port, unsigned char _data) {
    __asm__ __volatile__ ("outb %1, %0" : : "dN" (_port), "a" (_data));
}

void panic_assert(const char *file, unsigned int line, const char *desc) {
    __asm__ __volatile__("cli"); // Disable interrupts.
    settextcolor(RED, BLACK);
    puts("Assertion-Failed(");
    puts(desc);
    puts(") at ");
    puts(file);
    puts(":");
    putdec(line, 1);
    puts("\n");
    while (1);
}
