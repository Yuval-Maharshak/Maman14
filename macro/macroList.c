#include "macroExpansion.h"

void insert(macroList *lst, macro *mac) {
    /* 1. allocate node */
    macroNode *new_node = (macroNode *) malloc(sizeof(macroNode));
   
    /* 2. put in the data  */
    new_node->m  = *mac;
   
    /* 3. Make next of new node as head */
    new_node->next = lst->head;
   
    /* 4. move the head to point to the new node */
    lst->head  = new_node;
}
macro * search(macroList *lst, char *name) {
	macroNode *ptr;
	if (name == NULL)
		return NULL;
	for(ptr = lst->head; ptr != NULL; ptr = ptr->next) {
		if (strcmp(name, ptr->m.name) == 0)
			return &(ptr->m);
	}
	return NULL;
}
macroNode * createNode(macro m) {
	macroNode *n = malloc(sizeof(macroNode));
	n->m = m;
	n->next = NULL;
	return n;
}

void printList(macroList *lst) {
	macroNode *ptr;
	printf("< ");
	for(ptr = lst->head; ptr != NULL; ptr = ptr->next) {
		printf("%s, ", ptr->m.name);
	}
	printf(" >\n");
}
