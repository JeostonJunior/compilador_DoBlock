#include <stdio.h>
#include "AnaLexDoBlock.c"
#include "FuncAuxDoBlock.c"
#include "Processador_tokens.c"

FILE *fd;
TOKEN tk;

char TABS[200] = "";

int main()
{

    fd = fopen("test.txt", "r");

    if (fd == NULL)
    {
        fprintf(stderr, "Error ao abrir o arquivo.\n");
        return 1;
    }

    processador_tokens(fd);

    fclose(fd);

    return 0;
}
