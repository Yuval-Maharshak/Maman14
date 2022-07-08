#include "macroExpansion.h"

FILE *macroExpand(char *fileName, FILE *src) {
    /*
    - create new file {fileName}.am
    - create a collection of macro structs called macroArr
    - create a flag inMacro to demonstrate if the line is inside a macro
    - loop line by line through src, each line put in a temporary allocated string of 81 chars
        - get first word using strtok
        - if the first word is "macro" then
            - if inMacro then print error to stdout
            - otherwise, check second word(macro's name) legality with legalMacro(name)      
            - if the macro is legal (and there's no third word) add to macroArr a 
              macro struct with name being second word and set inMacro = True       (NOTE: this doesn't account for if the macro has commands in same line as his name)
            - else print error to stdout
        - if the first word is "endmacro" then
            - if not inMacro then print error to stdout
            - otherwise, set inMacro = False
        - if the first word is inside macroArr then
            - if inMacro then print error to stdout
            - otherwise, add to {fileName}.am the macro lines.
        - else (the first word is just some command or something)
            - if inMacro then concatenate the entire line into the lines of the last macro in macroArr
            - otherwise add to {fileName}.am the line
    return {fileName}.am pointer

    """ Remember to free every allocated memory and close every file """
    */
}

bool legalMacro(char *name) {       /* add to the paramaters macroArr */
    /*
        illigal names: assembly's commands, starting numbers, already in macroArr, 
    */
}