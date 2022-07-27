#include "head.h"

int main(int argc, char *argv[]) {
    int i;
    for(i = 1; i<argc; i++) {
        assembler(argv[i]);
    }
    return 0;
}

void assembler(char *fileName) {        /* not sure on return type, num of args */
    /* 
    - check if file exist (note the file name is {fileName}.as), if so open it, otherwise print error and exit
    - move the opened file to macroExpand(fileName, src)
    - move the returned file to output directory (use rename)
    - create a pointer to the file made by macroExpand called srcExtended

    ...

    */
}      




