#include "system.h"

unsigned int initial_esp;
struct dirent *node = 0;

extern int placement_address;

void main(unsigned int magic, struct multiboot_info *mboot, unsigned int initial_stack) {
    initial_esp = initial_stack;
    hidecursor();
    Video_Init();
    gotoxy(0, 0);
    settextcolor(BLACK, GREEN);
    printf("  neXtgen    v %d.%d  %d", MAJOR, MINOR, DATE);
    settextcolor(LIGHT_BLUE, BLACK);
    do_cpuid_test();
    gotoxy(0, 3);

    GDT_Init();
    IDT_Init();
    ISRS_Init();
    IRQ_Init();

    PIT_Init();
    RTC_Init();
    Keyboard_Init();
    __asm__ __volatile__("sti");
    sleep(5);

    ASSERT(mboot->m_modsCount > 0);
    unsigned int initrd_location = *((unsigned int *) mboot->m_modsAddr);
    unsigned int initrd_end = *(unsigned int *) (mboot->m_modsAddr + 4);
    placement_address = initrd_end;

    initialise_paging(mboot->m_memoryHi);

    fs_root = initialise_initrd(initrd_location);
    initrd_list();

    printf("   GRUB Multiboot header: Magic:");
    if (magic != 0x2BADB002) settextcolor(RED, BLACK);
    printf("%x", magic);
    settextcolor(WHITE, BLACK);
    printf(" LoMem:%dKB HiMem:%dKB ESP:%x\n", mboot->m_memoryLo, mboot->m_memoryHi, initial_esp);

    settextcolor(BLACK, WHITE);
    printf("\n\nPress CTRL+ALT+Z to proceed."); //if so we start Desktop()->WM_Init()->...

    while (1)   // never ending story...
    {
        if (IsShell) {
            shell();
            IsShell = 0;
            WinMngrWorking = 1;
            DrawDesktop();
        }
        __asm__ __volatile__ ("nop"); // let our CPU rest a little
    }
    return 0; // we should never get here
}

