/* neXtgen - System */

#ifndef __SYSTEM_H
#define __SYSTEM_H

#include "scrn.h"
#include "win_mngr.h"
#include "rtc.h"
#include "bootinfo.h"
#include "stdlib.h"
#include "ordered_array.h"
#include "fs.h"
#include "initrd.h"
#include "kheap.h"
#include "paging.h"
#include "cpuid.h"
#include "date.h"


#define bool int
#define true 1
#define false 1
#define MAJOR 0
#define MINOR 3
#define KERNEL_SIZE 128


/* This defines what the stack looks like after an ISR was running */
struct regs {
    unsigned int gs, fs, es, ds;
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;
    unsigned int int_no, err_code;
    unsigned int eip, cs, eflags, useresp, ss;
} register_t;

extern void shell();

extern struct dirent *node;

/* gdt.c */
extern void gdt_set_gate(int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran);

extern void GDT_Init();

/* idt.c */
extern void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags);

extern void IDT_Init();

/* isrs.c */
extern void ISRS_Init();

extern char *sickpc;

/* irq.c */
extern void irq_install_handler(int irq, void (*handler)(struct regs *r));

extern void irq_uninstall_handler(int irq);

extern void IRQ_Init();


/* timer.c */
extern void timer_wait(int ticks);

extern int timer_ticks;

extern void PIT_Init();

extern int rand();

extern int clock_visible;

/* kb.c */
extern void Keyboard_Init();

#endif
