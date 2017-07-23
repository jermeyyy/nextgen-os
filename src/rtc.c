/* neXtgen - Real Time Clock */

#include "rtc.h"
#include "system.h"

time_t global_time;
int bcd;


unsigned char read_register(unsigned char reg) {
    outportb(0x70, reg);
    return inportb(0x71);
}

void write_register(unsigned char reg, unsigned char value) {
    outportb(0x70, reg);
    outportb(0x71, value);
}

unsigned char bcd2bin(unsigned char bcd) {
    return ((bcd >> 4) * 10) + (bcd & 0x0F);
}

void gettime(time_t *time) {
    memcpy(time, &global_time, sizeof(time_t));
}

void rtc_handler(struct regs *r) {
    if (read_register(0x0C) & 0x10) {
        if (bcd) {
            global_time.second = bcd2bin(read_register(0x00));
            global_time.minute = bcd2bin(read_register(0x02));
            global_time.hour = bcd2bin(read_register(0x04));
            global_time.month = bcd2bin(read_register(0x08));
            global_time.year = bcd2bin(read_register(0x09));
            global_time.day_of_week = bcd2bin(read_register(0x06));
            global_time.day_of_month = bcd2bin(read_register(0x07));
        } else {
            global_time.second = read_register(0x00);
            global_time.minute = read_register(0x02);
            global_time.hour = read_register(0x04);
            global_time.month = read_register(0x08);
            global_time.year = read_register(0x09);
            global_time.day_of_week = read_register(0x06);
            global_time.day_of_month = read_register(0x07);
        }
    }
}

void RTC_Init(void) {
    unsigned char status;

    status = read_register(0x0B);
    status |= 0x02;             // 24 hour clock
    status |= 0x10;             // update ended interrupts
    status &= ~0x20;             // no alarm interrupts
    status &= ~0x40;             // no periodic interrupt
    bcd = !(status & 0x04); // check if data type is BCD
    write_register(0x0B, status);

    read_register(0x0C);

    irq_install_handler(8, rtc_handler);
    settextcolor(GREEN, BLACK);
    printf("[OK]  ");
    settextcolor(WHITE, BLACK);
    printf("Real Time Clock\n");
}

void printTime() {
    printf(" %d", global_time.hour);
    if (global_time.minute < 10) printf(":0%d:", global_time.minute);
    else printf(":%d:", global_time.minute);
    if (global_time.second < 10) printf("0%d", global_time.second);
    else printf("%d", global_time.second);
    printf("  %d ", global_time.day_of_month);
    switch (global_time.month) {
        case 1:
            puts("styczen");
            break;
        case 2:
            puts("luty");
            break;
        case 3:
            puts("marzec");
            break;
        case 4:
            puts("kwiecien");
            break;
        case 5:
            puts("maj");
            break;
        case 6:
            puts("czerwiec");
            break;
        case 7:
            puts("lipiec");
            break;
        case 8:
            puts("sierpien");
            break;
        case 9:
            puts("wrzesien");
            break;
        case 10:
            puts("pazdziernik");
            break;
        case 11:
            puts("listopad");
            break;
        case 12:
            puts("grudzien");
            break;
        default:
            puts("ERROR");
            break;
    }
    printf(" %d%d", global_time.year < 10 ? 200 : global_time.year > 99 ? 2 : 20, global_time.year);
}
