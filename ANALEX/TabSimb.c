#include "TabSimb.h"
#include <stdbool.h>
#include <string.h>

// Inicializa a tabela de símbolos
void Iniciar_tabela()
{
    tabela_idef.tamTab = 0;
    escopoAtual = 0; // Escopo global
}

// Busca um identificador pelo escopo na tabela de símbolos
int Buscar_escopo(char lexema[], int escopo)
{
    // Busca do fim para o início
    for (int i = tabela_idef.tamTab - 1; i >= 0; i--)
    {
        if (tabela_idef.tabela_simb[i].escopo == escopo && strcmp(tabela_idef.tabela_simb[i].lexema, lexema) == 0)
        {
            return i;
        }
    }
    return -1;
}

// Evita a inserção duplicada na tabela de símbolos
int Insercao_tabela(char lexema[], int tipo, int escopo, char categoria[TAM_CATEGORIA], bool zombie)
{
    if (Buscar_escopo(lexema, escopo) != -1)
    {
        printf("Erro: Identificador '%s' já declarado neste escopo.\n", lexema);
        return -1;
    }

    if (tabela_idef.tamTab == TAM_MAX_TAB)
    {
        printf("Erro: Tabela de símbolos cheia.\n");
        return -1;
    }

    int i = tabela_idef.tamTab;

    strcpy(tabela_idef.tabela_simb[i].lexema, lexema);
    tabela_idef.tabela_simb[i].tipo = tipo;
    tabela_idef.tabela_simb[i].escopo = escopo;
    strcpy(tabela_idef.tabela_simb[i].categoria, categoria);
    tabela_idef.tabela_simb[i].zombie = zombie;
    tabela_idef.tamTab++;

    return i;
}

// Remove o último identificador inserido na tabela de símbolos
int Remover_ultimo()
{
    if (tabela_idef.tamTab > 0)
    {
        int index = tabela_idef.tamTab - 1;

        tabela_idef.tabela_simb[index].lexema[0] = '\0';
        tabela_idef.tabela_simb[index].escopo = 0;
        tabela_idef.tabela_simb[index].tipo = 0;
        tabela_idef.tabela_simb[index].categoria[0] = '\0';
        tabela_idef.tabela_simb[index].zombie = false;
        tabela_idef.tamTab--;

        return index;
    }
    return -1;
}

// Busca um identificador na tabela de símbolos
int Buscar_tabela(char lexema[])
{
    for (int i = 0; i < tabela_idef.tamTab; i++)
    {
        if (strcmp(lexema, tabela_idef.tabela_simb[i].lexema) == 0)
        {
            return i;
        }
    }
    return -1;
}

// Imprime a tabela de símbolos
void Imprimir_tabela(TabIdef tabela_idef, int tamTab)
{
    printf("| %-20s | %-6s | %-6s | %-10s | %-6s |\n",
           "Nome ID", "Tipo", "Escopo", "Categoria", "Zombie");
    printf("|----------------------|--------|--------|------------|--------|\n");

    for (int i = 0; i < tamTab; i++)
    {
        printf("| %-20s | %-6d | %-6d | %-10s | %-6s |\n",
               tabela_idef.tabela_simb[i].lexema,
               tabela_idef.tabela_simb[i].tipo,
               tabela_idef.tabela_simb[i].escopo,
               tabela_idef.tabela_simb[i].categoria,
               tabela_idef.tabela_simb[i].zombie ? "True" : "False");
    }
}
