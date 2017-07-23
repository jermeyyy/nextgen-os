/* neXtgen - Keyboard */

#include "win_mngr.h"
#include "system.h"
#include "keyb.h"
#include "func.h"

#define disable_irqs()  __asm__ __volatile__("cli": : :"memory")


unsigned char *kbdhead, *kbdbuf;        // keyboard buffer
const unsigned int kbd_buf_limit = 512;            // safety

int clock_visible = 0;
unsigned char buf;
int change;

unsigned char kbdus[] = {
        0, 27, '1', '2', '3', '4', '5', '6',
        '7', '8', '9', '0', '-', '=', '\b', '\t',
        'q', 'w', 'e', 'r', 't', 'y', 'u', 'i',
        'o', 'p', '[', ']', '\n', 0, 'a', 's',
        'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
        '\'', '`', 0, '\\', 'z', 'x', 'c', 'v',
        'b', 'n', 'm', ',', '.', '/', 0, 0,
        0, ' ', 0, KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5,
        KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, 0, 0, KEY_HOME,
        KEY_UP, KEY_PGUP, '-', KEY_LFT, '5', KEY_RT, '+', KEY_END,
        KEY_DN, KEY_PGDN, KEY_INS, KEY_DEL, 0, 0, 0, KEY_F11,
        KEY_F12
};

inline char toupper(const char toUpper) {
    if ((toUpper >= 'a') && (toUpper <= 'z'))
        return (char) (toUpper - 0x20);
    return toUpper;
}

static void writekbd(unsigned char port, unsigned char data) {
    unsigned long timeout = 500000L;
    unsigned char stat;

    while (timeout) {
        stat = inportb(KBD_PORT);
        if ((stat & 0x02) == 0)
            break;
        timeout--;
    }
    if (timeout != 0)
        outportb(port, data);
}

void pushkbdc(char k) {
    buf = k;
    change = 1;
}

char popkbdc() {
    if (change) {
        change = 0;
        return buf;
    }
    return 0x0;
}

/* Sets the keyboard LEDs */
static void keyboard_leds(unsigned short status) {
    unsigned short temp = 0;
    writekbd(KBD_DATA, KBD_SET_LEDS);
    if (status & KBD_SCRL)
        temp |= 1;
    if (status & KBD_NUM)
        temp |= 2;
    if (status & KBD_CAPS)
        temp |= 4;
    writekbd(KBD_DATA, temp);    /* Bottom 3 bits set LEDs */
}

reboot() {
    puts("Rebooting.\n");
    disable_irqs ();              // Disable Interrupts
    while ((inportb(0x64) & 0x02) != 0); // Clear keyboard buffer
    outportb(0x64, 0xFE);        // Signal the reset to CPU
    // We should'nt get here but if we do halt the CPU
    puts("Cannot reboot machine. Please press the reset button.");
}


/* Handles the keyboard interrupt */
void keyboard_handler(struct regs *r) {
    static unsigned short status;
    unsigned char scancode, key = 0;

    // Read from the keyboard's data buffer
    scancode = inportb(KBD_DATA);

    // TODO: Sync status and LEDs with keyboard

    /* If the top bit of the byte we read from the keyboard is
       set, that means that a key has just been released */
    if (scancode & 0x80) {
        scancode &= 0x7F;         // take the high bit out
        switch (scancode) {
            // switch for state keys (Ctrl, Alt, Shift)
            case KEY_LCTRL: // KEY_RCTRL
                status &= ~KBD_CTRL;
                break;

            case KEY_LALT:          // KEY_RALT
                status &= ~KBD_ALT;
                break;

            case KEY_LSHIFT:        // KEY_RSHIFT
                status &= ~KBD_SHIFT;
                break;
        }

    } else {
        switch (scancode) {
            case KEY_LCTRL: // KEY_RCTRL
                status |= KBD_CTRL;
                break;

            case KEY_LALT:          // KEY_RALT
                status |= KBD_ALT;
                break;

            case KEY_LSHIFT:        // KEY_RSHIFT
                status |= KBD_SHIFT;
                break;

            case KEY_SCRL:
                status ^= KBD_SCRL;
                keyboard_leds(status);
                break;

            case KEY_NUM:
                status ^= KBD_NUM;
                keyboard_leds(status);
                break;

            case KEY_CAPS:
                status ^= KBD_CAPS;
                keyboard_leds(status);
                break;
        }

        /* Ignore invalid scan codes and Convert raw scancode to ASCII key */
        if (scancode >= (sizeof(kbdus) / sizeof(kbdus[0])))
            key = 0;
        else if ((status & KBD_CAPS) || (status & KBD_SHIFT))
            key = toupper(kbdus[scancode]);
        else
            key = kbdus[scancode];

        /* Reboot if we have Ctrl+Alt+Del */
        if ((status & KBD_CTRL) && (status & KBD_ALT) && (key == KEY_DEL)) {
            reboot();
        } else if ((status & KBD_CTRL) && (status & KBD_ALT) && (key == 'z')) {
            Desktop();
        } else if (WinMngrWorking) //WinMngr keyboard shortcuts
        {
            if ((status & KBD_CTRL) && (key == 'w')) {
                RestartWinMngr();
            }

            if ((status & KBD_CTRL) && (key == KEY_UP)) {
                MoveWindow(FindFocused(), 0, -1);
            }

            if ((status & KBD_CTRL) && (key == KEY_DN)) {
                MoveWindow(FindFocused(), 0, 1);
            }

            if ((status & KBD_CTRL) && (key == KEY_LFT)) {
                MoveWindow(FindFocused(), -1, 0);
            }

            if ((status & KBD_CTRL) && (key == KEY_RT)) {
                MoveWindow(FindFocused(), 1, 0);
            }

            if ((status & KBD_ALT) && (key == KEY_DN)) {
                ResizeWindow(FindFocused(), 0, 1);
            }

            if ((status & KBD_ALT) && (key == KEY_RT)) {
                ResizeWindow(FindFocused(), 1, 0);
            }

            if ((status & KBD_ALT) && (key == KEY_UP)) {
                ResizeWindow(FindFocused(), 0, -1);
            }

            if ((status & KBD_ALT) && (key == KEY_LFT)) {
                ResizeWindow(FindFocused(), -1, 0);
            }

            if ((status & KBD_ALT) && (key == 'k')) {
                DeleteWindow(FindFocused());
            }

            if ((status & KBD_ALT) && (key == 'n')) {
                AddWindow("Example window", 12, 32, 0, 2);
                DrawDesktop();
            }

            if ((status & KBD_CTRL) && (key == 'n')) {
                FocusNext();
            }
            if ((status & KBD_CTRL) && (key == 'd')) {
                if (WindowList[WindowIndex("Debug")].present == 1) DeleteWindow(WindowIndex("Debug"));
                else {
                    AddWindow("Debug", 12, 32, 0, 2);
                    WindowList[WindowIndex("Debug")].content = " ";
                    WindowList[WindowIndex("Debug")].resizable = 0;
                    DrawDesktop();
                }
            }
            if (key == KEY_F1) {
                Help();
            }
            if ((status & KBD_CTRL) && (key == 's')) {
                ShellWindow();
            }
        }
            /* If we have a valid key process it */
        else if (key != 0) {
            /* Push key in to the keyboard buffer */
            pushkbdc(key);
            //buf=key;
            //change=1;
        }
    }
}


/* Installs the keyboard handler into IRQ1 */
void
Keyboard_Init() {
    kbdbuf = kmalloc(kbd_buf_limit);
    //kbdhead = kbdbuf;
    //memset(kbdbuf, 0x0 , kbd_buf_limit);
    //*kbdbuf=0x0;
    irq_install_handler(1, keyboard_handler);
    settextcolor(GREEN, BLACK);
    puts("[OK]  ");
    settextcolor(WHITE, BLACK);
    puts("Keyboard\n");

}

char getch() {
    while (popkbdc() == 0x0) {
        showcursor();
        __asm__ __volatile__("hlt");
        hidecursor();
    }
    return buf;
}
