#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "token.h"

int insertHeader(FILE *theFile, char *name)
{
    fprintf(theFile, ";;;;;;;;;;;;;;;;;;;;;;;;;;;;%s;;;;;;;;;;;;;;;;;;;;;;;;;;;;\n\t\t.CODE\n\t\t.FALIGN\n%s\n", name, name);
    return 0;
}
int insertPrologue(FILE *theFile)
{
    fprintf(theFile, "\t;; prologue\n");
    // fprintf (theFile, "\tSTR R7, R6, #-2	;; save return address\n");
    // fprintf (theFile, "\tSTR R5, R6, #-3	;; save base pointer\n");
    // fprintf (theFile, "\tADD R6, R6, #-3\n");
    // fprintf (theFile, "\tADD R5, R6, #0\n");
    fprintf(theFile, "\tADD R6 R6 #-3\n\tSTR R7 R6 #1\n\tSTR R5 R6 #0\n\tADD R5 R6 #0\n");
    fprintf(theFile, "\t;; function body\n");
    return 0;
}
int processOther(token *theToken, FILE *theFile)
{
    if (theToken->type == LITERAL)
    {
        if (theToken->literal_value <= 255)
        {
            fprintf(theFile, "\tCONST R7, #%d \n", theToken->literal_value);
        }
        else
        {
            int high = theToken->literal_value >> 8;
            int low = ((unsigned short)(theToken->literal_value << 8)) >> 8;
            fprintf(theFile, "\tCONST R7, #%d \n", low);
            fprintf(theFile, "\tHICONST R7, #%d \n", high);
        }

        fprintf(theFile, "\tSTR R7, R6, #-1 \n");
        fprintf(theFile, "\tADD R6, R6, #-1 \n");
    }
    else if (theToken->type == ARG)
    {
        fprintf(theFile, "\tLDR R7, R5, #%d \n", theToken->arg_no + 2);
        fprintf(theFile, "\tSTR R7, R6, #-1 \n");
        fprintf(theFile, "\tADD R6, R6, #-1 \n");
    }
    else if (theToken->type == PLUS)
    {
        fprintf(theFile, "\tLDR R1, R6, #0 \n");
        fprintf(theFile, "\tLDR R2, R6, #1 \n");
        fprintf(theFile, "\tADD R7, R1, R2 \n");
        fprintf(theFile, "\tADD R6, R6, #1 \n");
        fprintf(theFile, "\tSTR R7, R6, #0 \n");
    }
    else if (theToken->type == MINUS)
    {
        fprintf(theFile, "\tLDR R1, R6, #0 \n");
        fprintf(theFile, "\tLDR R2, R6, #1 \n");
        fprintf(theFile, "\tSUB R7, R1, R2 \n");
        fprintf(theFile, "\tADD R6, R6, #1 \n");
        fprintf(theFile, "\tSTR R7, R6, #0 \n");
    }
    else if (theToken->type == MUL)
    {
        fprintf(theFile, "\tLDR R1, R6, #0 \n");
        fprintf(theFile, "\tLDR R2, R6, #1 \n");
        fprintf(theFile, "\tMUL R7, R1, R2 \n");
        fprintf(theFile, "\tADD R6, R6, #1 \n");
        fprintf(theFile, "\tSTR R7, R6, #0 \n");
    }
    else if (theToken->type == DIV)
    {
        fprintf(theFile, "\tLDR R1, R6, #0 \n");
        fprintf(theFile, "\tLDR R2, R6, #1 \n");
        fprintf(theFile, "\tDIV R7, R1, R2 \n");
        fprintf(theFile, "\tADD R6, R6, #1 \n");
        fprintf(theFile, "\tSTR R7, R6, #0 \n");
    }
    else if (theToken->type == MOD)
    {
        fprintf(theFile, "\tLDR R1, R6, #0 \n");
        fprintf(theFile, "\tLDR R2, R6, #1 \n");
        fprintf(theFile, "\tMOD R7, R1, R2 \n");
        fprintf(theFile, "\tADD R6, R6, #1 \n");
        fprintf(theFile, "\tSTR R7, R6, #0 \n");
    }
    else if (theToken->type == ROT)
    {
        fprintf(theFile, "\tLDR R1, R6, #0 \n");
        fprintf(theFile, "\tLDR R2, R6, #1 \n");
        fprintf(theFile, "\tLDR R3, R6, #2 \n");
        fprintf(theFile, "\tSTR R1, R6, #1 \n");
        fprintf(theFile, "\tSTR R2, R6, #2 \n");
        fprintf(theFile, "\tSTR R3, R6, #0 \n");
        
    }
    else if (theToken->type == IDENT)
    {
        fprintf(theFile, "\tJSR %s\n", theToken->str);
        //fprintf(theFile, "\tADD R6, R6, #-1 \n");
    }
    else if (theToken->type == DROP)
    {
        fprintf(theFile, "\tADD R6, R6, #1 \n");
    }
    else if (theToken->type == DUP)
    {
        fprintf(theFile, "\tLDR R7, R6, #0 \n");
        fprintf(theFile, "\tSTR R7, R6, #-1 \n");
        fprintf(theFile, "\tADD R6, R6, #-1 \n");
    }
    else if (theToken->type == SWAP)
    {
        fprintf(theFile, "\tLDR R1, R6, #0 \n");
        fprintf(theFile, "\tLDR R2, R6, #1 \n");
        fprintf(theFile, "\tSTR R1, R6, #1 \n");
        fprintf(theFile, "\tSTR R2, R6, #0 \n");
    }
    else if (theToken->type == GE)
    {
        fprintf(theFile, "\tLDR R1, R6, #0 \n");

        fprintf(theFile, "\tCMP R1, R2 \n");
        fprintf(theFile, "\tBRn L%d_NEG\n", line_number);
        fprintf(theFile, "\tCONST R7, #1;\n");
        fprintf(theFile, "\tJMP L%d_END\n", line_number);
        fprintf(theFile, "L%d_NEG\n", line_number);
        fprintf(theFile, "\tCONST R7, #0\n");
        fprintf(theFile, "L%d_END\n", line_number);
        fprintf(theFile, "\tADD R6, R6, #1 \n");
        fprintf(theFile, "\tSTR R7, R6, #0 \n");
    }
    else if (theToken->type == LT)
    {
        fprintf(theFile, "\tLDR R1, R6, #0 \n");
        fprintf(theFile, "\tLDR R2, R6, #1 \n");

        fprintf(theFile, "\tCMP R1, R2 \n");
        fprintf(theFile, "\tBRnp L%d_NEG\n", line_number);
        fprintf(theFile, "\tCONST R7, #1\n");
        fprintf(theFile, "\tJMP L%d_END\n", line_number);
        fprintf(theFile, "L%d_NEG      \n", line_number);
        fprintf(theFile, "\tCONST R7, #0\n");
        fprintf(theFile, "L%d_END\n", line_number);
        fprintf(theFile, "\tADD R6, R6, #1 \n");
        fprintf(theFile, "\tSTR R7, R6, #0 \n");
    }
    else if (theToken->type == LE)
    {
        fprintf(theFile, "\tLDR R1, R6, #0 \n");
        fprintf(theFile, "\tLDR R2, R6, #1 \n");

        fprintf(theFile, "\tCMP R1, R2 \n");
        fprintf(theFile, "\tBRp L%d_NEG\n", line_number);
        fprintf(theFile, "\tCONST R7, #1\n");
        fprintf(theFile, "\tJMP L%d_END\n", line_number);
        fprintf(theFile, "L%d_NEG      \n", line_number);
        fprintf(theFile, "\tCONST R7, #0\n");
        fprintf(theFile, "L%d_END\n", line_number);
        fprintf(theFile, "\tADD R6, R6, #1 \n");
        fprintf(theFile, "\tSTR R7, R6, #0 \n");
    }
    else if (theToken->type == EQ)
    {
        fprintf(theFile, "\tLDR R1, R6, #0 \n");
        fprintf(theFile, "\tLDR R2, R6, #1 \n");

        fprintf(theFile, "\tCMP R1, R2 \n");
        fprintf(theFile, "\tBRnp L%d_NEG\n", line_number);
        fprintf(theFile, "\tCONST R7, #1\n");
        fprintf(theFile, "\tJMP L%d_END\n", line_number);
        fprintf(theFile, "L%d_NEG      \n", line_number);
        fprintf(theFile, "\tCONST R7, #0\n");
        fprintf(theFile, "L%d_END\n", line_number);
        fprintf(theFile, "\tADD R6, R6, #1 \n");
        fprintf(theFile, "\tSTR R7, R6, #0 \n");
    }
    else if (theToken->type == GT)
    {
        fprintf(theFile, "\tLDR R1, R6, #0 \n");
        fprintf(theFile, "\tLDR R2, R6, #1 \n");

        fprintf(theFile, "\tCMP R1, R2 \n");
        fprintf(theFile, "\tBRnz L%d_NEG\n", line_number);
        fprintf(theFile, "\tCONST R7, #1\n");
        fprintf(theFile, "\tJMP L%d_END\n", line_number);
        fprintf(theFile, "L%d_NEG      \n", line_number);
        fprintf(theFile, "\tCONST R7, #0\n");
        fprintf(theFile, "L%d_END\n", line_number);
        fprintf(theFile, "\tADD R6, R6, #1 \n");
        fprintf(theFile, "\tSTR R7, R6, #0 \n");
    }
    else if (theToken->type == AND)
    {
        fprintf(theFile, "\tLDR R1, R6, #0 \n");
        fprintf(theFile, "\tLDR R2, R6, #1 \n");
        fprintf(theFile, "\tAND R7, R1, R2 \n");
        fprintf(theFile, "\tADD R6, R6, #1 \n");
        fprintf(theFile, "\tSTR R7, R6, #0 \n");
    }
    else if (theToken->type == OR)
    {
        fprintf(theFile, "\tLDR R1, R6, #0 \n");
        fprintf(theFile, "\tLDR R2, R6, #1 \n");
        fprintf(theFile, "\tOR R7, R1, R2 \n");
        fprintf(theFile, "\tADD R6, R6, #1 \n");
        fprintf(theFile, "\tSTR R7, R6, #0 \n");
    }
    else if (theToken->type == NOT)
    {
        fprintf(theFile, "\tLDR R1, R6, #0 \n");

        fprintf(theFile, "\tNOT R7, R1, R2 \n");

        fprintf(theFile, "\tSTR R7, R6, #0 \n");
    }

    return 0;
}
int processIf(token *theToken, FILE *fin, FILE *fout, char *name)
{   unique_number++;
    int uid = line_number;
    int saveNum = unique_number;
    
    fprintf(fout, "\tLDR R7, R6, #0 \n");
    fprintf(fout, "\tCONST R1, #0 \n");
    fprintf(fout, "\tADD R6, R6, #1 \n");
    fprintf(fout, "\tCMP R7, R1 \n");
    fprintf(fout, "\tBRz L%d_%d_%s_ELSE \n", uid, saveNum, name);
    while (theToken->type != ELSE)
    {
        read_token(theToken, fin); //read a Token
        if (theToken->type == IF)
        {
            processIf(theToken, fin, fout, name);
        }
        else if (theToken->type == ELSE)
        {
            break;
        }
        else if (theToken->type == ENDIF)
        {
            break;
        }
        else
        {
            processOther(theToken, fout);
        }
    }
    if (theToken->type == ELSE)
    {
        fprintf(fout, "\tJMP L%d_%d_%s_ENDIF\n", uid,saveNum, name);
        fprintf(fout, "L%d_%d_%s_ELSE\n", uid, saveNum,name);
        while (theToken->type != ENDIF)
        {
            read_token(theToken, fin); //read a Token
            if (theToken->type == IF)
            {
                processIf(theToken, fin, fout, name);
            }
            else if (theToken->type == ENDIF)
            {
                break;
            }
            else
            {
                processOther(theToken, fout);
            }
        }
        fprintf(fout, "L%d_%d_%s_ENDIF\n", uid, saveNum,name);
    }
    else
    {
        fprintf(fout, "L%d_%d_%s_ElSE\n", uid,saveNum, name);
    }

    return 0;
}
int processReturn(token *theToken, FILE *theFile)
{
    fprintf(theFile, "\t;; epilogue\n");
    // fprintf (theFile, "\tADD R6, R5, #0	;; pop locals off stack\n");
    // fprintf (theFile, "\tADD R6, R6, #3	;; free space for return address, base pointer, and return value\n");
    // fprintf (theFile, "\tSTR R7, R6, #-1 ;; store return value\n");
    // fprintf (theFile, "\tLDR R5, R6, #-3 ;; restore base pointer\n");
    // fprintf (theFile, "\tLDR R7, R6, #-2 ;; restore return address\n");
    // fprintf (theFile, "\tRET\n");
    fprintf(theFile, "\tLDR R7 R6 #0\n\tSTR R7 R5 #2\n\tADD R6 R5 #0\n\tLDR R5 R6 #0\n\tLDR R7 R6 #1\n\tADD R6 R6 #2\n\tRET\n");
    return 0;
}
