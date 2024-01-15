#include <X11/X.h>
#include <stdio.h>
#include <stdbool.h>
#include "include/some_h.h"

#include <X11/Xlib.h>

typedef struct Position {
    int x;
    int y;
} Position;

int main(void) {
    XEvent event;
    Position coor;
    coor.x = 50;
    coor.y = 50;
    char hello[] = "Hello, World!\n";

    Display *display = XOpenDisplay(NULL);
    if (display == NULL) {
        fprintf(stderr, "Unable to open display!\n");
        return 1;
    }

    Window root = DefaultRootWindow(display);
    Window win = XCreateSimpleWindow(display, root, 0, 0, 800, 600, 1, BlackPixel(display, 0), WhitePixel(display, 0));
    XMapWindow(display, win);
    XFlush(display);
    XSelectInput(display, win, ExposureMask | KeyPressMask);

    while (1) {
        XNextEvent(display, &event);
        switch(event.type) {
            case Expose:
                XDrawString(display, win, DefaultGC(display, 0), coor.x, coor.y, hello, 13);
                XFlush(display);
                coor.x = coor.x + 5;
                coor.y = coor.y + 5;
                break;
            case KeyPress:
                XDrawString(display, win, DefaultGC(display, 0), coor.x, coor.y, "Key pressed", 13);
                XFlush(display);
                coor.x = coor.x + 5;
                coor.y = coor.y + 5;
                break;
        }
    }

    XCloseDisplay(display);

    return 0;
}
