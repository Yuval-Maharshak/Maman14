#include "first_pass.h"

extList extlst = {NULL};

/* The method gets a file after pre assembler stage and checks each line in the file
* by sending the values to process_line_f_pass. */
bool first_pass(FILE *src, wordList *commandTable, wordList *dataTable, table_head *label_head, extList *extlst)
{
	char *line = (char *) malloc(sizeof(char) * LINE_LENGTH);
    int  lineNum=0;


    while (fgets(line, LINE_LENGTH , src))
    {   
        ++lineNum;
        if (ignored_line(line) == false) {
            process_line_f_pass(line, label_head, lineNum, commandTable, dataTable, extlst);
        }
    }
    update_data_nodes(label_head, IC);

    return printError(-1, "");
} 


/* The method gets a line and analyze the data of the line */
void process_line_f_pass(char *line, table_head *label_head, int lineNum, wordList *commandTable, wordList *dataTable, extList *extlst)
{
    char tmp_line[LINE_LENGTH];
    char label_name[LABLE_LENGTH];
    bool label_flag = false;
    int dataType;
    char *word;
    str_copy(&word, line);    /* copy a single word from line */
    strcpy(tmp_line, line);

    if (last_char_in_word(word, ':'))   /* checks if it's a label */
    {
        word[strlen(word)-1] = '\0';
            if (valid_lable_syntax(word))
            {
                label_flag = true;
                strcpy(label_name, word);
                strchrAssembly(&line);  /** moving to next word */
                str_copy(&word, line);
                if (word == NULL || *word == '\0' || *word == '\n') 
                {
                    printError(lineNum,"Error: no command or instruction after label\n");
                }
            }    
            else    /* invalid word */
            {
                printError(lineNum, "Error: invalid label name in\n");
            }               
    }
    dataType = is_data_str_struct_entry(word);
    
    if (isCommand(word))
    {
        if (label_flag)
            add_only_label_symbol(label_head, label_name, &IC, &DC, is_code , lineNum);
        handle_command(line, lineNum, commandTable);
    }
    else if (dataType < 5)
    {
        if (dataType < 3) {	/* .data/.string/.struct */
            if (label_flag)
                add_only_label_symbol(label_head, label_name, &IC, &DC, is_data , lineNum);
            handle_data_str_struct(line, dataType, lineNum, dataTable);
        }
        else /* if it's .entry or .extern */
        {
            if (label_flag)
                printf("Warning: %s after label in line %d\n", ((dataType == _ENTRY) ? ".entry":".extern"), lineNum);
            strchrAssembly(&line);
            str_copy(&word, line);
            insertExt(extlst, word, ((dataType == _ENTRY) ? ent:ext));
        }
    }
    else
    {
        printError(lineNum, "Error: unknown command or instruction\n");
    }    
}

/* The method receives a line, a line number and a pointer to the word table
* and parses the command. */
void handle_command(char *line, int line_num, wordList *commandTable)
{
    union gen_word tempGenWord;
    Word tempWord;
    cmd_word *tempCmd;
    int tempIC;

    tempIC = IC;
    tempCmd = analysCommand(line, line_num);
    tempGenWord.cmd = *tempCmd;
    tempWord.word = tempGenWord;
    tempWord.type = cmd;
    insert(commandTable, tempWord);
    insertEmpty(commandTable, IC - tempIC - 1);
}

/* The method analyzes the addressing type,depending on the addressing method, 
* inserts the values ​​into the data table */
void handle_data_str_struct(char *name, int data_type, int line_num, wordList *dataTable)
{
    char *tmp_line = name;
    int dataLength;
    data_word **tmpData; /*can't have data bigger than line length */

    if (data_type < 3)   /* if the second word is .data or .string or .struct */ 
    {
        switch(data_type) 
        {
        case _DATA:
            tmpData = analysData(tmp_line, &dataLength, line_num);
            break;
        case _STRING:
            tmpData = analysString(tmp_line, &dataLength, true, line_num);
            break;
        case _STRUCT:
            tmpData = analysStruct(tmp_line, &dataLength, line_num);
            break;
        }
        insertData(dataTable, tmpData, dataLength);
    }    
    else    /* .entry or .extern */ 
        return;
}


/** This method updates all data nodes addresses */
void update_data_nodes(table_head *_head, unsigned int IC)
{
    symbol_table *node = NULL;
    node = _head -> head;   /** now node pointes to the head of the symbol table */
    while (node)
    {
        if (node -> type == is_data) {
            node -> address = node -> address + IC;
        }
        node->address = node->address + 100;
        node = node -> next;    
    }
}

/** This method insert the data into the data table */
void insertData(wordList *dataTable, data_word **dataToInsert, int length) 
{
    int i;
    Word tmpWord;
    for (i=0; i<length; i++) 
    {
        tmpWord.type = data;
        tmpWord.word.data.DATA = dataToInsert[i]->DATA;
        insert(dataTable, tmpWord);
    }
    DC += length;
}


/** checks if the struct has correct parameters and send a pointer of data  to caller  */
data_word ** analysStruct(char *line, int *length, int lineNum) 
{
    char *word, *end, *tmp_line = (char *) malloc(sizeof(char) * strlen(line));
    int i=0, n;
    data_word ** data = (data_word **) malloc(sizeof(data_word *));
    strchrAssembly(&line);
    strcpy(tmp_line, line);
    word = strtok(line, ",");
    failMalloc(data)

    n = strtol(word, &end, BASE);
    if (end == word)
        printError(lineNum, "Error: first argument at .struct is not a number");

    data[0] = (data_word *) malloc(sizeof(data_word));
    data[0]->DATA = n;
    tmp_line++;     /* skip first number */
    while(isspace(*tmp_line) || *tmp_line == ',')       /* skip spaces and , to reach string */
        tmp_line++;
    
    if (tmp_line[i++] == '"') 
    {
        while(tmp_line[i] != '"' && tmp_line[i] != '\n') 
        {
            data[i] = (data_word *) malloc(sizeof(data_word));
            data[i]->DATA = (unsigned int) tmp_line[i];
            data = (data_word **) realloc(data, sizeof(data_word *) * (i+1));
            failMalloc(data)
            i++;
        }
        data[i-1] = (data_word *) malloc(sizeof(data_word));
        data[i-1]->DATA = (unsigned int) 0;
        data = (data_word **) realloc(data, sizeof(data_word *) * (i+1));
        failMalloc(data)
        if (tmp_line[i] == '\n') 
        {
            printError(lineNum, "Error: string at .string doesn't ends with \"");
        }
    }
    else
        printError(lineNum, "Error: string at .string doesn't starts with \"");
    *length = i;
    return data;
}


/** analyze the string and if the string is a valid string the method
 * send a pointer that points into the string to insert to data table
 */
data_word ** analysString(char *line, int *length, bool skip, int lineNum) 
{
    int i = 0;
    data_word **data = (data_word **) malloc(sizeof(data_word *));
    if (skip)
        strchrAssembly(&line);
    failMalloc(data)

    if (line[i++] == '"') 
    {
        while(line[i] != '"' && line[i] != '\n') 
        {
            data[i-1] = (data_word *) malloc(sizeof(data_word));
            data[i-1]->DATA = (unsigned int) line[i];
            data = (data_word **) realloc(data, sizeof(data_word *) * (i+1));
            failMalloc(data)
            i++;
        }
        data[i-1] = (data_word *) malloc(sizeof(data_word));
        data[i-1]->DATA = (unsigned int) 0;
        data = (data_word **) realloc(data, sizeof(data_word *) * (i+1));
        failMalloc(data)
        if (line[i] == '\n') 
        {
            printError(lineNum, "Error: string at .string doesn't ends with \"");
        }
    }
    else
        printError(lineNum, "Error: string at .string doesn't starts with \"");

    *length = i;
    return data;
} 


/** analyze the data and if the data is a valid data, the method
 * send a pointer that points into the data to insert to data table
 */
data_word **analysData(char *line, int *length, int lineNum) 
{
    char *word, *end;
    int n, i=1;
    data_word **data = (data_word **) malloc(sizeof(data_word *));
    strtok(line, " \t\n");
    failMalloc(data)

    while ((word = strtok(NULL, ","))) 
    {
        while(isspace(word[0]))
            word++;
        n = strtol(word, &end, BASE); 
        if (end == word) 
            printError(lineNum, "Error: non-integer passed to .data");
        data[i-1] = (data_word *) malloc(sizeof(data_word));
        data[i-1]->DATA = n;
        data = (data_word **) realloc(data, sizeof(data_word *) * (i+1));
        failMalloc(data)
        i++;
    }
    *length = i-1;
    return data;
}
