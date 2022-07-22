#include "macroExpansion.h"

void insert(macroList *lst, macroNode *n) {
	if (lst == NULL)
		return;
	n->next = lst->head;
	lst->head = n;
}
macro * search(macroList *lst, char *name) {
	macroNode *ptr;
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
