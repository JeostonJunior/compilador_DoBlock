#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "AnaLexDoBlock.c"
#include "AnaLexDoBlock.h"
#include "AnaSintDoBlock.h"

void consome(int esperado)
{
    if (tk.cat == esperado || tk.codigo == esperado)
    {
        tk = AnaLex(fd);
    }
    else
    {
        char errMsg[100];
        errorSint(contLinha, errMsg);
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
        errorSint(contLinha, "Declaracao de bloco main esperada.");
    }

    block_main();

    while (tk.cat == PAL_RESERV && tk.codigo == BLOCK)
    {
        block_def();
    }

    if (tk.cat != FIM_PROG)
    {
        errorSint(contLinha, "Fim do Arquivo Esperado");
    }

    printf("\n\t\t<EXPRESSAO OK>\n");
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

    if (tk.codigo != MAIN)
    {
        consome(ID);

        if (tk.codigo == WITH)
        {
            consome(WITH);

            while (true)
            {
                if (tk.cat == FIM_PROG || tk.codigo == MAIN || tk.codigo == BLOCK)
                {
                    break;
                }

                if (tk.codigo == REFERENCIA)
                {
                    consome(REFERENCIA);
                }

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
            }
        }
    }
}

void block_main()
{
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
        errorSint(contLinha, "Tipo invalido");
    }
}

void decl_var()
{
    consome(ID);

    while (tk.codigo == ABRE_COL)
    {

        consome(ABRE_COL);
        if (tk.cat == CONST_INT || tk.cat == ID)
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
            errorSint(contLinha, "Valor esperado após '='.");
        }
    }
}

void atrib()
{
    consome(ID);

    while (tk.codigo == ABRE_COL)
    {
        consome(ABRE_COL);
        expr();
        consome(FECHA_COL);
    }
    consome(ATRIBUICAO);
    expr();
}

void expr()
{
    expr_simp();
    if (tk.cat == OP_RELAC && (tk.codigo == IGUALDADE || tk.codigo == DIFERENTE || tk.codigo == MENOR_IGUAL ||
                               tk.codigo == MENOR_QUE || tk.codigo == MAIOR_IGUAL || tk.codigo == MAIOR_QUE))
    {
        op_rel();
        expr_simp();
    }
}

void expr_simp()
{
    if (tk.codigo == ADICAO || tk.codigo == SUBTRACAO)
    {
        consome(tk.codigo);
    }
    termo();

    if ((tk.codigo == ADICAO || tk.codigo == SUBTRACAO || tk.codigo == OR_LOGIC))
    {
        consome(tk.codigo);
        termo();
    }
}

void termo()
{

    if (tk.cat != FIM_PROG)
    {

        fator();
        while (tk.codigo == MULTIPLICACAO || tk.codigo == DIVISAO || tk.codigo == AND_LOGIC)
        {
            consome(tk.codigo);
            fator();
        }
    }
}

void fator()
{

    if (tk.cat == ID)
    {
        consome(ID);

        while (tk.codigo == ABRE_COL)
        {
            consome(ABRE_COL);
            expr();
            consome(FECHA_COL);
        }
    }
    else if (tk.cat == CONST_INT)
    {
        consome(CONST_INT);
    }
    else if (tk.cat == CONST_FLOAT)
    {
        consome(CONST_FLOAT);
    }
    else if (tk.cat == CONST_CHAR)
    {
        consome(CONST_CHAR);
    }
    else if (tk.codigo == ABRE_PAR)
    {
        consome(ABRE_PAR);
        expr();
        consome(FECHA_PAR);
    }
    else if (tk.codigo == NOT_LOGIC)
    {
        consome(NOT_LOGIC);
        fator();
    }
}

void op_rel()
{

    if (tk.codigo == IGUALDADE || tk.codigo == DIFERENTE || tk.codigo == MENOR_IGUAL ||
        tk.codigo == MENOR_QUE || tk.codigo == MAIOR_IGUAL || tk.codigo == MAIOR_QUE)
    {
        consome(tk.codigo);
    }
    else
    {
        errorSint(contLinha, "Operador relacional esperado");
    }
}

void cmd()
{
    if (tk.cat == PAL_RESERV)
    {
        switch (tk.codigo)
        {
        case DO:
            consome(DO);
            if (tk.cat == ID)
            {
                consome(ID);

                if (tk.codigo == WITH)
                {
                    consome(WITH);
                    consome(ID);

                    while (tk.codigo == VIRGULA)
                    {
                        consome(VIRGULA);
                        consome(ID);
                    }
                }
                if (tk.codigo == VARYING)
                {
                    consome(VARYING);
                    consome(ID);

                    consome(FROM);
                    expr();
                    if (tk.codigo == TO)
                    {
                        consome(TO);
                    }
                    else if (tk.codigo == DOWNTO)
                    {
                        consome(DOWNTO);
                    }
                    else
                    {
                        errorSint(contLinha, "Esperado 'to' ou 'downto'");
                    }
                    expr();
                }
                else if (tk.codigo == WHILE)
                {
                    consome(WHILE);
                    consome(ABRE_PAR);
                    expr();
                    consome(FECHA_PAR);
                }
                else if (tk.codigo == FOR)
                {
                    consome(FOR);
                    expr();
                }
            }
            else
            {
                cmd();
                if (tk.codigo == VARYING)
                {
                    consome(VARYING);
                    consome(ID);

                    consome(FROM);
                    expr();
                    if (tk.codigo == TO)
                    {
                        consome(TO);
                    }
                    else if (tk.codigo == DOWNTO)
                    {
                        consome(DOWNTO);
                    }
                    else
                    {
                        errorSint(contLinha, "Esperado 'to' ou 'downto'");
                    }
                    expr();
                }
                else if (tk.codigo == WHILE)
                {
                    consome(WHILE);
                    consome(ABRE_PAR);
                    expr();
                    consome(FECHA_PAR);
                }
                else if (tk.codigo == FOR)
                {
                    consome(FOR);
                    expr();
                }
            }
            break;

        case IF:
            consome(IF);
            consome(ABRE_PAR);
            expr();
            consome(FECHA_PAR);
            consome(ABRE_CHAVE);
            cmd();
            consome(FECHA_CHAVE);
            while (tk.codigo == ELSEIF)
            {
                consome(ELSEIF);
                consome(ABRE_PAR);
                expr();
                consome(FECHA_PAR);
                consome(ABRE_CHAVE);
                cmd();
                consome(FECHA_CHAVE);
            }
            if (tk.codigo == ELSE)
            {
                consome(ELSE);
                consome(ABRE_CHAVE);
                cmd();
                consome(FECHA_CHAVE);
            }
            consome(ENDIF);
            break;

        case WHILE:
            consome(WHILE);
            consome(ABRE_PAR);
            expr();
            consome(FECHA_PAR);
            consome(ABRE_CHAVE);
            cmd();
            consome(FECHA_CHAVE);
            consome(ENDWHILE);
            break;

        case GOBACK:
            consome(GOBACK);
            break;

        case GETINT:
            consome(GETINT);
            consome(ID);

            break;

        case GETREAL:
            consome(GETREAL);
            consome(ID);
            break;

        case GETCHAR:
            consome(GETCHAR);
            consome(ID);

            break;

        case PUTINT:
            consome(PUTINT);
            consome(ID);

            break;

        case PUTREAL:
            consome(PUTREAL);
            consome(ID);

            break;

        case PUTCHAR:
            consome(PUTCHAR);
            consome(ID);

            break;

        default:
            errorSint(contLinha, "Comando inválido.");
        }
    }
    else if (tk.cat == ID)
    {
        atrib();
    }
    else if (tk.cat != FIM_PROG)
    {
        errorSint(contLinha, "Comando inválido.");
    }
}