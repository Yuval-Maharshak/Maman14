#include "head.h"

typedef struct MACRO {
    char *name;
    char *lines;
}

FILE *macroExpand(char *, FILE *);
bool legalMacro(char *);