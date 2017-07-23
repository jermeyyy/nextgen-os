/* neXtgen Window Manager */

#include "system.h"
#include "win_mngr.h"

int help_visible = 0;
int WinMngrWorking = 0;
int IsShell = 0;

char *nextgen_logo =
        "                  [][]    [][]\n"
                "                   [][]  [][]\n"
                "                    [][][][]\n"
                "[]]   |]  [][][]     [][][]   [][[]][]   [][]]   [][][]  []]   |]\n"
                "[|[]  |]  [|          [][]       []     []       [|      [|[]  |]\n"
                "[| [] |]  [][]]      [][][]      []     []  []   [][]]   [| [] |]\n"
                "[|  []|]  [|        [][][][]     []     []   []  [|      [|  []|]\n"
                "[|   [[]  [][][]   [][]  [][]    []      [][][]  [][][]  [|   [[]\n"
                "                  [][]    [][]\n"
                "                 [][]      [][]\n"
                "                         v 0.3 alpha";
char *srsly =
        "░░▄▀░░░░░░░░░░░░░░░░░░░░▀▄░░░\n"
                "░▄▀░░░░░░░░░░░░░░░░░░░░░░░█░░\n"
                "░█░░░░░░░░░░░░░░░░░░░░░░░░░█░\n"
                "▐░░░░░░░░░░░░░░░░░░░░░░░░░░░█\n"
                "█░░░░▀▀▄▄▄▄░░░▄▌░░░░░░░░░░░░▐\n"
                "▌░░░░░|░░▀▀█▀▀░░░▄▄░░░░░░░▌░▐\n"
                "▌░░░░░░▀▀▀▀░░░░░░▌░▀██▄▄▄▀░░▐\n"
                "▌░░░░░░░░░░░░░░░░░▀▄▄▄▄▀░░░▄▌\n"
                "▐░░░░▐░░░░░░░░░░░░░░░░░░░░▄▀░\n"
                "░█░░░▌░░▌▀▀▀▄▄▄▄░░░░░░░░░▄▀░░\n"
                "░░█░░▀░░░░░░░░░░▀▌░░▌░░░█░░░░\n"
                "░░░▀▄░░░░░░░░░░░░░▄▀░░▄▀░░░░░\n"
                "░░░░░▀▄▄▄░░░░░░░░░▄▄▀▀░░░░░░░";

void WM_Init() {

    int index;
    for (int i = 0; i < SCREEN_HEIGHT; i++)
        for (int j = 0; j < SCREEN_WIDTH; j++) {
            Screen.Buffer[i][j].val = 0;
            Screen.Buffer[i][j].Modified = 0;
        }
    for (index = 0; index < MAX_WINS; index++) {
        WindowList[index].present = 0;
        WindowList[index].content = 0;
        WindowList[index].xpos = 0;
        WindowList[index].ypos = 0;
        WindowList[index].height = 0;
        WindowList[index].width = 0;
        WindowList[index].focus = 0;
        WindowList[index].resizable = 0;
        WindowList[index].title = 0;
        Screen.Modified = 0;
    }
    WinMngrWorking = 1;
    DrawDesktop();
}

int WindowIndex(char *title) {
    int index = 0;
    while (index < MAX_WINS) {
        if (strcmp(WindowList[index].title, title) == 0) return index;
        index++;
    }
    return -1;
}

int AddWindow(char *title, int height, int width, int xpos, int ypos) {
    int index = 0, found = 0;
    for (index = 0; index < MAX_WINS; index++) {
        if (WindowList[index].present == 0) {
            found = 1;
            break;
        }
    }
    if (!found) return -1;

    WindowList[index].present = 1;
    WindowList[index].title = title;
    WindowList[index].height = height;
    WindowList[index].width = width;
    WindowList[index].xpos = xpos;
    WindowList[index].ypos = ypos;
    WindowList[index].resizable = 1;
    WindowList[index].content = "<no_content>";
    if (FindFocused() > -1) WindowList[FindFocused()].focus = 0;
    WindowList[index].focus = 1;
    return 1;
}

void DrawDesktop() {
    if (!WinMngrWorking) return;
    int index;
    settextcolor(BLACK, BLACK);
    cls(' '); // we clear screen
    settextcolor(GREEN, BLACK);
    int line = 0;
    gotoxy(8, 6);
    for (int i = 0; i < strlen(nextgen_logo); i++) // draw "wallpaper" - neXtgen logo
    {
        if (nextgen_logo[i] == '\n') {
            line++;
            gotoxy(8, 6 + line);
            continue;
            i++;
        }
        else { putch(nextgen_logo[i]); }
    }

    for (index = 0; index < MAX_WINS; index++) // first we draw unfocused windows
    {
        if (WindowList[index].present == 1 && WindowList[index].focus == 0) DrawWindow(index);
    }
    DrawWindow(FindFocused()); // then the one remaining - focused
    gotoxy(0, SCREEN_HEIGHT);
    int limiter = 0;
    for (index = 0; index < MAX_WINS; index++) //now drawing taskbar
    {
        settextcolor(GRAY, BLUE);
        if (limiter >= SCREEN_WIDTH) {
            continue;
        }
        if (WindowList[index].present) {
            limiter += strlen(WindowList[index].title) + 1;
            if (limiter >= SCREEN_WIDTH) {
                limiter -= strlen(WindowList[index].title) + 1;
                break;
            }
            if (WindowList[index].focus == 1) settextcolor(WHITE, BLUE);
            puts(WindowList[index].title);
            putch(' ');
        }
    }
    scrolls = 0; /* we hack scroll function so it won't scroll
					while we are drawing taskbar */
    for (index = 0; index < (SCREEN_WIDTH - limiter); index++) {
        puts(" ");
    }
    scrolls = 1;
}

void DrawWindow(int index) {
    if (index == -1) return;
    int xpos = WindowList[index].xpos;     // we make rest
    int ypos = WindowList[index].ypos;     // of the function
    int width = WindowList[index].width;     // more
    int height = WindowList[index].height; // readable
    int focus = WindowList[index].focus;     // ->

    int xextent = (xpos + width - 1), yextent = (ypos + height - 1);
    int x, y;
    // draw window
    for (y = ypos; y <= yextent; y++) {
        settextcolor(LIGHT_WHITE, BLUE);
        gotoxy(xpos, y);

        if (y == ypos) {
            // draw top caption bar
            if (focus)
                settextcolor(WHITE, RED);
            if (!focus)
                settextcolor(GRAY, RED);

            for (x = xpos; x <= xextent; x++) {
                if (x == xpos) putch(213);
                else if (x == xextent) putch(254);
                else putch(205);
            }
            gotoxy(xpos + ((width - strlen(WindowList[index].title)) / 2), y);
            putch(' ');
            puts(WindowList[index].title);
            putch(' ');

        } else if (y < yextent) {
            // draw middle lines
            for (x = xpos; x <= xextent; x++) {
                if (x == xpos || x == xextent) putch(179);
                else putch(' ');
            }

        } else {
            // draw bottom edge
            for (x = xpos; x <= xextent; x++) {
                if (x == xpos) putch(192);
                else if (x == xextent) putch(217);
                else putch(196);
            }
        }
    }

    gotoxy(xpos + 2, ypos + 2);
    settextcolor(LIGHT_WHITE, BLUE);
    int line = 0, cursor_pos = 0;
    // now time to print content
    if (focus) settextcolor(WHITE, BLUE);
    for (int i = 0; i < strlen(WindowList[index].content); i++) {
        if (cursor_pos == (width - 4)) {
            line++;
            gotoxy(xpos + 2, ypos + 2 + line);
            cursor_pos = 0;
        }
        if (line == (height - 4)) break;
        if (WindowList[index].content[i] == '\n') {
            line++;
            gotoxy(xpos + 2, ypos + 2 + line);
            cursor_pos = 0;
            continue;
            i++;
        }
        else {
            putch(WindowList[index].content[i]);
            cursor_pos++;
        }
    }
}

void MoveWindow(int index, int x, int y) {
    if (index == -1)return;
    if ((WindowList[index].xpos > 0 || (WindowList[index].xpos == 0 && x > 0)) &&
        (WindowList[index].xpos + WindowList[index].width < SCREEN_WIDTH ||
         (WindowList[index].xpos + WindowList[index].width == SCREEN_WIDTH && x < 0)))
        WindowList[index].xpos += x;
    if ((WindowList[index].ypos > 1 || (WindowList[index].ypos == 1 && y > 0)) &&
        (WindowList[index].ypos + WindowList[index].height < SCREEN_HEIGHT ||
         (WindowList[index].ypos + WindowList[index].height == SCREEN_HEIGHT && y < 0)))
        WindowList[index].ypos += y;

    DrawDesktop();
}

int MinHeight(index) {
    int minHeight = 5;
    for (int i = 0; i < strlen(WindowList[index].content); i++) {
        if (WindowList[index].content[i] == '\n') minHeight++;
    }
    return minHeight;
}

int MinWidth(index) {
    int minWidth;
    int Max = 0;
    for (int i = 0; i < strlen(WindowList[index].content); i++) {
        minWidth++;
        if (WindowList[index].content[i] == '\n') {
            Max = (minWidth > Max ? minWidth : Max);
            minWidth = 0;
        }
    }
    return Max + 4;
}

void ResizeWindow(int index, int width, int height) {
    if (index == -1 || WindowList[index].resizable == 0)return;
    if ((WindowList[index].width <= MinWidth(index) && width < 0) ||
        (WindowList[index].height <= MinHeight(index) && height < 0))
        return;
    if ((WindowList[index].width > (strlen(WindowList[index].title) + 5) ||
         (WindowList[index].width == (strlen(WindowList[index].title) + 5)) && width > 0)
        && (WindowList[index].xpos + WindowList[index].width < SCREEN_WIDTH ||
            (WindowList[index].xpos + WindowList[index].width && width < 0)))
        WindowList[index].width += width;
    if ((WindowList[index].height > 4 || (WindowList[index].height == 4 && height > 0)) &&
        (WindowList[index].ypos + WindowList[index].height < 24
         || (WindowList[index].ypos + WindowList[index].height == SCREEN_HEIGHT && height < 0)))
        WindowList[index].height += height;
    DrawDesktop();
}

int FindFocused() {
    int index = 0;
    while (index < MAX_WINS) {
        if (WindowList[index].focus == 1) return index;
        index++;
    }
    return -1;
}

int FocusNext() {
    int index = 0, focused = 0;
    index = FindFocused() + 1;
    focused = FindFocused();
    if (focused == -1) return -1;
    while (1) {
        if (index > MAX_WINS) index = 0;
        if (WindowList[index].focus == 0 && WindowList[index].present == 1) {
            WindowList[focused].focus = 0;
            WindowList[index].focus = 1;
            DrawDesktop();
            return 0;
        } else if (index == focused) return 0;
        index++;
    }
}

void DeleteWindow(int index) {
    DeleteWindowR(index, 1);
}

void DeleteWindowR(int index, int redraw) {
    if (index == -1)return;
    if (WindowList[index].present == 1) {
        WindowList[index].present = 0;
        WindowList[index].width = 0;
        WindowList[index].height = 0;
        WindowList[index].xpos = 0;
        WindowList[index].ypos = 0;
        WindowList[index].focus = 0;
        WindowList[index].title = " ";
        WindowList[index].content = " ";
        index = 0;
        while (index < MAX_WINS) {
            if (WindowList[index].focus == 0 && WindowList[index].present == 1) {
                WindowList[index].focus = 1;
                break;
            }
            index++;
        }
        if (redraw) DrawDesktop();
    }
}

void Help() {
    help_visible = (help_visible ? 0 : 1);
    int i, j;
    if (help_visible) {
        for (i = 0; i < 18; i++) {
            settextcolor(WHITE, WHITE);
            for (j = 0; j < 48; j++) {
                gotoxy(17 + j, 4 + i);
                putch('-');
            }
        }
        settextcolor(BLACK, WHITE);
        gotoxy(17 + 22, 5);
        printf("HELP");
        gotoxy(17 + 4, 7);
        printf("CTRL+W  -  Restart WinMngr");
        gotoxy(17 + 4, 8);
        printf("CTRL+arrows  -  Move Window");
        gotoxy(17 + 4, 9);
        printf("ALT+arrows  -  Resize Window");
        gotoxy(17 + 4, 10);
        printf("CTRL+N  -  Switch to next window");
        gotoxy(17 + 4, 11);
        printf("ALT+K  -  Close window");
        gotoxy(17 + 4, 12);
        printf("ALT+N  -  New window");
        gotoxy(17 + 4, 13);
        printf("CTRL+S  -  Open shell");
        gotoxy(17 + 4, 14);
        printf("CTRL+ALT+DEL  -  Reboot computer");
        gotoxy(17 + 4, 15);
        printf("CTRL+D - Debug window");
        gotoxy(17 + 15, 20);
        printf("neXtgen v %d.%d", MAJOR, MINOR);
    } else DrawDesktop();
}

int GetWindowsCount() {
    int count = 0;
    for (int index = 0; index < MAX_WINS; index++)
        if (WindowList[index].present == 1) count++;
    return count;
}

void RestartWinMngr() {
    for (int index = 0; index < MAX_WINS; index++) {
        if (WindowList[index].present) DeleteWindowR(index, 0);
    }
    WM_Init();
}

void DebugWindow() {
    if (WindowList[WindowIndex("Debug")].present == 1 && WindowList[WindowIndex("Debug")].focus == 1) {
        settextcolor(WHITE, BLUE);
        gotoxy(WindowList[WindowIndex("Debug")].xpos + 2, WindowList[WindowIndex("Debug")].ypos + 2);
        printf("neXtgen v %d.%d alpha", MAJOR, MINOR);
        gotoxy(WindowList[WindowIndex("Debug")].xpos + 2, WindowList[WindowIndex("Debug")].ypos + 3);
        printf("timer_tics:%d", timer_ticks);
        gotoxy(WindowList[WindowIndex("Debug")].xpos + 2, WindowList[WindowIndex("Debug")].ypos + 4);
        //printf("memory_size:%dKB",GetMemorySize());
        //gotoxy(WindowList[WindowIndex("Debug")].xpos+2,WindowList[WindowIndex("Debug")].ypos+5);
        //printf("blocks total:%d used:%d",GetBlockCount(), GetUseBlockCount());
        //gotoxy(WindowList[WindowIndex("Debug")].xpos+2,WindowList[WindowIndex("Debug")].ypos+6);
        //printf("Paging: %d   ",IsPaging());
        //gotoxy(WindowList[WindowIndex("Debug")].xpos+2,WindowList[WindowIndex("Debug")].ypos+7);
        printf("Windows: %d", GetWindowsCount());
    }
}

void ShellWindow() {
    WinMngrWorking = 0;
    settextcolor(WHITE, BLACK);
    cls(' ');
    IsShell = 1;
}

void DrawTimeBar() {
    if (clock_visible && WinMngrWorking) {
        settextcolor(WHITE, BLUE);
        gotoxy(0, 0);
        puts("                                                                                ");
        gotoxy(0, 0);
        printTime();

    }
}
