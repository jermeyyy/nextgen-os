/* neXtgen - Screen output */

#ifndef SCRN_H
#define SCRN_H

#include "system.h"

#define BLACK                '0'
#define BLUE                 '1'
#define GREEN                '2'
#define AQUA                 '3'
#define RED                  '4'
#define PURPLE               '5'
#define YELLOW               '6'
#define WHITE                '7'
#define GRAY                 '8'
#define LIGHT_BLUE           '9'
#define LIGHT_GREEN          'A'
#define LIGHT_AQUA           'B'
#define LIGHT_RED            'C'
#define LIGHT_PURPLE         'D'
#define LIGHT_YELLOW         'E'
#define LIGHT_WHITE          'F'

extern void init_video(void);

extern void puts(unsigned char *text);

extern void putch(unsigned char c);

extern void cls(unsigned char ch);

extern void putsnocr(char *text);

extern void putdec(unsigned int value, int digits);

extern int printf(const char *str, ...);

extern int sprintf(char *dest, const char *str, ...);

extern int scrolls;
extern int csr_x, csr_y;
extern char *sprintf_buffer;
#endif
