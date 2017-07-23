#ifndef WIN_MNGR_H
#define WIN_MNGR_H
#define MAX_WINS 128
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 24

struct Window {
    unsigned char present;
    unsigned char xpos;
    unsigned char ypos;
    unsigned char width;
    unsigned char height;
    unsigned char focus;
    unsigned char resizable;
    char *title;
    char *content;
};

struct ts_pix {
    unsigned short int Modified;
    char val;
};
struct ts_screen {
    struct ts_pix Buffer[24][80];
    unsigned short int Modified;
} Screen;

struct Window WindowList[MAX_WINS];

extern char *neXtgen_logo;
extern int IsShell;

extern void WM_Init();

extern void DrawDesktop();

extern void DrawWindow(int index);

extern int WindowIndex(char *title);

extern int AddWindow(char *title, int height, int width, int xpos, int ypos);

extern void MoveWindow(int index, int x, int y);

extern void ResizeWindow(int index, int width, int height);

extern int FocusNext();

extern void DeleteWindow(int index);

extern void Help();

extern int GetWindowsCount();

extern void RestartWinMngr();

extern int WinMngrWorking;

extern void DebugWindow();

extern void DrawTimeBar();

extern void ShellWindow();

#endif
