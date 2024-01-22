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
#include <X11/keysym.h> // Detecting the key


#define VERSION "v0.0.1"

typedef struct Position {
    int x;
    int y;
} Position;

typedef struct FScreen {
    int width;
    int height;
} FScreen;

int getScreenWidth(Display *display, int screen_number) {
    return XDisplayWidthMM(display, screen_number);
}

int getScreenHeight(Display *display, int screen_number) {
    return XDisplayHeightMM(display, screen_number);
}

int detectKeyPress(Display *display, Window root);
void handleKeyPress(int key);
int errorHandler(Display *display, XErrorEvent *event);

void spawnTerminal() {
    system("st");  // Replace with the command to start your preferred terminal
}

int main(int argc, char *argv[]) {
    if (argc == 2 && !strcmp(argv[1], "-v"))
        die("TribalWM-"VERSION"\n");
    else if (argc != 1)
        die("Bad call to program. Type tribalwm\n");

    XSetErrorHandler(errorHandler);

    XEvent event;
    Position coor;
    coor.x = 50;
    coor.y = 50;
    char hello[] = "Hello, World!\n";

    Display *display = XOpenDisplay(NULL);
    if (display == NULL)
        die("Unable to open display\n");

    FScreen full_screen = { .width = getScreenWidth(display, 0), .height = getScreenHeight(display, 0) };

    Window root = DefaultRootWindow(display);
    Window win = XCreateSimpleWindow(display, root, 0, 0, full_screen.width, full_screen.height, 1, BlackPixel(display, 0), WhitePixel(display, 0));
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
                    XSync(display, False);
                    coor.x = coor.x + 10;
                    coor.y = coor.y + 10;
                } break;
            case KeyPress: {
                int key = detectKeyPress(display, root);
                handleKeyPress(key);
                if (event.xkey.keycode == XKeysymToKeycode(display, XK_T) &&
                    event.xkey.state & ControlMask && event.xkey.state & ShiftMask) {
                    spawnTerminal();
                }
                XFlush(display);
                XSync(display, False);
            } break;
            case MapNotify:
                printf("Window mapped\n");
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
                } break;
        }
    }

    XCloseDisplay(display);

    return 0;
}

int detectKeyPress(Display *display, Window root) {
    // KeyCode key_code = XKeysymToKeycode(display, XK_q);
    //
    // XGrabKey(display, key_code, ControlMask, root, True, GrabModeAsync, GrabModeAsync);
    XEvent event;
    while(XPending(display) > 0) {
        XNextEvent(display, &event);
        if (event.type == KeyPress && event.xkey.keycode == XKeysymToKeycode(display, XK_q) && event.xkey.state & ControlMask) {
            XUngrabKey(display, XKeysymToKeycode(display, XK_q), ControlMask, root);
            return XK_q;
        }
    }
    return -1;
}

void handleKeyPress(int key) {
    if (key == -1) {
        die("There has been some error while processing the key\n");
    }
    if (key == XK_q) {
        // Handle Ctrl+Q press
        printf("Ctrl+Q pressed\n");
        // Additional handling logic here
    }
    // You can add more cases for other keys if needed
}

int errorHandler(Display *display, XErrorEvent *event) {
    char error_message[256];
    XGetErrorText(display, event->error_code, error_message, sizeof(error_message));
    fprintf(stderr, "Xlib error: %s\n", error_message);
    return 0;
}
