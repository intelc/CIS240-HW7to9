#include <stdio.h>
#include "token.h"
int insertHeader(FILE *theFile, char *name);
int insertPrologue(FILE *theFile);
int processOther(token *theToken, FILE *theFile);
int processIf(token *theToken, FILE *fin, FILE *fout, char *name);
int processReturn(token *theToken, FILE *theFile);
