#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "AnaLexDoBlock.c"
#include "AnaLexDoBlock.h"
#include "AnaSintDoBlock.h"
#include "TabSimb.h"
#include "TabSimb.c"

TabIdef tabela_idef;
int escopoAtual = 0;

void consome(int esperado) {
    if ((int)tk.cat == esperado || (int)tk.codigo == esperado) {
        tk = AnaLex(fd);
    } else {
        char errMsg[100];
        sprintf(errMsg, "Token inesperado. Esperado: %d, Encontrado: %d", esperado, tk.codigo);
        errorSint(contLinha, errMsg);
    }
}

void prog() {
    Iniciar_tabela();
    tk = AnaLex(fd);

    while (tk.cat == PAL_RESERV && (tk.codigo == CONST || tk.codigo == INT || tk.codigo == CHAR || tk.codigo == REAL || tk.codigo == BOOL)) {
        decl_list_var();
    }

    while (tk.cat == PAL_RESERV && tk.codigo == BLOCK) {
        decl_block_prot();
    }

    if (!(tk.cat == PAL_RESERV && tk.codigo == MAIN)) {
        errorSint(contLinha, "Declaracao de bloco main esperada.");
    }

    block_main();

    while (tk.cat == PAL_RESERV && tk.codigo == BLOCK) {
        block_def();
    }

    if (tk.cat != FIM_PROG) {
        errorSint(contLinha, "Fim do Arquivo Esperado");
    }

    Imprimir_tabela();
}

void decl_list_var() {
    printf("\nENTROU -> decl_list_var\n");

    if (tk.codigo == CONST) {
        consome(CONST);
    }
    tipo();
    decl_var();

    while (tk.codigo == VIRGULA) {
        consome(VIRGULA);
        decl_var();
    }

    printf("\nSAIU -> decl_list_var\n");
}

void decl_block_prot() {
    printf("\nENTROU -> decl_block_prot\n");
    consome(BLOCK);

    if (tk.codigo != MAIN) {
        escopoAtual++;
        consome(ID);

        if (tk.codigo == WITH) {
            printf("[decl_block_prot][WITH][Entrada]\n\n");
            consome(WITH);

            while (true) {
                if (tk.cat == FIM_PROG || tk.codigo == MAIN || tk.codigo == BLOCK) {
                    break;
                }

                if (tk.codigo == REFERENCIA) {
                    printf("[decl_block_prot][REFERENCIA][Entrada]\n");
                    consome(REFERENCIA);
                    printf("[decl_block_prot][REFERENCIA][Saida]\n\n");
                }

                tipo();

                if (tk.codigo == ABRE_COL) {
                    printf("[decl_block_prot][ABRE_COL][Entrada]\n");
                    consome(ABRE_COL);
                    printf("[decl_block_prot][ABRE_COL][Saida]\n\n");

                    printf("[decl_block_prot][FECHA_COL][Entrada]\n");
                    consome(FECHA_COL);
                    printf("[decl_block_prot][FECHA_COL][Saida]\n\n");
                }
                if (tk.codigo == VIRGULA) {
                    printf("[decl_block_prot][VIRGULA][Entrada]\n");
                    consome(VIRGULA);
                    printf("[decl_block_prot][VIRGULA][Saida]\n\n");
                }
            }
            printf("[decl_block_prot][WITH][Saida]\n\n");
        }
        escopoAtual--;
    }

    printf("\nSAIU -> decl_block_prot\n");
}

void block_main() {
    printf("\nENTROU -> block_main\n\n");
    consome(MAIN);
    escopoAtual++;

    while (tk.cat == PAL_RESERV && (tk.codigo == CONST || tk.codigo == INT || tk.codigo == CHAR || tk.codigo == REAL || tk.codigo == BOOL)) {
        decl_list_var();
    }

    while (tk.codigo != ENDBLOCK) {
        cmd();
    }
    consome(ENDBLOCK);

    escopoAtual--;

    printf("\nSAIU -> block_main\n");
}

void block_def() {
    printf("\nENTROU -> block_def\n");
    consome(BLOCK);
    consome(ID);

    if (tk.codigo == WITH) {
        consome(WITH);
        tipo();
        consome(ID);

        if (tk.codigo == ABRE_COL) {
            consome(ABRE_COL);
            if (tk.cat == CONST_INT || tk.cat == ID_CONST) {
                consome(tk.cat);
            }
            consome(FECHA_COL);
        }

        while (tk.codigo == VIRGULA) {
            consome(VIRGULA);
            tipo();
            consome(ID);
            if (tk.codigo == ABRE_COL) {
                consome(ABRE_COL);
                if (tk.cat == CONST_INT || tk.cat == ID_CONST) {
                    consome(tk.cat);
                }
                consome(FECHA_COL);
            }
        }
    }

    while (tk.cat == PAL_RESERV && (tk.codigo == CONST || tk.codigo == INT || tk.codigo == CHAR || tk.codigo == REAL || tk.codigo == BOOL)) {
        decl_list_var();
    }

    while (tk.codigo != ENDBLOCK) {
        cmd();
        printf("<Resultado> Expressao OK!\n");
    }
    consome(ENDBLOCK);
    printf("\nSAIU -> block_def\n");
}

void tipo() {
    printf("\nENTROU -> tipo\n");

    if (tk.codigo == CHAR || tk.codigo == INT || tk.codigo == REAL || tk.codigo == BOOL) {
        consome(tk.codigo);
    } else {
        errorSint(contLinha, "Tipo invalido");
    }
    printf("\nSAIU -> tipo\n");
}
void decl_var() {
    printf("\nENTROU -> decl_var\n");

    // Captura o lexema antes de consumir
    char lexema[TAM_MAX_LEXEMA];
    strncpy(lexema, tk.lexema, TAM_MAX_LEXEMA - 1);
    lexema[TAM_MAX_LEXEMA - 1] = '\0';

    if (Buscar_escopo(lexema, escopoAtual) != -1) {
        printf("[ERRO] Identificador '%s' já declarado no escopo atual.\n", lexema);
        errorSint(contLinha, "Identificador já declarado no escopo atual");
    }

    int tipo_var = tk.codigo;
    consome(ID);

    if (Insercao_tabela(lexema, escopoAtual, tipo_var, "var", false) == -1) {
        printf("[ERRO] Não foi possível inserir o identificador na tabela de símbolos.\n");
    }

    while (tk.codigo == ABRE_COL) {
        consome(ABRE_COL);
        if (tk.cat == CONST_INT || tk.cat == ID) {
            consome(tk.cat);
        }
        consome(FECHA_COL);
    }

    if (tk.codigo == ATRIBUICAO) {
        consome(ATRIBUICAO);
        if (tk.cat == CONST_INT || tk.cat == CONST_FLOAT || tk.cat == CONST_CHAR || tk.cat == LITERAL) {
            consome(tk.cat);
        } else if (tk.codigo == ABRE_CHAVE) {
            consome(ABRE_CHAVE);
            do {
                if (tk.cat == CONST_INT || tk.cat == CONST_FLOAT || tk.cat == CONST_CHAR || tk.cat == LITERAL) {
                    consome(tk.cat);
                }
                if (tk.codigo == VIRGULA) {
                    consome(VIRGULA);
                } else {
                    break;
                }
            } while (true);
            consome(FECHA_CHAVE);
        } else {
            errorSint(contLinha, "Valor esperado após '='.");
        }
    }
    printf("\nSAIU -> decl_var\n");
}
void atrib() {
    consome(ID);
    while (tk.codigo == ABRE_COL) {
        consome(ABRE_COL);
        expr();
        consome(FECHA_COL);
    }
    consome(ATRIBUICAO);
    expr();
}

void expr() {
    printf("\nENTROU -> expr\n");
    expr_simp();
    if (tk.cat == OP_RELAC && (tk.codigo == IGUALDADE || tk.codigo == DIFERENTE || tk.codigo == MENOR_IGUAL ||
                               tk.codigo == MENOR_QUE || tk.codigo == MAIOR_IGUAL || tk.codigo == MAIOR_QUE)) {
        op_rel();
        expr_simp();
    }
    printf("\nSAIU -> expr\n");
}

void expr_simp() {
    printf("\nENTROU -> expr_simp\n");
    if (tk.codigo == ADICAO || tk.codigo == SUBTRACAO) {
        consome(tk.codigo);
    }
    termo();

    if ((tk.codigo == ADICAO || tk.codigo == SUBTRACAO || tk.codigo == OR_LOGIC)) {
        consome(tk.codigo);
        termo();
    }

    printf("\nSAIU -> expr_simp\n");
}

void termo() {
    printf("\nENTROU -> termo\n");
    if (tk.cat != FIM_PROG) {

        fator();
        while (tk.codigo == MULTIPLICACAO || tk.codigo == DIVISAO || tk.codigo == AND_LOGIC) {
            consome(tk.codigo);
            fator();
        }
    }
    printf("\nSAIU -> termo\n");
}

void fator() {
    printf("\nENTROU -> fator\n");
    if (tk.cat == ID) {
        consome(ID);
        while (tk.codigo == ABRE_COL) {
            consome(ABRE_COL);
            expr();
            consome(FECHA_COL);
        }
    } else if (tk.cat == CONST_INT) {
        consome(CONST_INT);
    } else if (tk.cat == CONST_FLOAT) {
        consome(CONST_FLOAT);
    } else if (tk.cat == CONST_CHAR) {
        consome(CONST_CHAR);
    } else if (tk.codigo == ABRE_PAR) {
        consome(ABRE_PAR);
        expr();
        consome(FECHA_PAR);
    } else if (tk.codigo == NOT_LOGIC) {
        consome(NOT_LOGIC);
        fator();
    }
    printf("\nSAIU -> fator\n");
}

void op_rel() {
    printf("\nENTROU -> op_rel\n");
    if (tk.codigo == IGUALDADE || tk.codigo == DIFERENTE || tk.codigo == MENOR_IGUAL ||
        tk.codigo == MENOR_QUE || tk.codigo == MAIOR_IGUAL || tk.codigo == MAIOR_QUE) {
        consome(tk.codigo);
    } else {
        errorSint(contLinha, "Operador relacional esperado");
    }
    printf("\nSAIU -> op_rel\n");
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