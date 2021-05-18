#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
//#include "token.h"
#include "asm_magic.h"
void strip_ext(char *fname)
{
    char *end = fname + strlen(fname);

    while (end > fname && *end != '.')
    {
        --end;
    }

    if (end > fname)
    {
        *end = '\0';
    }
}
int main(int argc, char **argv)
{
    printf("%s\n",argv[1]);
    //read
    FILE *fin;
    fin = fopen(argv[1], "r");
    if (fin == NULL)
    {
        printf("Could not open source file\n");
        exit(1);
    }

    //write
    FILE *fout;
    char *tempArgv = (char *)calloc(200, sizeof(char));
    char *tempArgv2 = (char *)calloc(200, sizeof(char));

    strcat(tempArgv, argv[1]);
    strcat(tempArgv2, argv[1]);
    //printf("%s",tempArgv);
    //strcat(tempArgv,".asm");

    strip_ext(tempArgv);
    strip_ext(tempArgv2);
    strcat(tempArgv2, ".asm\0");
    printf("%s\n", tempArgv2);
    fout = fopen(tempArgv2, "w");

    token *currToken;
    currToken = (token *)malloc(1 * sizeof(token));

    while (read_token(currToken, fin) == 0)
    { // info is now stored into the token

       // printf("string is: %s %s %d arg%d\n",currToken->str, token_type_to_string(currToken->type),currToken->literal_value,currToken->arg_no);
        //printf("string is: %s \n",currToken->str);
        if (currToken->type == DEFUN)
        {

            read_token(currToken, fin); //get IDENT
            if (currToken->type == IDENT)
            {
                char *funcName = currToken->str;
                insertHeader(fout, funcName);
            }
            else
            {
                free(currToken);
                free(tempArgv);
                free(tempArgv2);
                exit(1);
            }
            insertPrologue(fout);

            while (currToken->type != RETURN)
            {
                read_token(currToken, fin); //read a Token
                if (currToken->type == IF)
                {
                    processIf(currToken, fin, fout, tempArgv);
                }
                else if (currToken->type == RETURN)
                {
                    processReturn(currToken, fout);
                }
                else
                {
                    processOther(currToken, fout);
                }
            }
            //we have processed a return
        }

        //fprintf (fout, "hello");
        //reset currToken
    }

    fclose(fin);
    fclose(fout);
    free(tempArgv);
    free(currToken);
    free(tempArgv2);
    printf("line:%d", line_number);
    return 0;
}