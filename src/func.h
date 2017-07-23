void Desktop() {
    clock_visible = 1;
    WM_Init();
    AddWindow("Shnela", 12, 36, 4, 6);
    AddWindow("Sick PC", 12, 20, 53, 6);
    AddWindow("neXtgen OS - Welcome!", 15, 53, 15, 5);

    WindowList[WindowIndex("neXtgen OS - Welcome!")].content =
            "Welcome in neXtgen, the x86 operating system\n"
                    "made by Jermey(http://blog.jermey.pl).\n"
                    "Currently neXtgen provides limited functionality\n"
                    "as it is in early development stage.\n"
                    "Project page can be found at\n"
                    "http://nextgen.jermey.pl\n"
                    "Questions? Ideas? Problems?\n"
                    "Contact me at jermey@jermey.pl\n\n"
                    "If you need help press F1,\n"
                    "press Alt+K to close this window.";
    WindowList[WindowIndex("Shnela")].content = "Shnela to rzyd!\nSorry, nie ma polskich znakow...\n...jeszcze";
    WindowList[WindowIndex("Sick PC")].content = sickpc;
    DrawDesktop();
}


