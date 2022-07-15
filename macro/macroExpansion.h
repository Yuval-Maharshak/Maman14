#include "../head.h"

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

FILE *macroExpand(char *fileName, FILE *src) ;
bool legalMacro(char *);