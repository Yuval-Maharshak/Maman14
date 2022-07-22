#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "reserved_words.c"

#define lineLength 81

typedef enum {
	false,
	true
} bool;

void assembler(char *);