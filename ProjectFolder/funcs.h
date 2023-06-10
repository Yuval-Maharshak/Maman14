#ifndef FUNCS_H
#define FUNCS_H

#include "head.h"
#include "global_var.h"
#include "utils.h"
#include "symbol_table.h"
#include "first_line_check.h"
#include "str_methods.h"
#include "extList.h"


/** Opcode encode */
typedef struct cmd_word {
    unsigned int ARE: 2;                
    unsigned int dest: 2;               
    unsigned int src: 2;
    unsigned int op_code: 4;
} cmd_word;

/** immediate operand encode  */
typedef struct adress_num_word {
    unsigned int ARE: 2;
    unsigned int NUM: 8;
} adress_num_word;

/** registers encode */
typedef struct reg_word {
    unsigned int ARE: 2;
    unsigned int destReg: 4;
    unsigned int srcReg: 4;
} reg_word;

/** data encode */
typedef struct data_word {
    unsigned int DATA: 10;
}data_word;

typedef enum Type {
    reg, adress_num, cmd, data, empty
}type;

/** union to generate all types of words in the assembly language */
union gen_word {
    reg_word reg;
    adress_num_word adnum;
    cmd_word cmd;
    data_word data;
};

typedef struct Word {
    union gen_word word;
    enum Type type; 
} Word;

typedef struct wordNode wordNode;

struct wordNode {
    Word word;
    wordNode *next;
    wordNode *prev;
};

typedef struct wordList {
    wordNode *head;
    wordNode *tail;
} wordList;

/** converts every 5 bits in binary base into a single bit in 32 base */
typedef struct wordForTranslation {
    unsigned int firstHalf: 5;
    unsigned int secondHalf: 5;
} wordForTranslation;


/**
* The Method gets a command name and search for the command in the command table
* @param command  
* @return if command found return's the command number else return's -1.
*/
int searchCommand(char *command);


/**
* The Method gets a line and a the line number in the source file and converts the line
* into a assembly opcpde lines depend on the command parameters.
* @param line The command line  
* @param lineNum The line number from the source file.
* @return Pointer to the encoded opcode line
*/
cmd_word *analysCommand(char *line, int lineNum);


/**
* This method gets an immediate number and encode the line
* @param num The number to encode.
* @return Pointer to the encoded immediate line
*/
adress_num_word analysNumber(char *num);


/**
* This method gets an source and destination registers and encode the line
* @param dest The destination register
* @param src The source register.
* @return Pointer to the encoded register's line
*/
reg_word analysRegister(char *dest, char *src);


/**
* This method insert to a word list a given word
* @param lst the list of words
* @param word the word to be inserted
*/
void insert (wordList *lst, Word word);

/**
* This method insert a given amount of empty elements to a word list
* @param lst the list of words
* @param n the number of empty words to be inserted to lst
*/
void insertEmpty(wordList *lst, int n);

/**
* This method free the memory kept by the word list
* @param lst the word list to be freed
*/
void freeList(wordList lst);
void printList(wordList lst);

bool isCommand(char *name); 


bool isExt(extList *lst, char *word);

#endif

