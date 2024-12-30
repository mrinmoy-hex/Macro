#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

struct termios orig_termios; // Store the original terminal attributes


void disableRawMode() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios); // Restore the original terminal attributes
}



void enableRawMode() {
    tcgetattr(STDIN_FILENO, &orig_termios); // Get the current terminal attributes  
    atexit(disableRawMode); // Call disableRawMode when the program exits

    struct termios raw = orig_termios;     // Create a termios struct to store the terminal attributes

    /*
    Turn off the ECHO flag using bitwise operations,
    raw.c_lflag is a bit field representing various local terminal flags
    ECHO is a bit flag (e.g., 00000000000000000000000000001000 in binary)
    to disable it, we use bitwise-NOT operator (~) to flip all the bits
    and bitwise-AND operator (&) to unset the ECHO bit
    resulting in 11111111111111111111111111110111 in binary

    */
    raw.c_lflag &= ~(ECHO | ICANON);     // Turn off echo mode 

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw); // Apply the modified attributes
}


int main() {

    enableRawMode();    // Enable raw mode, disable echoing of input 

    char c;
    // read characters from std input by each character
    // until 'q' is pressed
    while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q') {
        if (iscntrl(c)) {
            printf("%d\n", c);
        } else {
            printf("%d ('%c')\n", c, c);
        }
    }
    return 0;
}