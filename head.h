#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

#ifndef head_INCLUDED

#define head_INCLUDED 0

#define lineLength 81

enum BOOL {false, true};
typedef enum BOOL bool;

void assembler(char *);
FILE *macroExpand(char *fileName, FILE *src);

#endif