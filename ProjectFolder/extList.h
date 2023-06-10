#include "head.h"
#ifndef extList_INCLUDED
#define extList_INCLUDED


typedef enum extent {
    ext, ent
} extent;   /* type of extNode, wethere the node is extern or entry */

/** Extern symbol table */
typedef struct extNode {
    char *Name;
    extent type;
    struct extNode *next;
}  extNode;

/** Head of extern table */
typedef struct extList {
    extNode *head;
} extList;



/**
* The Method insert the data into the extern symbol table
* @param lst Pointer to the head of extern table
* @param word The label name.
* @param type entry or extern
*/
void insertExt(extList *lst, char *word, extent type);


/**
* The Method checks if word exist inside the extern table.
* @param lst Pointer to the head of extern table
* @param word The label name.
* @return true if word is found otherwise return -1.
*/
bool isExt(extList *lst, char *word);


/**
* The Method prints the extern list into file.ext
* @param ext The file 
* @param name The label name.
* @param adress the symbol address
*/
void printExt(FILE *ext, char *name, int adress);


void printExtList(extList *lst);

#endif
