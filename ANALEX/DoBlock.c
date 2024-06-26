#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "AnaSintDoBlock.c"
#include "FuncAuxDoBlock.c"
#include "AnaSintDoBlock.h"
#include "FuncAuxDoBlock.h"
#include "Processador_tokens.c"

FILE *fd;
TOKEN tk;

char TABS[200] = "";

void Analisador_sintatico()
{
    // Iniciar_tabela();
    printf("\n[========== INICIO - Analise sintatica ==========]\n");
    prog();
    printf("[========== FIM - Analise sintatica ==========]\n");
}

int main()
{

    fd = fopen("teste.dbk", "r");

    if (fd == NULL)
    {
        fprintf(stderr, "Error ao abrir o arquivo.\n");
        return 1;
    }

    Analisador_sintatico();
    // processador_tokens(fd);

    fclose(fd);

    return 0;
}
