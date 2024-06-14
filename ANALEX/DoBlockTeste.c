#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "AnaSintDoBlock.c"
#include "FuncAuxDoBlock.c"

FILE *fd;
TOKEN tk;
char TABS[200] = "";

void Analisador_sintatico()
{
    fd = fopen("teste.dbk", "r");

    prog();
    if (tk.cat == FIM_PROG)
    {
        printf("<Resultado> Expressao OK!\n");
    }

    fclose(fd);
}

int main()
{

    printf("[========== Analise sintatica ==========]\n");
    Analisador_sintatico();

    return 0;
}