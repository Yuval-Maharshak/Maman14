#include "macroExpansion.h"

FILE *macroExpand(char *fileName, FILE *src) {          /* not sure if needed src and it's name */
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
    char *expandedFileName = (char *) malloc(sizeof(char) * strlen(fileName) + 1);       /* the name of expanded file */       /* REMEMBER TO FREE */
    FILE *expanded;                 /* the expanded file */
    macroList macroTable = {NULL};  /* the macro collection */
    bool inMacro = false;           /* a flag to check if we're in a macro defenition */
    char *line = (char *) malloc(sizeof(char) * lineLength);
    char *word;
    macro * tempMac;
    int i, temp, lineNum=0;          /* for indexing */

    strncpy(expandedFileName, fileName, strlen(fileName) - 1);      /* copy the fileName without as ending */
    strcat(expandedFileName, "m");     /* adds am ending */

    expanded = fopen(expandedFileName, "w");        /* creates new file expandedFileName */
    if (!expanded)      /* probably can't fail so it's fine */
        return NULL;        /* might wanna do something different here */
    
    while(getline(src, &line)) {
        word = strtok(line, "\t ");
        if (strcmp(word, "macro") == 0) {
            if (inMacro)
                printf("Error: macro %s defined inside macro %s at line %d\n", strtok(NULL, " "), macroTable.head->m.name, lineNum);        /* this will cause unexpected behaviour when Error (need to check) */
            else {
                word = strtok(NULL, " ");
                if (legalMacro(word) && strtok(NULL, " ") == NULL && !search(&macroTable, word)) {
                    tempMac = {word, ftell(src), -1};
                    insert(&macroTable, &tempMac);
                    inMacro = true;
                }
                else
                    printf("Error: macro %s defined at line %d, is poorly defined\n", word, lineNum);     /* this will cause unexpected behaviour when Error (need to check) */
            }
                
        }
        else if (strcmp(word, "end_macro")) {
            if (!inMacro) 
                printf("Error: end_macro statement outsided of a macro at line %d \n", lineNum);
            else {
                macroTable.head->m.endLine = ftell(src);
                inMacro = false;
            }
        }
        else if (tempMac = search(&macroTable, word)) {
            if (inMacro)
                printf("Error: macro %s is called inside macro %s at line %d\n", tempMac->name, macroTable.head->m.name, lineNum);
            else {
                temp = ftell(src);
                fseek(src, tempMac->startLine, SEEK_SET);
                for(i=tempMac->startLine; i<tempMac->endLine; i++) {
                    fprintf(expanded, "%c", fgetc(src));
                }
                fseek(src, temp, SEEK_SET);
            }
        }
        else {
            if (!inMacro) {
                fprintf(expanded, line);
            }
        }
        lineNum++;
    }
    
    fclose(expanded);
    free(expandedFileName);
    free(line);

    return expanded;
}

bool legalMacro(char *name) {       /* add to the paramaters macroArr */
    /*
        illigal names: assembly's commands, starting numbers, already in macroArr, 
    */
    if (reservedWord(name))
        return false;
    if (isdigit(name[0]))
        return false;
    return true;
}
bool reservedWord(char *name) {
    return in(name, two_operands_cmd,5) || in(name, one_operands_cmd,9) || in(name, zero_operands_cmd,2) || in(name, regs_list,8) || in(name, instruction_type,5);
}

bool in(char *name, char *lst[], int length) {
    for (; length > 0; length--) {
        if (strcmp(name, lst[length-1]) == 0)
            return true;
    }
    return false;
}

int getLineAsmb(FILE *f, char **line) {        /* HOWW */

}