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

void Analisador_lexico(FILE *fd)
{
    printf("\n[========== INICIO - Analise lexica ==========]\n");
    processador_tokens(fd);
    printf("\n[========== FIM - Analise lexica ==========]\n");
}

void Analisador_sintatico()
{
    Iniciar_tabela();
    printf("\n[========== INICIO - Analise sintatica ==========]\n");
    prog();
    printf("\n[========== FIM - Analise sintatica ==========]\n");

    // Testando outras funções da tabela de símbolos
    const char *lex = "DIM_COL";
    int resultado = Buscar_tabela(lex);
    
    if (resultado != -1) {
        printf("Lexema encontrado no índice: %d\n", resultado);
    } else {
        printf("Lexema não encontrado.\n");
    }
    int result = Remover_ultimo(); //0 é removido 
    printf("%d",result);
}

int main()
{

    //fd = fopen("C:/Users/guilh/OneDrive/Documentos/DoBlock2/compilador_DoBlock/ANALEX/teste.dbk", "r");
     fd = fopen("teste.dbk", "r");
    if (fd == NULL)
    {
        fprintf(stderr, "\nError ao abrir o arquivo.\n");
        return 1;
    }

    // Analisador_lexico(fd); // É só um exemplo, o analisador lexico está dentro do sintatico
    Analisador_sintatico();

    fclose(fd);

    return 0;
}
