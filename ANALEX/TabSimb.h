#ifndef TABSIMB
#define TABIM

#include <stdio.h>
#include <stdlib.h>
#include "AnaLexDoBlock.h"
#include <stdbool.h>

#define TAM_MAX_TAB 100
#define TAM_CATEGORIA 3 
typedef
    struct 
    {
        /* data */
        char lexema[TAM_MAX_LEXEMA];
        int tipo;
        int escopo;
        char categoria[TAM_CATEGORIA];
        bool zombie;
        int endereco;

    }TabSimb;
    
typedef
    struct 
    {
        /* data */
        int tamTab;
        TabSimb tabela_simb[TAM_MAX_TAB];
    }TabIdef;
    
extern TabIdef tabela_idef;
extern TOKEN tk;

void Iniciar_tabela();
void Imprimir_tabela(TabIdef, int tamTab);
int Buscar_tabela(char lexema[]);
int Buscar_escopo(char lexema[],int escopo);
int Insercao_tabela(char lexema[],int escopo,int tipo, char categoria[TAM_CATEGORIA],bool zombie);
int Remover_ultimo();
#endif
