#include <X11/X.h>
#include <stdio.h>
#include <stdbool.h>
#include "include/some_h.h"

#include <X11/Xlib.h>

int main(void) {
    XEvent event;
    printf("Let's build some X11 Window Manager!\n");
    char hello[] = "Hello, World!\n";
    print_hello(hello);

    Display *display = XOpenDisplay(NULL);
    Window w = XCreateSimpleWindow(display, DefaultRootWindow(display), 50, 50, 250, 250, 1, BlackPixel(display, 0), WhitePixel(display, 0));
    XMapWindow(display, w);
    XSelectInput(display, w, ExposureMask);

    for (;;) {
        XNextEvent(display, &event);
        if (event.type == Expose) {
            XDrawString(display, w, DefaultGC(display, 0), 100, 100, hello, 13);
            // XDrawLine(display, w, DefaultGC(display, 0), 100, 100, 13, 13);
        }
    }

    return 0;
}
