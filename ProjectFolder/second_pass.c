#include "second_pass.h"

bool s_pass_err = false;

bool second_pass(FILE *expanded, wordList *commandTable, table_head *label_table, extList *ext_lst, FILE *ent, FILE *ext)
{
    char _line[LINE_LENGTH];
    int lineNum=0;
    int address=0;

	wordNode *comm = commandTable->head;
	rewind(expanded);
    
    while(fgets(_line, LINE_LENGTH, expanded)) /* Read lines until end of file */
    {   
        ++lineNum;
        if (ignored_line(_line) == false) {
            process_line_s_pass(_line, label_table, lineNum, commandTable,ext_lst, ent, ext, &comm, &address);
		}
    }
    return printError(-1, "");
}

/* This function analyzes and extracts information needed for the second pass from a given line */
void process_line_s_pass(char * line, table_head *label_table, int lineNum, wordList *commandTable, extList *ext_lst, FILE *ent, FILE *ext, wordNode **comm, int *address)
{
    int type_instruction;
    symbol_table *symbol_ptr = label_table -> head;
    char *token;
   
    line = remove_left_space(line); /*Removing spaces and tabs until the first word*/
    if(line == NULL || *line == '\0' || *line == '\n') /*If we got an empty line, we will skip to the next line*/ 
        return; 
    str_copy(&token, line);

    if(last_char_in_word(token, ':'))
    { 
        strchrAssembly(&line);  /*if label found skip the label to next token */
        str_copy(&token, line);
    }
    type_instruction = is_data_str_struct_entry(token);
        if (type_instruction == _ENTRY)
        {
			strchrAssembly(&line);
			str_copy(&token, line);
            symbol_ptr = get_label(token, label_table);
            fprintf(ent, "%s ", symbol_ptr->name);
			fprintf(ent, "%s\n", translateNumTo32(symbol_ptr->address));
		}
    else if (isCommand(token))
    {
        *address = fillBlanks(line, *commandTable, comm, label_table, lineNum, *address, *ext_lst, ext);
    }
}


/** This method fills the empty words that generated in the first pass */
int fillBlanks(char *line, wordList commandTable, wordNode **firstWord, table_head *label_table, int lineNum, int address, extList extLst, FILE *extFile) 
{
    cmd_word comm = (*firstWord)->word.word.cmd;    /* assumes firstWord is a cmd command */
    union gen_word genWord;
    char *dest, *src = NULL;
    symbol_table *ptr;
    extent tmp;
    *firstWord = (*firstWord)->next;
	address++;
    strtok(line, " \t\n");  /* skip the command */
 
 
    if (global_table[comm.op_code].destSyntx != NONE)
    {
        dest = strtok(NULL, " \t\n,");
        if (global_table[comm.op_code].srcSyntx != NONE) 
        {
            src = dest;
            dest = strtok(NULL, " \t\n,");
            /*
            analyse src
            */
		        switch(comm.src) 
		        {
		            case 0:
		                genWord.adnum = analysNumber(src + 1);
		                (*firstWord)->word.word = genWord;
		                (*firstWord)->word.type = adress_num;
		                (*firstWord) = (*firstWord)->next;
		                address++;
		                break;
		            case 1:
		                ptr = get_label(src, label_table);
		                tmp = isExt(&extLst, src);
		                if (!ptr)
		                {
		                    if (tmp == ext)
		                    {
		                        genWord.adnum.NUM = 0;
		                        genWord.adnum.ARE = 1;
		                        printExt(extFile, src, address);
		                    }
		                    else
		                    {
		                        printError(lineNum, "Error: non-existing label put as input");
		                    }
		                }
		                else 
		                {
		                    genWord.adnum.NUM = ptr->address;
		                    genWord.adnum.ARE = 2;
		                }
		                

		                (*firstWord)->word.word = genWord;
		                if (tmp != ext)
		                    (*firstWord)->word.type = adress_num;
		                else
		                    (*firstWord)->word.type = ext;

		                (*firstWord) = (*firstWord)->next;
		                address++;
		                break;
		            case 2:
		                src = strtok(src, ".");
		                ptr = get_label(src, label_table);
		                tmp = isExt(&extLst, src);
						
		                if (!ptr) 
		                {
		                    if (tmp == ext) 
		                    {
		                        genWord.adnum.NUM = 0;
		                        genWord.adnum.ARE = 1;
		                        printExt(extFile, src, address);
		                    }
		                    else 
		                    {
		                        printError(lineNum, "Error: non-existing label put as input");
		                    }
		                }
		                else 
		                {
		                    genWord.adnum.NUM = ptr->address;
		                    genWord.adnum.ARE = 2;
		                }
		                (*firstWord)->word.word = genWord;
		                if (tmp != ext)
		                    (*firstWord)->word.type = adress_num;
		                else
		                    (*firstWord)->word.type = ext;

		                (*firstWord) = (*firstWord)->next;
		                address++;
		                src = strtok(NULL, " \t\n.");
		                genWord.adnum = analysNumber(src);
		                (*firstWord)->word.word = genWord;
		                (*firstWord)->word.type = adress_num;
		                (*firstWord) = (*firstWord)->next;
		                address++;
						
		                break;
		            case 3:
						if (comm.dest == 3)
							genWord.reg = analysRegister(dest, src);
						else
							genWord.reg = analysRegister(NULL, src);
		                (*firstWord)->word.word = genWord;
						(*firstWord)->word.type = reg;
		                (*firstWord) = (*firstWord)->next;
		                address++;
		                break;    
		        }	/* end switch */
	        }
        /*
        analyse dest
        */
		    switch(comm.dest) 
		    {
		    
		            case 0:
		                genWord.adnum = analysNumber(dest + 1);
		                (*firstWord)->word.word = genWord;
		                (*firstWord)->word.type = adress_num;
		                (*firstWord) = (*firstWord)->next;
		                address++;
		                break;
		            case 1:
		                ptr = get_label(dest, label_table);
		                tmp = isExt(&extLst, dest);
		                if (!ptr) 
		                {
		                    if (tmp == ext) 
		                    {
		                        genWord.adnum.NUM = 0;
		                        genWord.adnum.ARE = 1;
		                        printExt(extFile, dest, address);
		                    }
		                    else 
		                    {
		                        printError(lineNum, "Error: non-existing label put as input");
		                    }
		                }
		                else 
		                {
		                    genWord.adnum.NUM = ptr->address;
		                    genWord.adnum.ARE = 2;
		                }
		                (*firstWord)->word.word = genWord;
		                if (tmp != ext)
		                    (*firstWord)->word.type = adress_num;
		                else
		                    (*firstWord)->word.type = ext;

		                (*firstWord) = (*firstWord)->next;
		                address++;
		                break;
		            case 2:
		                dest = strtok(dest, ".");

		                ptr = get_label(dest, label_table);
		                tmp = isExt(&extLst, dest);
		                if (!ptr) 
		                {
		                    if (tmp == ext) 
		                    {
		                        genWord.adnum.NUM = 0;
		                        genWord.adnum.ARE = 1;
		                        printExt(extFile, dest, address);
		                    }
		                    else 
		                    {
		                        printError(lineNum, "Error: non-existing label put as input");
		                    }
		                }
		                else 
		                {
		                    genWord.adnum.NUM = ptr->address;
		                    genWord.adnum.ARE = 2;
		                }
		                (*firstWord)->word.word = genWord;
		                if (tmp != ext)
		                    (*firstWord)->word.type = adress_num;
		                else
		                    (*firstWord)->word.type = ext;

		                (*firstWord) = (*firstWord)->next;
		                address++;
		                dest = strtok(NULL, ". \t\n");
		                genWord.adnum = analysNumber(dest);
		                (*firstWord)->word.word = genWord;
		                (*firstWord)->word.type = adress_num;
		                (*firstWord) = (*firstWord)->next;
		                address++;
		                break;
		            case 3:
		                if (comm.src != 3) {
		                    genWord.reg = analysRegister(dest, NULL);
		                    (*firstWord)->word.word = genWord;
		                    (*firstWord)->word.type = reg;
		                    (*firstWord) = (*firstWord)->next;
		                    address++;
		                } 
		                break;    
		    } /* end switch*/
	    }
    return address;
}
