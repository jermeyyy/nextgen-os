/* neXtgen - stdlib */
#ifndef STDLIB_H
#define STDLIB_H

#define ASSERT(b) ((b) ? (void)0 : panic_assert(__FILE__, __LINE__, #b))

typedef int size_t;

extern void *memcpy(void *dest, const void *src, size_t count);

extern void *memset(void *dest, char val, size_t count);

extern unsigned short *memsetw(unsigned short *dest, unsigned short val, size_t count);

extern size_t strlen(const char *str);

extern char *strcpy(char *where, char *what);

extern int strcmp(const char *s1, const char *s2);

extern unsigned char inportb(unsigned short _port);

extern void outportb(unsigned short _port, unsigned char _data);

extern char *strcat(char *s1, const char *s2);

extern void panic_assert(const char *file, unsigned int line, const char *desc);

#endif
