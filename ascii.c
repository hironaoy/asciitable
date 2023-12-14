#include <ctype.h>
#include <stdio.h>
#include <termios.h>
#include <string.h>

#define NALPH   26
#define BUFSIZE 32

typedef struct {
    char code;
    char expression[BUFSIZE];
} controlchar;

int main()
{
    // initialize control charactors table
    int i;
    controlchar controlchars[NALPH];
    for (i = 0; i < NALPH; i++) {
	controlchars[i].code = i + 1;

	char expression[BUFSIZE];
	sprintf(expression, "C-%c", i + 97);
	strcpy(controlchars[i].expression, expression);
    }

    // main routine
    struct termios initialstate, newstate;
    tcgetattr(fileno(stdin), &initialstate);

    newstate = initialstate;
    newstate.c_iflag &= ~(IXON | ICRNL);
    newstate.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
    
    tcsetattr(fileno(stdin), TCSANOW, &newstate);

    printf("Type 'q' to quit. Note: 'q' is 0x71.\n");
    char input;
    while (read(fileno(stdin), &input, 1) == 1 && input != 'q') {
	if (iscntrl(input))
	    for (int i = 0; i < NALPH; i++) {
		if (controlchars[i].code == input) {
		    printf("'%s': 0x%02x\n", controlchars[i].expression, input);
		}
	    }	    
	else
	    printf("  '%c': 0x%02x\n", input, input);
    }
    
    tcsetattr(fileno(stdin), TCSANOW, &initialstate);
    return 0;
}
