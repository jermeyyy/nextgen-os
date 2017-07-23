#ifndef KEYMAP_H
#define KEYMAP_H


#define KBD_DATA        0x60
#define KBD_PORT        0x64

#define KBD_ENABLE_AUX        0xA8
#define KBD_WRITE_CMD        0x60
#define KBD_WRITE_AUX        0xD4
#define KBD_DISABLE_KBD        0x10
#define KBD_DISABLE_AUX        0x20

#define KBD_INFO        0xE9
#define KBD_SET_LEDS        0xED
#define KBD_IDENT        0xF2
#define KBD_ENABLE        0xF4
#define KBD_ACK            0xFA
#define KBD_RESET        0xFF

#define    KEY_LCTRL        0x1D
#define    KEY_RCTRL        0x1D
#define    KEY_LSHIFT        0x2A    /* L and R are the same */
#define    KEY_RSHIFT        0x36
#define    KEY_LALT        0x38    /* L and R are the same */
#define    KEY_RALT        0x38
#define    KEY_CAPS        0x3A
#define KEY_NUM            0x45
#define KEY_SCRL        0x46

#define    KEY_F1            0x80
#define    KEY_F2            0x81
#define    KEY_F3            0x82
#define    KEY_F4            0x83
#define    KEY_F5            0x84
#define    KEY_F6            0x85
#define    KEY_F7            0x86
#define    KEY_F8            0x87
#define    KEY_F9            0x88
#define    KEY_F10            0x89
#define    KEY_F11            0x8A
#define    KEY_F12            0x8B

#define    KEY_INS            0x90
#define    KEY_DEL            0x91
#define    KEY_HOME        0x92
#define    KEY_END            0x93
#define    KEY_PGUP        0x94
#define    KEY_PGDN        0x95
#define    KEY_LFT            0x96
#define    KEY_UP            0x97
#define    KEY_DN            0x98
#define    KEY_RT            0x99
#define    KEY_PRINT        0x9A
#define    KEY_PAUSE        0x9B
#define    KEY_LWIN        0x9C
#define    KEY_RWIN        0x9D
#define    KEY_MENU        0x9E

#define    KBD_CTRL        0x200
#define    KBD_ALT            0x400
#define    KBD_SHIFT        0x800
#define    KBD_CAPS        0x1000
#define    KBD_NUM            0x2000
#define    KBD_SCRL        0x4000


/* KBDUS means US Keyboard Layout. This is a scancode table
*  used to layout a standard US keyboard. */
unsigned char kbdus[89];

/* keyboard utils */

extern void Keyboard_Init();

extern void keyboard_handler(struct regs *);

extern char getch();


#endif
