#include "head.h"

int main(int argc, char *argv[]) {
    int i;
    for(i = 0; i<argc; i++) {
        assembler(argv[i]);
    }
}

void assembler(char *fileName) {        /* not sure on return type, num of args */
    /* 
    - check if file exist (note the file name is {fileName}.as), if so open it, otherwise print error and exit
    - move the opened file to macroExpand(fileName, src)
    - create a pointer to the file made by macroExpand called srcExtended
    ...
    */
}      
