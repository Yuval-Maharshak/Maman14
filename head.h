#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include "reserved_words.c"

#define lineLength 81

enum BOOL {false, true};
typedef enum BOOL bool;

void assembler(char *);