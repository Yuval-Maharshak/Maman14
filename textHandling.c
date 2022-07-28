#include "macro/macroExpansion.h"
#include "reserved_words.c"

bool legalMacro(char *name) {       /* add to the paramaters macroArr */
    /*
        illigal names: assembly's commands, starting numbers, already in macroArr, 
    */
    if (!name)
        return false;
    if (reservedWord(name))
        return false;
    if (isdigit(name[0]))
        return false;
    return true;
}
bool reservedWord(char *name) {
    /*fd*/
    return in(name, two_operands_cmd,5) || in(name, one_operands_cmd,9) || in(name, zero_operands_cmd,2) || in(name, regs_list,8) || in(name, instruction_type,5) || in(name, macroRelated, 2);
}

bool in(char *name, char *lst[], int length) {
    for (; length > 0; length--) {
        if (strcmp(name, lst[length-1]) == 0)
            return true;
    }
    return false;
}
