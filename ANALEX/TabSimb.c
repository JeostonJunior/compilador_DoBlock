#include "TabSimb.h"
#include <stdbool.h>

void Iniciar_tabela()
{
    tabela_idef.tamTab= 0;
}

int Buscar_tabela(char lexema[])
{
    for(int i = 0; i < tabela_idef.tabela_simb; i++){
        if(!(strcmp(lexema,tabela_idef.tabela_simb[i].lexema))){
            return i;
        }
    }
    return -1;
}

int Buscar_escopo(char lexema[], int escopo)
{
    int i;
    for(i = 0; i < tabela_idef.tamTab;i++){
        if(!(strcmp(lexema, tabela_idef.tabela_simb[i].lexema))){
            return i;
        }
    }
    return -1;
}

int Insercao_tabela(char lexema[], int escopo, int tipo, char categoria[TAM_CATEGORIA], bool zombie)
{
    int i;

    if(tabela_idef.tamTab == TAM_MAX_TAB){
        printf("Erro");

    }else{
        i = tabela_idef.tamTab;

        strcpy(tabela_idef.tabela_simb[i].lexema, lexema);
        
        //tabela_idef.tabela_simb[i].endereco = i;
        
        tabela_idef.tabela_simb[i].escopo = escopo;
        tabela_idef.tabela_simb[i].tipo = tipo;

        strcpy(tabela_idef.tabela_simb[i].categoria,categoria);
        tabela_idef.tabela_simb[i].zombie = true;

        tabela_idef.tamTab++;

        return i;
    }
}
int Remover_ultimo()
{
    if(tabela_idef.tamTab > 0){
        int index = tabela_idef.tamTab - 1;

        tabela_idef.tabela_simb[index].lexema[0] = '\0';
        tabela_idef.tabela_simb[index].escopo = 0;
        tabela_idef.tabela_simb[index].tipo = 0;
        tabela_idef.tabela_simb[index].categoria[0] = '\0';
        tabela_idef.tabela_simb[index].zombie = false;

        tabela_idef.tamTab--;
        return index;
    }
}
