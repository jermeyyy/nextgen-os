/* neXtgen - Screen output support */

#include "system.h"
#include "va_list.h"

#define VGA_CRTC_INDEX       0x3D4
#define VGA_CRTC_DATA        0x3D5

int scrolls = 1;

char *sprintf_buffer;
/* These define our textpointer, our background and foreground
*  colors (attributes), and x and y cursor coordinates */
unsigned short *textmemptr;
int attrib = 0x0F;
int csr_x = 0, csr_y = 0;
unsigned short *screen = (unsigned short *) 0xB8000;
char tbuf[32];
char bchars[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

/* Scrolls the screen */
void scroll(void) {
    if (scrolls) // our hack from WinMngr
    {
        unsigned blank, temp;

        /* A blank is defined as a space... we need to give it
        *  backcolor too */
        blank = 0x20 | (attrib << 8);

        /* Row 25 is the end, this means we need to scroll up */
        if (csr_y >= 25) {
            /* Move the current text chunk that makes up the screen
            *  back in the buffer by a line */
            temp = csr_y - 25 + 1;
            memcpy(textmemptr, textmemptr + temp * 80, (25 - temp) * 80 * 2);

            /* Finally, we set the chunk of memory that occupies
            *  the last line of text to our 'blank' character */
            memsetw(textmemptr + (25 - temp) * 80, blank, 80);
            csr_y = 25 - 1;
        }
    }
}

/* Updates the hardware cursor: the little blinking line
*  on the screen under the last character pressed! */
void move_csr(void) {
    unsigned temp;

    temp = csr_y * 80 + csr_x;

    outportb(0x3D4, 14);
    outportb(0x3D5, temp >> 8);
    outportb(0x3D4, 15);
    outportb(0x3D5, temp);
}


void showcursor() {
    outportb(VGA_CRTC_INDEX, 10);
    outportb(VGA_CRTC_DATA, (inportb(VGA_CRTC_DATA) & ~0x20));
}

void hidecursor() {
    outportb(VGA_CRTC_INDEX, 10);
    outportb(VGA_CRTC_DATA, (inportb(VGA_CRTC_DATA) | 0x20));
}


/* Puts a single character on the screen */
void putch(unsigned char c) {
    unsigned short *where;
    unsigned att = attrib << 8;

    /* Handle a backspace, by moving the cursor back one space */
    if (c == 0x08) {
        if (csr_x != 0) csr_x--;
    }
        /* Handles a tab by incrementing the cursor's x, but only
            to a point that will make it divisible by 8 */
    else if (c == 0x09) {
        csr_x = (csr_x + 8) & ~(8 - 1);
    }
        /* Handles a 'Carriage Return', which simply brings the
            cursor back to the margin */
    else if (c == '\r') {
        csr_x = 0;
    }
        /* We handle our newlines the way DOS and the BIOS do: we
            treat it as if a 'CR' was also there, so we bring the
            cursor to the margin and we increment the 'y' value */
    else if (c == '\n') {
        csr_x = 0;
        csr_y++;
    }
        /* Any character greater than and including a space, is a
            printable character. The equation for finding the index
            in a linear chunk of memory can be represented by:
           Index = [(y * width) + x] */
    else if (c >= ' ') {
        where = textmemptr + (csr_y * 80 + csr_x);
        *where = c | att;    /* Character AND attributes: color */
        //Screen.Buffer[csr_y][csr_x].val=c;
        //Screen.Buffer[csr_y][csr_x].Modified=1;
        //Screen.Modified=1;
        csr_x++;
    }

    /* If the cursor has reached the edge of the screen's width, we
    *  insert a new line in there */
    if (csr_x >= 80) {
        csr_x = 0;
        csr_y++;
    }

    /* Scroll the screen if needed, and finally move the cursor */
    scroll();
    move_csr();
}

/* Uses the above routine to output a string... */
void puts(unsigned char *text) {
    for (int i = 0; i < strlen(text); i++) {
        putch(text[i]);
    }
}

/* Sets the forecolor and backcolor that we will use */
void settextcolor(unsigned char forecolor, unsigned char backcolor) {
    /* Top 4 bytes are the background, bottom 4 bytes
        are the foreground color */
    attrib = (backcolor << 4) | (forecolor & 0x0F);
}

void updatecursor() {
    unsigned int index = (csr_y * 80) + csr_x;
    outportb(VGA_CRTC_INDEX, 14);
    outportb(VGA_CRTC_DATA, (index >> 8));
    outportb(VGA_CRTC_INDEX, 15);
    outportb(VGA_CRTC_DATA, (index & 0xFF));
}

void gotoxy(int x, int y) {
    csr_x = x;
    csr_y = y;
    updatecursor();
}

/* Sets our text-mode VGA pointer, then clears the screen for us */
void Video_Init(void) {
    textmemptr = (unsigned short *) 0xB8000;
    cls(' ');
    gotoxy(0, 2);
    settextcolor(GREEN, BLACK);
    printf("[OK]  ");
    settextcolor(WHITE, BLACK);
    printf("Screen");
    settextcolor(BLUE, BLACK);
    printf("  text mode 80x25 chars\n");
}

void cls(unsigned char chr) {
    unsigned short attr = 0x0F00;
    memsetw(screen, (attr | chr), 80 * 25);
    csr_x = 0;
    csr_y = 0;
    updatecursor();
}


void putdec(unsigned int value, int digits) {
    char base[10];
    int i = 0;
    memset_old(base, '0', 10);
    while (value > 0) {
        base[i++] = '0' + (value % 10);
        value /= 10;
    }
    if (i < digits) i = digits;
    while (i-- > 0)
        putch(base[i]);
}

void putsnocr(char *text) {
    while (*text)
        putch(*text++);
}

void itoa(unsigned i, unsigned base, char *buf) {
    int pos = 0;
    int opos = 0;
    int top = 0;

    if (i == 0 || base > 16) {
        buf[0] = '0';
        buf[1] = '\0';
        return;
    }

    while (i != 0) {
        tbuf[pos] = bchars[i % base];
        pos++;
        i /= base;
    }
    top = pos--;
    for (opos = 0; opos < top; pos--, opos++) {
        buf[opos] = tbuf[pos];
    }
    buf[opos] = 0;
}

void itoa_s(int i, unsigned base, char *buf) {
    if (base > 16) return;
    if (i < 0) {
        *buf++ = '-';
        i *= -1;
    }
    itoa(i, base, buf);
}

int printf(const char *str, ...) {

    if (!str)
        return 0;

    va_list args;
    va_start (args, str);
    size_t i;
    for (i = 0; i < strlen(str); i++) {

        switch (str[i]) {

            case '%':

                switch (str[i + 1]) {

                    /* characters  */
                    case 'c': {
                        char c = va_arg (args, char);
                        putch(c);
                        i++;        // go to next character
                        break;
                    }

                        /* address of  */
                    case 's': {
                        int c = (int) va_arg (args, char);
                        char *str;
                        strcpy(str, (const char *) c);
                        puts(str);
                        i++;        // go to next character
                        break;
                    }

                        /* integers  */
                    case 'd':
                    case 'i': {
                        int c = va_arg (args, int);
                        char str[32] = {0};
                        itoa_s(c, 10, str);
                        puts(str);
                        i++;        // go to next character
                        break;
                    }

                        /* display in hex  */
                    case 'X':
                    case 'x': {
                        int c = va_arg (args, int);
                        char str[32] = {0};
                        itoa_s(c, 16, str);
                        puts(str);
                        i++;        // go to next character
                        break;
                    }

                    default:
                        va_end (args);
                        return 1;
                }

                break;

            default:
                putch(str[i]);
                break;
        }

    }

    va_end (args);
    return i;
}

int sprintf(char *dest, const char *str, ...) {

    if (!str)
        return 0;

    va_list args;
    va_start (args, str);
    size_t i;
    for (i = 0; i < strlen(str); i++) {

        switch (str[i]) {

            case '%':

                switch (str[i + 1]) {

                    /* characters  */
                    case 'c': {
                        char c = va_arg (args, char);
                        *sprintf_buffer += c;//putch (c);
                        i++;        // go to next character
                        break;
                    }

                        /*  address of  */
                    case 's': {
                        int c = (int) va_arg (args, char);
                        char *str;
                        strcpy(str, (const char *) c);
                        strcpy(sprintf_buffer, str);//puts (str);
                        i++;        // go to next character
                        break;
                    }

                        /*  integers  */
                    case 'd':
                    case 'i': {
                        int c = va_arg (args, int);
                        char str[32] = {0};
                        itoa_s(c, 10, str);
                        strcpy(sprintf_buffer, str);//puts (str);
                        i++;        // go to next character
                        break;
                    }

                        /*  display in hex  */
                    case 'X':
                    case 'x': {
                        int c = va_arg (args, int);
                        char str[32] = {0};
                        itoa_s(c, 16, str);
                        strcpy(sprintf_buffer, str);//puts (str);
                        i++;        // go to next character
                        break;
                    }

                    default:
                        va_end (args);
                        return 1;
                }

                break;

            default:
                *sprintf_buffer += str[i];//putch (str[i]);
                break;
        }

    }

    va_end (args);
    strcpy(dest, sprintf_buffer);
    return i;
}
