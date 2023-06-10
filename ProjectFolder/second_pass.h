#include "constants.h"
#include "utils.h"
#include "head.h"
#include "str_methods.h"
#include "symbol_table.h"
#include "first_line_check.h"
#include "funcs.h"
#include "translate.h"
#include <string.h>

/**
* The method gets the file after first pass and fill the empty
* spaces in the source file
* @param expanded Surce file
* @param commandTable Points to the command table
* @param label_head Points to the head of the symbol table
* @param extlst Points to the head of the extern table
* @param ent Entry file
* @param ext Extern file
* @return true if success else -1
*/
bool second_pass(FILE *expanded, wordList *commandTable, table_head *label_table, extList *ext_lst, FILE *ent, FILE *ext);


/**
* process a single source line in the second pass
* @param line line
* @param label_head Points to the head of the symbol table
* @param lineNum Line num in source file
* @param commandTable Points to the command table
* @param extlst Points to the head of the extern table
* @param ent Entry file
* @param ext Extern file
* @param comm points to command table
* @param address 
*/
void process_line_s_pass(char * line, table_head *label_table, int lineNum, wordList *commandTable, extList *ext_lst, FILE *ent, FILE *ext, wordNode **comm, int *address);

/**
* fill in the blanks in the command table based on the source file on every line. it also prints to extFile all the instance of extern.
* @param line the line we're filling
* @param commandTable the command table
* @param firstWord the first word, a pointer moving forward across the command table until the end.
* @param label_table the table of labels defined
* @param lineNum Line num in source file
* @param address the current address we're in (in terms of the command table)
* @param extLst the list of extern labels defined.
* @param extFile the file {fileName}.ext in which we prints all the instance of extern in the code.
 */
int fillBlanks(char *line, wordList commandTable, wordNode **firstWord, table_head *label_table, int lineNum, int address, extList extLst, FILE *extFile);