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
    char *content = (char *) malloc(sizeof(char) * lineLength), *line = (char *) malloc(sizeof(char) * lineLength);
    char *word, *token=" \t\n";
    macro * tempMac;
    int i, temp, lineNum=0;          /* for indexing */
    bool finalLineFlag;

    strncpy(expandedFileName, fileName, strlen(fileName) - 1);      /* copy the fileName without as ending */
    strcat(expandedFileName, "m");     /* adds am ending */

    expanded = fopen(expandedFileName, "w");        /* creates new file expandedFileName */
    if (!expanded)      /* probably can't fail so it's fine */
        return NULL;        /* might wanna do something different here */
    
    while(finalLineFlag) {
        finalLineFlag = getLineAsmb(src, &line);
        strcpy(content, line);
        word = strtok(content, token);
        if (!word) {                        /* an empty line (only blank characters) */
            if (!inMacro)
                fprintf(expanded, "%s", content);
            printf("Empty line %d\n", lineNum);
        }
        else if (strcmp(word, "macro") == 0) {
            printf("macro decleration at line %d\n", lineNum);
            if (inMacro)
                printf("Error: macro %s defined inside macro %s at line %d\n", strtok(NULL, " "), macroTable.head->m.name, lineNum);        /* this will cause unexpected behaviour when Error (need to check) */
            else {
                word = strtok(NULL, token);
                printf("$%s$", word);
                if (legalMacro(word) && strtok(NULL, " ") == NULL && !search(&macroTable, word)) {
                    tempMac = (macro *) malloc(sizeof(macro)); /**/
                    tempMac->name = (char *) malloc(sizeof(char) * strlen(word));
                    strcpy(tempMac->name, word);
                    tempMac->startLine = ftell(src);
                    tempMac->endLine = -1;
                    insert(&macroTable, tempMac);
                    inMacro = true;
                }
                else
                    printf("Error: macro %s defined at line %d, is poorly defined\n", word, lineNum);     /* this will cause unexpected behaviour when Error (need to check) */
            }
                
        }
        else if (strcmp(word, "endmacro") == 0) {
            printf("ending a macro at line %d\n", lineNum);
            if (!inMacro) 
                printf("Error: end_macro statement outsided of a macro at line %d \n", lineNum);
            /* else if there is another word - error */
            else {
                macroTable.head->m.endLine = ftell(src) - strlen(line);
                inMacro = false;
            }
        }
        else if ((tempMac = search(&macroTable, word))) {
            printf("macro name at line %d\n", lineNum);
            if (inMacro)
                printf("Error: macro %s is called inside macro %s at line %d\n", tempMac->name, macroTable.head->m.name, lineNum);
            else {
                temp = ftell(src);
                fseek(src, tempMac->startLine, SEEK_SET);
                for(i=tempMac->startLine+1; i<tempMac->endLine; i++) {     /* i is +1 because ftell is where \n is in line so we need to get one after */
                    char h = fgetc(src);
                    fprintf(expanded, "%c", h);
                }
                fseek(src, temp, SEEK_SET);
            }
        }
        else {
            
            if (!inMacro) {
                printf("normal line outside macro at %d\n", lineNum);
                fprintf(expanded, "%s\n", line);
            }
            else
                printf("normal line inside macro at %d\n", lineNum);
        }
        lineNum++;
    }
    
    fclose(expanded);
    free(expandedFileName);
    free(content);

    return expanded;
}


bool getLineAsmb(FILE *f, char **line) {        /* assumes line point at an allocated block of at least LINE_LENGTH */
    int i=0;
    int c;
    memset(*line,0,strlen(*line));
    while((c = fgetc(f)) != '\n' && c != EOF) {
        (*line)[i] = c;
        i++;
    }
    (*line)[i] = '\0';
    return c != EOF;
}