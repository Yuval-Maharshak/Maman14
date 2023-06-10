#include "funcs.h"

int searchCommand(char *command) 
{
    int i;
    for (i=0; i<16; i++) 
    {
        if (strcmp(global_table[i].cmd, command) == 0)
            return i;
    }
    return -1;
}


cmd_word *analysCommand(char *line, int lineNum) 
{ 
    char *command, *src="0", *dest="0", *token = " \t\n", *seperator = " \t\n,";
    int i;
    cmd_table cmd;
    cmd_word *cmdword = (cmd_word *) malloc(sizeof(cmdword));
    int L = 1;      /* the number of words the command is translated to */

    failMalloc(cmdword)
    
    command = strtok(line, token);     /* the command is the first word of line    */
    i = searchCommand(command); /* searching the global_table with all commands for the first word  */
    if (i == -1) 
    {
        printError(lineNum, "Error: first word %s is not a command", command);
        return NULL;
    }
    cmd = global_table[i];

    /* the default values of the word */
    cmdword->ARE = 0;
    cmdword->op_code = cmd.op_code;
    cmdword->src = 0;
    cmdword->dest = 0;
    
    /* inputting src and dest */
    if (cmd.srcSyntx != NONE && cmd.destSyntx != NONE) 
    {
        src = strtok(NULL, seperator);
        dest = strtok(NULL, seperator);
    }
    else if (cmd.destSyntx != NONE) 
    {
        dest = strtok(NULL, token);
        if (strtok(NULL, seperator)) 
        {
            printError(lineNum, "Error: input not compatible(the command %s expected 1 argument and 						got 2)", command);
            return NULL;
        }
    }
    else if (strtok(NULL, token)) 
    {
        printError(lineNum, "Error: input not compatible (the command %s expected 0 arguments and got 						more than 0)", command);
        return NULL;
    }
    if (!src || !dest) 
    {        /* checks if any of the strtok failed */
        printError(lineNum, "Error: input not compatible (wrong number of arguments)");
        return NULL;
    }

    if (cmd.destSyntx != NONE) 
    {        /* if there should be a first operand */
        L++;        /* that means we need to translate at least two words */

        if (isNum(dest) && cmd.destSyntx != NOT_CONSTANT)
            cmdword->dest = 0;
        else if (isLabel(dest)) 
        {
            cmdword->dest = 1;
        }
        else if (isStructLabel(dest)) 
        {
            cmdword->dest = 2;
            L++;        /* need to add num of field word */
        }
        else if (isRegister(dest))
            cmdword->dest = 3;
        else 
        {
            printError(lineNum, "Error: input not compatible (dest argument isn't legal)");
            return NULL;            /* not sure if will work */
        }
        
        if (cmd.srcSyntx != NONE) 
        {         /* if src operand is needed */
            L++;
            if (isNum(src) && cmd.srcSyntx != LABEL)
                cmdword->src = 0;
            else if (isLabel(src))
                cmdword->src = 1;
            else if (isStructLabel(src)) 
            {
                cmdword->src = 2;
                L++;
            }
            else if (isRegister(src) && cmd.srcSyntx != LABEL)  
            {
                cmdword->src = 3;
                if (cmdword->dest == 3)
                    L--;
            }
            else 
            {
                printError(lineNum, "Error: input isn't compatible with command (src argument isn't 							legal)");
                return NULL;     /* again not sure */
            }
        }
    }
    IC += L;
    return cmdword;
}


adress_num_word analysNumber(char *num) 
{
    adress_num_word res;
    char *end;
    res.NUM = strtol(num, &end, BASE);
    res.ARE = 0;
    return res;
}



reg_word analysRegister(char *dest, char *src) 
{
    reg_word rg;

    rg.ARE = 0;
    rg.destReg = 0;
    rg.srcReg = 0;
    if (dest)
        rg.destReg = (int) (dest[1]-'0');
    if (src)
        rg.srcReg = (int) (src[1] - '0');
    return rg;
}
