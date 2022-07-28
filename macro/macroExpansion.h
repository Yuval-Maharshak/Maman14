#include "../head.h"

#ifndef macroExpansion_INCLUDED

#define macroExpansion_INCLUDED 0

#define lineCharCycle 10	/* delete */

typedef struct MACRO {
    char *name;
    int startLine;
	int endLine;
} macro;

typedef struct MACRO_NODE macroNode;
struct MACRO_NODE {
	macro m;
	struct MACRO_NODE *next;		/* fix to look nicer */
};

struct macroList {
	macroNode *head;
};
typedef struct macroList macroList;

FILE *macroExpand(char *fileName, FILE *src);
bool legalMacro(char *);
bool getLineAsmb(FILE *, char **);

void insert(macroList *, macro *);
macro * search(macroList *, char *);
macroNode * createNode(macro);
void printList(macroList *lst);

bool reservedWord(char *name);

bool in(char *name, char *lst[], int length);

#endif