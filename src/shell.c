#include "system.h"

void prompt() {
    settextcolor(GREEN, BLACK);
    printf("neXtgen ");
    settextcolor(LIGHT_BLUE, BLACK);
    printf("~ $ ");
    settextcolor(WHITE, BLACK);
}

//! gets next command
void get_cmd(char *buf, char *arg, int n) {
    prompt();
    int isarg = 0, args = 0, key = 0x0;
    int BufChar;
    int i = 0;
    while (i < n) {
        BufChar = true;
        key = getch();
        if (key == '\n') break;
        if (key == ' ') {
            args++;
            isarg = 1;
        }

        if (key == '\b') {
            BufChar = 0;
            if (i > 0) {
                unsigned y, x;
                x = csr_x;
                y = csr_y;
                if (x > 0) gotoxy(--x, y);
                else {
                    y--;
                    x = csr_x;
                }
                putch(' ');
                gotoxy(x, y);
                if (args > 0) {
                    arg[args - 1] = 0x0;
                    args--;
                }
                if (args == 0) isarg = 0;
                i--;
            }
        }

        if (BufChar) {

            char c = key;
            if (c != 0) {
                putch(c);
                if (isarg && c != ' ') {
                    arg[args - 1] = c;
                    args++;
                    i++;
                }
                else if (c != ' ') buf[i++] = c;
                else i++;
            }
        }
    }

    buf[i] = '\0';
    arg[args - 1] = '\0';
}

bool run_cmd(char *cmd_buf, char *arg_buf) {

    //! exit command
    if (strcmp(cmd_buf, "exit") == 0) return 1;
        //! clear screen
    else if (strcmp(cmd_buf, "cls") == 0) cls(' ');
        //! help
    else if (strcmp(cmd_buf, "help") == 0) {
        printf("\nSupported commands:\n");
        printf(" - exit: quits shell\n");
        printf(" - cls: clears the display\n");
        printf(" - help: displays this message\n");
        printf(" - version: displays system version\n");
        printf(" - reboot: restarts system\n");
        printf(" - ls: lists files\n");
        printf(" - cat [filename]: prints file content\n");
        printf(" - echo [text]: prints text\n");
        printf(" - time: prints current time and date\n");
    } else if (strcmp(cmd_buf, "version") == 0) {
        printf("\nneXtgen v %d.%d\n", MAJOR, MINOR);
    } else if (strcmp(cmd_buf, "reboot") == 0) {
        reboot();
    } else if (strcmp(cmd_buf, "ls") == 0) {
        ls();
    } else if (strcmp(cmd_buf, "cat") == 0) {
        cat(arg_buf);
    } else if (strcmp(cmd_buf, "echo") == 0) {
        echo(arg_buf);
    } else if (strcmp(cmd_buf, "time") == 0) {
        putch('\n');
        printTime();
        putch('\n');
    } else {
        settextcolor(RED, BLACK);
        printf("\nUnknown command: ");
        puts(cmd_buf);
        puts(" ");
        puts(arg_buf);
        printf("\n");
        settextcolor(WHITE, BLACK);
    }

    return 0;
}

void shell() {
    gotoxy(0, 0);
    settextcolor(RED, GREEN);
    printf("                            COMMAND SHELL                     ");
    settextcolor(BLACK, GREEN);
    printf("neXtgen    v %d.%d  ", MAJOR, MINOR);
    gotoxy(0, 1);
    unsigned char cmd_buf[100];
    unsigned char arg_buf[100];
    while (1) {
        for (int i = 0; i < 100; i++) cmd_buf[i] = arg_buf[i] = 0x0;
        get_cmd(cmd_buf, arg_buf, 196);
        if (run_cmd(cmd_buf, arg_buf) == 1) break;
    }
}

void echo(unsigned char *text) {
    putch('\n');
    puts(text);
    putch('\n');
}
