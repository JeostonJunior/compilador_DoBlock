#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "AnaLexDoBlock.h"
#include "AnaSintDoBlock.h"
#include "FuncAuxDoBlock.h"

void consome(int esperado)
{
    if (tk.cat == esperado || tk.codigo == esperado)
    {
        tk = AnaLex(fd);
    }
    else
    {
        char errMsg[100];
        sprintf(errMsg, "Token inesperado. Esperado: %d, Encontrado: %d", esperado, tk.codigo);
        error(errMsg);
    }
}

void prog()
{
    tk = AnaLex(fd);

    while (tk.cat == PAL_RESERV && (tk.codigo == CONST || tk.codigo == INT || tk.codigo == CHAR || tk.codigo == REAL || tk.codigo == BOOL))
    {
        decl_list_var();
    }

    while (tk.cat == PAL_RESERV && tk.codigo == BLOCK)
    {
        decl_block_prot();
    }

    if (!(tk.cat == PAL_RESERV && tk.codigo == MAIN))
    {
        error("Declaração de bloco main esperada.");
    }

    block_main();

    while (tk.cat == PAL_RESERV && tk.codigo == BLOCK)
    {
        block_def();
    }

    if (tk.cat != FIM_PROG)
    {
        error("Fim do Arquivo Esperado");
    }
}

void decl_list_var()
{
    if (tk.codigo == CONST)
    {
        consome(CONST);
    }

    tipo();
    decl_var();

    while (tk.codigo == VIRGULA)
    {
        consome(VIRGULA);
        decl_var();
    }
}

void decl_block_prot()
{
    consome(BLOCK);
    consome(ID);

    if (tk.codigo == WITH)
    {
        consome(WITH);

        do
        {
            consome(REFERENCIA);
            tipo();
            if (tk.codigo == ABRE_COL)
            {
                consome(ABRE_COL);
                consome(FECHA_COL);
            }
            if (tk.codigo == VIRGULA)
            {
                consome(VIRGULA);
            }
            else
            {
                break;
            }
        } while (true);
    }
}

void block_main()
{
    consome(BLOCK);
    consome(MAIN);

    while (tk.cat == PAL_RESERV && (tk.codigo == CONST || tk.codigo == INT || tk.codigo == CHAR || tk.codigo == REAL || tk.codigo == BOOL))
    {
        decl_list_var();
    }

    while (tk.codigo != ENDBLOCK)
    {
        cmd();
    }
    consome(ENDBLOCK);
}

void block_def()
{
    consome(BLOCK);
    consome(ID);

    if (tk.codigo == WITH)
    {
        consome(WITH);
        tipo();
        consome(ID);

        if (tk.codigo == ABRE_COL)
        {
            consome(ABRE_COL);
            if (tk.cat == CONST_INT || tk.cat == ID_CONST)
            {
                consome(tk.cat);
            }
            consome(FECHA_COL);
        }

        while (tk.codigo == VIRGULA)
        {
            consome(VIRGULA);
            tipo();
            consome(ID);
            if (tk.codigo == ABRE_COL)
            {
                consome(ABRE_COL);
                if (tk.cat == CONST_INT || tk.cat == ID_CONST)
                {
                    consome(tk.cat);
                }
                consome(FECHA_COL);
            }
        }
    }

    while (tk.cat == PAL_RESERV && (tk.codigo == CONST || tk.codigo == INT || tk.codigo == CHAR || tk.codigo == REAL || tk.codigo == BOOL))
    {
        decl_list_var();
    }

    while (tk.codigo != ENDBLOCK)
    {
        cmd();
    }
    consome(ENDBLOCK);
}
void tipo()
{
    if (tk.codigo == CHAR || tk.codigo == INT || tk.codigo == REAL || tk.codigo == BOOL)
    {
        consome(tk.codigo);
    }
    else
    {
        error("Tipo inválido");
    }
}

void decl_var()
{
    consome(ID);

    if (tk.codigo == ABRE_COL)
    {
        consome(ABRE_COL);
        if (tk.cat == CONST_INT || tk.cat == ID_CONST)
        {
            consome(tk.cat);
        }
        consome(FECHA_COL);
    }

    if (tk.codigo == ATRIBUICAO)
    {
        consome(ATRIBUICAO);
        if (tk.cat == CONST_INT || tk.cat == CONST_FLOAT || tk.cat == CONST_CHAR || tk.cat == LITERAL)
        {
            consome(tk.cat);
        }
        else if (tk.codigo == ABRE_CHAVE)
        {
            consome(ABRE_CHAVE);
            do
            {
                if (tk.cat == CONST_INT || tk.cat == CONST_FLOAT || tk.cat == CONST_CHAR || tk.cat == LITERAL)
                {
                    consome(tk.cat);
                }
                if (tk.codigo == VIRGULA)
                {
                    consome(VIRGULA);
                }
                else
                {
                    break;
                }
            } while (true);
            consome(FECHA_CHAVE);
        }
        else
        {
            error("Valor esperado após '='.");
        }
    }
}