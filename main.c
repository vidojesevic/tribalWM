/*
* Tribal Window Manager
*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// My headers
#include "include/some_h.h"
#include "include/errors.h"

// X
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/XKBlib.h> // XkbKeycodeToKeysym

#define VERSION "v0.0.1"

typedef struct Position {
    int x;
    int y;
} Position;

int main(int argc, char *argv[]) {
    if (argc == 2 && !strcmp(argv[1], "-v"))
        die("TribalWM-"VERSION"\n");

    XEvent event;
    Position coor;
    coor.x = 50;
    coor.y = 50;
    char hello[] = "Hello, World!\n";

    Display *display = XOpenDisplay(NULL);
    if (display == NULL)
        die("Unable to open display\n");

    Window root = DefaultRootWindow(display);
    Window win = XCreateSimpleWindow(display, root, 0, 0, 800, 600, 1, BlackPixel(display, 0), WhitePixel(display, 0));
    XMapWindow(display, win);
    XFlush(display);
    XSelectInput(display, win, ExposureMask | KeyPressMask | KeyReleaseMask);

    while (1) {
        XNextEvent(display, &event);
        switch(event.type) {
            case Expose:
                {
                    XDrawString(display, win, DefaultGC(display, 0), coor.x, coor.y, hello, 13);
                    XFlush(display);
                    coor.x = coor.x + 10;
                    coor.y = coor.y + 10;
                }
                break;
            case KeyPress:
                {
                    XDrawString(display, win, DefaultGC(display, 0), coor.x, coor.y, "Key pressed", 13);
                    XFlush(display);
                    coor.x = coor.x + 10;
                    coor.y = coor.y + 10;
                }
                break;
            case KeyRelease:
                {
                    KeySym keysym = XkbKeycodeToKeysym(display, event.xkey.keycode, 0, 0);
                    char key[20];
                    snprintf(key, sizeof(key), "Key: %d (%#x): %lu", event.xkey.keycode, event.xkey.keycode, keysym);
                    XDrawString(display, win, DefaultGC(display, 0), coor.x, coor.y, key, strlen(key));
                    XFlush(display);
                    coor.x = coor.x + 10;
                    coor.y = coor.y + 10;
                    printf("Key: %x\n", event.xkey.keycode);
                }
                break;
        }
    }

    XCloseDisplay(display);

    return 0;
}
