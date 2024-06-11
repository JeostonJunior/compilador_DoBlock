#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AnaLexDoBlock.h"
#include "FuncAuxDoBlock.h"

void error(char msg[])
{
    printf("%s\n", msg);
    exit(1);
}