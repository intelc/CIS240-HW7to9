#include <stdlib.h>
#include <string.h>
#include "token.h"
#include <ctype.h>

// line_number : You can use this to keep track of the current line number which can be useful in reporting syntax errors.
int line_number = 0;
int unique_number = 0;
int tokenTypeSwitch(token *theToken, char *bufferToken)
{
    theToken->literal_value = -1;
    theToken->arg_no = -1;
    if (strcmp(bufferToken, "defun") == 0)
    {
        theToken->type = DEFUN;
    }
    else if (strcmp(bufferToken, "return") == 0)
    {
        theToken->type = RETURN;
    }
    else if (strcmp(bufferToken, "+") == 0)
    {
        theToken->type = PLUS;
    }
    else if (strcmp(bufferToken, "-") == 0)
    {
        theToken->type = MINUS;
    }
    else if (strcmp(bufferToken, "*") == 0)
    {
        theToken->type = MUL;
    }
    else if (strcmp(bufferToken, "/") == 0)
    {
        theToken->type = DIV;
    }
    else if (strcmp(bufferToken, "%") == 0)
    {
        theToken->type = MOD;
    }
    else if (strcmp(bufferToken, "and") == 0)
    {
        theToken->type = AND;
    }
    else if (strcmp(bufferToken, "or") == 0)
    {
        theToken->type = OR;
    }
    else if (strcmp(bufferToken, "not") == 0)
    {
        theToken->type = NOT;
    }
    else if (strcmp(bufferToken, "lt") == 0)
    {
        theToken->type = LT;
    }
    else if (strcmp(bufferToken, "le") == 0)
    {
        theToken->type = LE;
    }
    else if (strcmp(bufferToken, "eq") == 0)
    {
        theToken->type = EQ;
    }
    else if (strcmp(bufferToken, "ge") == 0)
    {
        theToken->type = GE;
    }
    else if (strcmp(bufferToken, "gt") == 0)
    {
        theToken->type = GT;
    }
    else if (strcmp(bufferToken, "if") == 0)
    {
        theToken->type = IF;
    }
    else if (strcmp(bufferToken, "else") == 0)
    {
        theToken->type = ELSE;
    }
    else if (strcmp(bufferToken, "endif") == 0)
    {
        theToken->type = ENDIF;
    }
    else if (strcmp(bufferToken, "drop") == 0)
    {
        theToken->type = DROP;
    }
    else if (strcmp(bufferToken, "dup") == 0)
    {
        theToken->type = DUP;
    }
    else if (strcmp(bufferToken, "swap") == 0)
    {
        theToken->type = SWAP;
    }
    else if (strcmp(bufferToken, "rot") == 0)
    {
        theToken->type = ROT;
    }
    else if (strncmp(bufferToken, "arg", 3) == 0)
    {
        theToken->type = ARG;
        int *t;
        t = (int *)malloc(1 * sizeof(int));
        *t = 0;
        sscanf(bufferToken, "arg%u", t);
        theToken->arg_no = *t;
        free(t);
    }
    else if (strncmp(bufferToken, "-", 1) == 0)
    {
        if (isdigit(bufferToken[1]))
        {
            theToken->type = LITERAL;
            int *t;
            t = (int *)malloc(1 * sizeof(int));
            *t = 0;
            sscanf(bufferToken, "%d", t);
            theToken->literal_value = *t;
            free(t);
        }
        else
        {
            theToken->type = BROKEN_TOKEN;
        }
    }
    else if (strncmp(bufferToken, "0x", 2) == 0)
    {
        theToken->type = LITERAL;
        int *t;
        t = (int *)malloc(1 * sizeof(int));
        *t = 0;
        sscanf(bufferToken, "%x", t);
        theToken->literal_value = *t;
        free(t);
    }
    else if (isdigit(bufferToken[0]))
    {
        theToken->type = LITERAL;
        int *t;

        t = (int *)malloc(1 * sizeof(int));
        *t = 0;
        sscanf(bufferToken, "%d", t);
        theToken->literal_value = *t;
        free(t);
    }
    else if (strcmp(bufferToken, " ") == 0)
    {
        theToken->type = BROKEN_TOKEN;
    }
    else
    {
        theToken->type = IDENT;
    }
    //printf("bt after: %s \n",bufferToken);
    strcpy(theToken->str, bufferToken);
    return 0;
}

int read_token(token *theToken, FILE *theFile)
{
    if (feof(theFile))
    {
        return -1;
    }
    char c = -1;
    //c = fgetc(theFile);
    int errorFlag = 0;
    char *bufferToken;
    bufferToken = (char *)calloc(251, sizeof(char));
    bufferToken[0] = '\0';

    while (1)
    {

        if (!isalpha(c))
        {
            if (c == '\n')
            {
                line_number++;
                printf("%d->\n", line_number);
            }
            c = fgetc(theFile);
        }
        bufferToken[0] = '\0';
        if (feof(theFile))
        {

            line_number++;

            free(bufferToken);
            return -2;
        }
        // bufferToken = "";
        //printf("token is: %c",c);
        while ((c == ' ') || (c == '\t'))
        { //if contains a space then keep going
            c = fgetc(theFile);
            if (c == '\n')
            {

                continue;
            }
        }
        if (c == ';')
        {
            while (c != '\n')
            {
                c = fgetc(theFile);
            } //move the cursor to the end of line(next up is new line)

            continue;
        }
        if (c == '\n' || c == '\r')
        {

            continue;
        }
        //c = fgetc(theFile);
        //strncat(bufferToken,&c,1);

        while (c != ' ' && c != '\t' && c != '\n' && c != '\r' && !feof(theFile))
        {                                //move the cursor to the next seperate
            strncat(bufferToken, &c, 1); // the token is now in the buffer
            c = fgetc(theFile);
            if (feof(theFile))
            {
                line_number++;
                errorFlag = 0;
                //return errorFlag;
                break;
            }
            if (c == '\n')
            {
                line_number++;
                printf("%d->\n", line_number);
            }
        }
        
        tokenTypeSwitch(theToken, bufferToken); //********IMPLEMENT
        printf("bt: %s type: %s literal %d\n",bufferToken,token_type_to_string(theToken->type),theToken->literal_value);
        //strcpy(theToken->str,  bufferToken);

        break;
        //handle error cases
    }
    free(bufferToken);
    return errorFlag;
}

// Extra functions which you may wish to define and use , or not
const char *token_type_to_string(int type)
{
    if (type == DEFUN)
    {
        return "defun";
    }
    else if (type == IDENT)
    {
        return "ident";
    }
    else if (type == RETURN)
    {
        return "return";
    }
    else if (type == PLUS)
    {
        return "plus";
    }
    else if (type == MINUS)
    {
        return "minus";
    }
    else if (type == MUL)
    {
        return "mul";
    }
    else if (type == DIV)
    {
        return "div";
    }
    else if (type == MOD)
    {
        return "mod";
    }
    else if (type == AND)
    {
        return "and";
    }
    else if (type == OR)
    {
        return "or";
    }
    else if (type == NOT)
    {
        return "not";
    }
    else if (type == LT)
    {
        return "lt";
    }
    else if (type == LE)
    {
        return "le";
    }
    else if (type == EQ)
    {
        return "eq";
    }
    else if (type == GE)
    {
        return "ge";
    }
    else if (type == GT)
    {
        return "gt";
    }
    else if (type == IF)
    {
        return "if";
    }
    else if (type == ELSE)
    {
        return "else";
    }
    else if (type == ENDIF)
    {
        return "endif";
    }
    else if (type == DROP)
    {
        return "drop";
    }
    else if (type == DUP)
    {
        return "dup";
    }
    else if (type == SWAP)
    {
        return "swap";
    }
    else if (type == ROT)
    {
        return "rot";
    }
    else if (type == ARG)
    {
        return "arg";
    }
    else if (type == LITERAL)
    {
        return "literal";
    }
    else if (type == BROKEN_TOKEN)
    {
        return "broken_token";
    }
    return "error";
}

//void print_token (token *theToken);