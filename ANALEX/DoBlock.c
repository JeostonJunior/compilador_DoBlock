#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "AnaLexDoBlock.h"
#include "AnaLexDoBlock.c"

FILE *fd;
TOKEN tk;

int main()
{

    fd = fopen("test.txt", "r");

    while (1)
    {
        tk = AnaLex(fd);

        switch (tk.cat)
        {
        case ID:
            printf("<ID, %s>\n", tk.lexema);
            break;
        case CONST_INT:
            printf("<CONST_INT, %d>\n", tk.valInt);
            break;
        case CONST_FLOAT:
            printf("<CONST_FLOAT, %0.2f>\n", tk.valFloat);
            break;
        case CONST_CHAR:
            printf("<CONST_CHAR, %s>\n", tk.lexema);
            break;
        case LITERAL:
            printf("<LITERAL, %s>\n", tk.lexema);
            break;
        case FIM_EXPR:
            printf("<FIM EXPRESSAO>\n\n", tk.lexema);
            break;
        case ID_CONST:
            printf("<ID_CONST>\n\n");
            break;
        case FIM_PROG:
            printf("\nFIM DO ARQUIVO\n");
        case SINAL:
            switch (tk.codigo)
            {
            case VIRGULA:
                printf("<SN, VIRGULA>\n");
                break;
            case ABRE_PAR:
                printf("<SN, ABRE_PAR>\n");
                break;
            case FECHA_PAR:
                printf("<SN, FECHA_PAR>\n");
                break;
            case ABRE_CHAVE:
                printf("<SN, ABRE_CHAVE>\n");
                break;
            case FECHA_CHAVE:
                printf("<SN, FECHA_CHAVE>\n");
                break;
            case ABRE_COL:
                printf("<SN, ABRE_COL>\n");
                break;
            case FECHA_COL:
                printf("<SN, FECHA_COL>\n");
                break;
            case REFERENCIA:
                printf("<SN, REFERENCIA>\n");
                break;
            default:
                break;
            }
            break;
        case PAL_RESERV:
            switch (tk.codigo)
            {
            case MAIN:
                printf("<PR, MAIN>\n");
                break;
            case BLOCK:
                printf("<PR, BLOCK>\n");
                break;
            case ENDBLOCK:
                printf("<PR, ENDBLOCK>\n");
                break;
            case CONST:
                printf("<PR, CONST>\n");
                break;
            case CHAR:
                printf("<PR, CHAR>\n");
                break;
            case INT:
                printf("<PR, INT>\n");
                break;
            case REAL:
                printf("<PR, REAL>\n");
                break;
            case BOOL:
                printf("<PR, BOOL>\n");
                break;
            case WITH:
                printf("<PR, WITH>\n");
                break;
            case DO:
                printf("<PR, DO>\n");
                break;
            case VARYING:
                printf("<PR, VARYING>\n");
                break;
            case FROM:
                printf("<PR, FROM>\n");
                break;
            case TO:
                printf("<PR, TO>\n");
                break;
            case DOWNTO:
                printf("<PR, DOWNTO>\n");
                break;
            case WHILE:
                printf("<PR, WHILE>\n");
                break;
            case FOR:
                printf("<PR, FOR>\n");
                break;
            case IF:
                printf("<PR, IF>\n");
                break;
            case ENDIF:
                printf("<PR, ENDIF>\n");
                break;
            case ELSEIF:
                printf("<PR, ELSEIF>\n");
                break;
            case ELSE:
                printf("<PR, ELSE>\n");
                break;
            case GOBACK:
                printf("<PR, GOBACK>\n");
                break;
            case GETCHAR:
                printf("<PR, GETCHAR>\n");
                break;
            case PUTINT:
                printf("<PR, PUTINT>\n");
                break;
            case GETINT:
                printf("<PR, GETINT>\n");
                break;
            case PUTREAL:
                printf("<PR, PUTREAL>\n");
                break;
            case PUTCHAR:
                printf("<PR, PUTCHAR>\n");
                break;
            default:
                break;
            }
            break;
        case OP_ARIT:
            switch (tk.codigo)
            {
            case ATRIBUICAO:
                printf("<OP_ARIT, ATRIBUICAO>\n");
                break;
            case ADICAO:
                printf("<OP_ARIT, ADICAO>\n");
                break;
            case SUBTRACAO:
                printf("<OP_ARIT, SUBTRACAO>\n");
                break;
            case MULTIPLICACAO:
                printf("<OP_ARIT, MULTIPLICACAO>\n");
                break;
            case DIVISAO:
                printf("<OP_ARIT, DIVISAO>\n");
                break;
            default:
                break;
            }
            break;
        case OP_LOGIC:
            switch (tk.codigo)
            {
            case AND_LOGIC:
                printf("<OP_LOGIC, AND_LOGIC>\n");
                break;
            case OR_LOGIC:
                printf("<OP_LOGIC, OR_LOGIC>\n");
                break;
            case NOT_LOGIC:
                printf("<OP_LOGIC, NOT_LOGIC>\n");
                break;
            default:
                break;
            }
            break;
        case OP_RELAC:
            switch (tk.codigo)
            {
            case IGUALDADE:
                printf("<OP_RELAC, IGUALDADE>\n");
                break;
            case DIFERENTE:
                printf("<OP_RELAC, DIFERENTE>\n");
                break;
            case MENOR_IGUAL:
                printf("<OP_RELAC, MENOR_IGUAL>\n");
                break;
            case MAIOR_IGUAL:
                printf("<OP_RELAC, MAIOR_IGUAL>\n");
                break;
            case MENOR_QUE:
                printf("<OP_RELAC, MENOR_QUE>\n");
                break;
            case MAIOR_QUE:
                printf("<OP_RELAC, MAIOR_QUE>\n");
                break;
            default:
                break;
            }
            break;
        }
        if (tk.cat == FIM_PROG)
        {
            break;
        }
    }
    fclose(fd);

    return 0;
}