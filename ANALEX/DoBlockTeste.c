#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "AnaSintDoBlock.c"
#include "FuncAuxDoBlock.c"
#include "AnaSintDoBlock.h"
#include "FuncAuxDoBlock.h"

FILE *fd;
TOKEN tk;
char TABS[200] = "";

// TabIdef tabela_idef;

void Analisador_sintatico()
{
    fd = fopen("teste.dbk", "r");

    // Iniciar_tabela();
    prog();
    printf("<Resultado> Expressao OK!\n");
    fclose(fd);
}

int main()
{

    printf("[========== Analise sintatica ==========]\n");
    Analisador_sintatico();

    return 0;
}