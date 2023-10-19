#ifndef KBHIT_H
#define KBHIT_H

#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>

int _kbhit() { // detects whether there is a keyboard input
    static const int STDIN = 0;
    static bool initialized = false;

    if (! initialized) {
        // Use termios to turn off line buffering
        termios term;
        tcgetattr(STDIN, &term);
        term.c_lflag &= ~ICANON;
        tcsetattr(STDIN, TCSANOW, &term);
        setbuf(stdin, NULL);
        initialized = true;
    }

    int bytesWaiting;
    ioctl(STDIN, FIONREAD, &bytesWaiting);
    return bytesWaiting;
}// from https://www.flipcode.com/archives/_kbhit_for_Linux.shtml

#endif