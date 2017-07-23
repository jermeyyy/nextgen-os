/* neXtgen -Real time clock */

#include "rtc.h"
#include "system.h"

#define TIMER_SPEED 100
int timer_ticks = 0;
int wait_ticks = 0;
int waiting = 0;
char *debugstr;

int rand() {
    int b = (global_time.minute + global_time.hour + global_time.second);
    for (int a = 0; a < (int) (b / 3); ++a) {}
    return (timer_ticks + 1 / (b + 1));
}

static void timer_phase(int hz)        /* change timer phase */
{
    int divisor = (3579545 / (3 * hz));    /* Calculate our divisor */
    outportb(0x43, ((0 << 6) | (3 << 4) | (2 << 1)));//0x36);	/* Set command byte */
    outportb(0x40, (unsigned char) divisor & 0xFF);    /* Set low byte of divisor */
    outportb(0x40, (unsigned char) (divisor >> 8));//& 0xFF);	/* Set high byte of divisor */
}

static void timer_handler(struct regs *r) {

    timer_ticks++;
    if (wait_ticks > 0) { wait_ticks--; }
    if (wait_ticks == 0) waiting = 0;
    if (timer_ticks % TIMER_SPEED == 0 && wait_ticks > 0) putdec(wait_ticks / 100, 1);

    if (timer_ticks % 10 == 0 && WinMngrWorking) {
        DrawTimeBar();
        DebugWindow();
    }
}

/* This will continuously loop until the given time has been reached */
void timer_wait(int ticks) {
    unsigned long long eticks;

    eticks = timer_ticks + ticks;
    while (timer_ticks < eticks);
}

inline void sleep(int s) {
/*waiting=1;
wait_ticks=s*TIMER_SPEED;
while(waiting)
if (timer_ticks % TIMER_SPEED == 0 && wait_ticks>0) putdec(wait_ticks/100,1); //{ __asm__ __volatile__("hlt");}*/

/*int hrt=0,mint=0,sect=0;
int hr=0,min=0,sec=0;
hr=global_time.hour;
min=global_time.minute;
sec=global_time.second;
hrt=hr;
mint=min;
if(sec+s>59)
{
sect=0+s;
mint=sec+1;
}
else
sect=sec+s;

while(1)
{
	if(hrt==global_time.hour && mint==global_time.minute && sect==global_time.second)
	return;
	}*/
}

/* Sets up the system clock by installing the timer handler into IRQ0 */
void PIT_Init() {
    /* Installs 'timer_handler' to IRQ0 */
    timer_phase(TIMER_SPEED);
    irq_install_handler(0, timer_handler);
    settextcolor(GREEN, BLACK);
    printf("[OK]  ");
    settextcolor(WHITE, BLACK);
    printf("Programmable Interupt Timer");
    settextcolor(BLUE, BLACK);
    printf("  @ %d Hz\n", TIMER_SPEED);
}
