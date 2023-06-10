#include "extList.h"
#include "translate.h"

/** The Method insert the data into the extern symbol table */
void insertExt(extList *lst, char *word, extent type) 
{
    extNode *en = (extNode *) malloc(sizeof(extNode));/* temporary node */
    failMalloc(en)
    en->Name = (char *) malloc(sizeof(char) * strlen(word));
    strcpy(en->Name, word);
    en->type = type;

    en->next = lst->head;
    lst->head = en;
}

/* The Method checks if word exist inside the extern table. */
bool isExt(extList *lst, char *word) 
{
    extNode *ptr = lst->head;
    while(ptr) 
    {
        if (strcmp(ptr->Name, word) == 0) 
            return ptr->type;
        ptr = ptr->next;
    }
    return -1;
}

/*  The Method prints the extern list into file.ext */
void printExt(FILE *ext, char *name, int adress) 
{
    fprintf(ext, "%s ", name);
    fprintf(ext, "%s\n", translateNumTo32(adress + 100));
}


void printExtList(extList *lst) 
{
    extNode *ptr = lst->head;
    while(ptr) {
        /*printf("%s ", ptr->Name);
        printf("%d\n", ptr->type);*/
        ptr = ptr->next;
    }
}
