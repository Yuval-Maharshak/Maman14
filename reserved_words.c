char *two_operands_cmd[] = {
                            "mov",
                            "cmp",
                            "add",
                            "sub",
                            "lea"
                          };


char *one_operands_cmd[] = {
                            "not",
                            "clr",
                            "inc",
                            "dec",
                            "jmp",
                            "bne",
                            "get",
                            "prn",
                            "jsr"
                          };



char *zero_operands_cmd[] = {
                            "rts",
                            "hlt"
                          };


char *regs_list[] = {
                     "r0",
                     "r1",
                     "r2",
                     "r3",
                     "r4",
                     "r5",
                     "r6",
                     "r7"
                    };

char *instruction_type[] = {
                            ".data",
                            ".string",
                            ".struct",
                            ".entry",
                            ".extern"
}                    