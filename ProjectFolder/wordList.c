#include "funcs.h"

/** This mehted inserts words into the word list */
void insert (wordList *lst, Word word) 
{
    wordNode *wn = malloc(sizeof(wordNode));
    failMalloc(wn)
    wn->word = word;
    wn->next = NULL;
    wn->prev = lst->tail;
    
    if (!lst->head) 
    {
        lst->head = wn;
    }

    if (lst->tail) 
    {
        lst->tail->next = wn;
    }
    lst->tail = wn;
}


/** create an empty word in word list.
 * in second pass filling those empty lines
 */
void insertEmpty(wordList *lst, int n) 
{
    int i;
    adress_num_word EmptyWord = {0,0};
    union gen_word tempUnion;
    Word tempWord;

    tempUnion.adnum = EmptyWord;
    tempWord.word = tempUnion;
    tempWord.type = empty;

    for (i=0; i<n; i++) 
    {
        insert(lst, tempWord);
    }
}


/** free the allocated memory for word list */
void freeList(wordList lst) 
{
    wordNode *word, *tmp;
    word = lst.head;
    while(word) 
    {
        tmp = word->next;
        free(word);
        word = tmp;
    }
}


void printList(wordList lst) 
{
    wordNode *word = lst.head;
    
    while(word) 
    {
        switch(word->word.type) 
        {
            case reg:
                printf("%d %d %d\n", word->word.word.reg.srcReg, word->word.word.reg.destReg, word->word.word.reg.ARE);
                break;
            case adress_num:
                printf("%d %d\n",  word->word.word.adnum.NUM, word->word.word.adnum.ARE);
                break;
            case cmd:
                printf("%d %d %d %d\n", word->word.word.cmd.op_code, word->word.word.cmd.src, word->word.word.cmd.dest, word->word.word.cmd.ARE);
                break;
            case data:
                printf("%d\n", word->word.word.data.DATA);
                break;
            case empty:
                printf("empty\n");
                break;
            default:
                printf("def\n");
                break;
        }
        word = word->next;
    }
    
}
