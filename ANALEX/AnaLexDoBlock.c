#include <stdio.h>

#include <stdlib.h>

#include <ctype.h>

#include <string.h>

#include "AnaLexDoBlock.h"

#define TAM_LEXEMA 50
#define TAM_LITERAL 50
#define TAM_NUM 20

int indiceLiteral = 0;
char tableLit[TAM_LITERAL][TAM_LEXEMA] = {""};
int lti = 0; // controle do indice de literais
int contLinha = 1;

void error(int contLinha, char caracter)
{
    printf("\nCaracter '%c' invalido na linha: %d\n", caracter, contLinha);
    exit(1);
}

TOKEN AnaLex(FILE *fd)
{
    int estado = 0;
    int tamL = 0;
    int tamD = 0;
    char lexema[TAM_LEXEMA] = "";
    char digitos[TAM_NUM] = "";

    char c;

    TOKEN token;

    while (1)
    {
        c = fgetc(fd);

        switch (estado)
        {
        case 0:
            if (c == ' ' || c == '\t')
            {
                estado = 0;
            }
            else if (c == '\'')
            {
                estado = 9;
                lexema[tamL] = c;

                lexema[++tamL] = '\0';
            }
            else if (c == '\"')
            {
                estado = 15;
            }
            else if (c == '/')
            {
                estado = 18;
            }
            else if (c == '=')
            {
                estado = 21;
            }
            else if ((c == '_') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
            {
                estado = 1;
                lexema[tamL] = c;
                lexema[++tamL] = '\0';
            }
            else if (c >= '0' && c <= '9')
            {
                estado = 4;
                digitos[tamD] = c;
                digitos[++tamD] = '\0';
            }
            else if (c == '+')
            {
                estado = 35;
                token.cat = OP_ARIT;
                token.codigo = ADICAO;
                return token;
            }
            else if (c == '-')
            {
                estado = 36;
                token.cat = OP_ARIT;
                token.codigo = SUBTRACAO;
                return token;
            }
            else if (c == '*')
            {
                estado = 37;
                token.cat = OP_ARIT;
                token.codigo = MULTIPLICACAO;
                return token;
            }
            else if (c == '!')
            {
                estado = 24;
                token.cat = OP_RELAC;
            }
            else if (c == '|')
            {
                estado = 42;
                token.cat = OP_LOGIC;
            }
            else if (c == '&')
            {
                estado = 39;
            }
            else if (c == '>')
            {
                estado = 30;
                token.cat = OP_RELAC;
            }
            else if (c == '<')
            {
                estado = 27;
                token.cat = OP_RELAC;
            }
            else if (c == '{')
            {
                estado = 33;
                token.cat = SINAL;
                token.codigo = ABRE_CHAVE;
                return token;
            }
            else if (c == '}')
            {
                estado = 34;
                token.cat = SINAL;
                token.codigo = FECHA_CHAVE;
                return token;
            }
            else if (c == '(')
            {
                estado = 44;
                token.cat = SINAL;
                token.codigo = ABRE_PAR;
                return token;
            }
            else if (c == ')')
            {
                estado = 45;
                token.cat = SINAL;
                token.codigo = FECHA_PAR;
                return token;
            }
            else if (c == ',')
            {
                estado = 52;
                token.cat = SINAL;
                token.codigo = VIRGULA;
                return token;
            }
            else if (c == '[')
            {
                estado = 46;
                token.cat = SINAL;
                token.codigo = ABRE_COL;
                return token;
            }
            else if (c == ']')
            {
                estado = 47;
                token.cat = SINAL;
                token.codigo = FECHA_COL;
                return token;
            }
            else if (c == '\n')
            {
                estado = 0;
                // token.cat = FIM_EXPR;
                // contLinha++;
                // return token;
            }
            else if (c == EOF)
            {
                token.cat = FIM_PROG;
                return token;
            }
            else
            {
                error(contLinha, c);
            }
            break;
        case 1:
            if (c == '_' || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
            {
                estado = 1;
                lexema[tamL] = c;
                lexema[++tamL] = '\0';
            }
            else
            {
                estado = 3;

                ungetc(c, fd);

                strcpy(token.lexema, lexema);
                // verificacao de palavras reservadas
                if (strcmp("MAIN", lexema) == 0)
                {
                    token.cat = PAL_RESERV;
                    token.codigo = MAIN;
                }
                else if (strcmp("BLOCK", lexema) == 0)
                {
                    token.cat = PAL_RESERV;
                    token.codigo = BLOCK;
                }
                else if (strcmp("ENDBLOCK", lexema) == 0)
                {
                    token.cat = PAL_RESERV;
                    token.codigo = ENDBLOCK;
                }
                else if (strcmp("CONST", lexema) == 0)
                {
                    token.cat = PAL_RESERV;
                    token.codigo = CONST;
                }
                else if (strcmp("CHAR", lexema) == 0)
                {
                    token.cat = PAL_RESERV;
                    token.codigo = CHAR;
                }
                else if (strcmp("INT", lexema) == 0)
                {
                    token.cat = PAL_RESERV;
                    token.codigo = INT;
                }
                else if (strcmp("REAL", lexema) == 0)
                {
                    token.cat = PAL_RESERV;
                    token.codigo = REAL;
                }
                else if (strcmp("BOOL", lexema) == 0)
                {
                    token.cat = PAL_RESERV;
                    token.codigo = BOOL;
                }
                else if (strcmp("WITH", lexema) == 0)
                {
                    token.cat = PAL_RESERV;
                    token.codigo = WITH;
                }
                else if (strcmp("DO", lexema) == 0)
                {
                    token.cat = PAL_RESERV;
                    token.codigo = DO;
                }
                else if (strcmp("VARYING", lexema) == 0)
                {
                    token.cat = PAL_RESERV;
                    token.codigo = VARYING;
                }
                else if (strcmp("FROM", lexema) == 0)
                {
                    token.cat = PAL_RESERV;
                    token.codigo = FROM;
                }
                else if (strcmp("TO", lexema) == 0)
                {
                    token.cat = PAL_RESERV;
                    token.codigo = TO;
                }
                else if (strcmp("DOWNTO", lexema) == 0)
                {
                    token.cat = PAL_RESERV;
                    token.codigo = DOWNTO;
                }
                else if (strcmp("WHILE", lexema) == 0)
                {
                    token.cat = PAL_RESERV;
                    token.codigo = WHILE;
                }
                else if (strcmp("FOR", lexema) == 0)
                {
                    token.cat = PAL_RESERV;
                    token.codigo = FOR;
                }
                else if (strcmp("IF", lexema) == 0)
                {
                    token.cat = PAL_RESERV;
                    token.codigo = IF;
                }
                else if (strcmp("ELSEIF", lexema) == 0)
                {
                    token.cat = PAL_RESERV;
                    token.codigo = ELSEIF;
                }
                else if (strcmp("ELSE", lexema) == 0)
                {
                    token.cat = PAL_RESERV;
                    token.codigo = ELSE;
                }
                else if (strcmp("ENDIF", lexema) == 0)
                {
                    token.cat = PAL_RESERV;
                    token.codigo = ENDIF;
                }
                else if (strcmp("GOBACK", lexema) == 0)
                {
                    token.cat = PAL_RESERV;
                    token.codigo = GOBACK;
                }
                else if (strcmp("GETINT", lexema) == 0)
                {
                    token.cat = PAL_RESERV;
                    token.codigo = GETINT;
                }
                else if (strcmp("GETCHAR", lexema) == 0)
                {
                    token.cat = PAL_RESERV;
                    token.codigo = GETCHAR;
                }
                else if (strcmp("PUTINT", lexema) == 0)
                {
                    token.cat = PAL_RESERV;
                    token.codigo = PUTINT;
                }
                else if (strcmp("PUTREAL", lexema) == 0)
                {
                    token.cat = PAL_RESERV;
                    token.codigo = PUTREAL;
                }
                else if (strcmp("PUTCHAR", lexema) == 0)
                {
                    token.cat = PAL_RESERV;
                    token.codigo = PUTCHAR;
                }
                else
                {
                    // Se a palavra não for uma palavra reservada conhecida, retorne um token ID
                    token.cat = ID;
                    strcpy(token.lexema, lexema);
                }

                return token;
            }
            break;
        case 4:
            if (c >= '0' && c <= '9')
            {
                estado = 4;
                digitos[tamD] = c;
                digitos[++tamD] = '\0';
            }
            else if (c == '.')
            {
                estado = 7;
                digitos[tamD] = c;
                digitos[++tamD] = '\0';
            }
            else if (c == '_' || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
            {
                error(contLinha, c);
            }
            else
            {
                estado = 5;
                ungetc(c, fd);

                token.cat = CONST_INT;
                token.valInt = atoi(digitos);

                return token;
            }
            break;
        case 7:
            if (c >= '0' && c <= '9')
            {
                estado = 7;

                digitos[tamD] = c;
                digitos[++tamD] = '\0';
            }
            else
            {
                // transicao OUTRO* do estado 8 do AFD

                estado = 8; // monta token constante float e retorna

                ungetc(c, fd);

                token.cat = CONST_FLOAT;
                token.valFloat = atof(digitos);

                return token;
            }
            break;
        case 9:
            if (c == '\'')
            {
                error(contLinha, c);
            }
            else if (c == '\\')
            {
                estado = 12;
            }
            else if (isprint(c))
            {
                estado = 10;
                lexema[tamL] = c;
                lexema[++tamL] = '\0';
            }
            else
            {
                error(contLinha, c);
            }
            break;
        case 10:
            if (c == '\'')
            {
                estado = 0;

                token.cat = CONST_CHAR;

                strcpy(token.lexema, lexema);

                return token;
            }
            else
            {
                error(contLinha, c);
            }
            break;
        case 12:
            if (c == 'n')
            {
                estado = 10;
                strcpy(lexema, "enter");
            }
            else if (c == '0')
            {
                estado = 10;

                strcpy(lexema, "null");
            }
            else
            {
                error(contLinha, c);
            }
            break;
        case 15:
            if (c == '\"')
            {
                if (tamL == 0)
                {
                    error(contLinha, c);
                }

                estado = 0;

                token.cat = LITERAL;
                strcpy(token.lexema, lexema);

                return token;
            }
            else if (isprint(c))
            {
                if (tamL < TAM_LEXEMA - 1)
                {
                    lexema[tamL] = c;
                    lexema[++tamL] = '\0';
                }
                else
                {
                    error(contLinha, c);
                }
            }
            else if (c == '\n')
            {
                error(contLinha, c);
            }
            break;
        case 18:
            if (c == '/')
            {
                estado = 19;

                lexema[tamL] = c;
                lexema[++tamL] = '\0';
            }
            else
            {
                estado = 38;
                ungetc(c, fd);

                token.cat = OP_ARIT;
                token.codigo = DIVISAO;

                return token;
            }
            break;
        case 19:
            if (c == '\n')
            {
                estado = 0; // Voltar ao estado inicial
            }
            else
            {
                // Continuar lendo o comentário
                lexema[tamL++] = c;
            }
            break;
        case 21:
            if (c == '=')
            {
                estado = 22;

                token.cat = OP_RELAC;
                token.codigo = IGUALDADE;

                return token;
            }
            else
            {
                estado = 23;

                ungetc(c, fd);

                token.cat = OP_ARIT;
                token.codigo = ATRIBUICAO;

                return token;
            }
            break;
        case 24:
            if (c == '=')
            {
                estado = 0;

                token.codigo = DIFERENTE;

                return token;
            }
            else
            {
                estado = 0;
                ungetc(c, fd);

                token.codigo = NOT_LOGIC;

                return token;
            }
            break;
        case 27:
            if (c == '=')
            {
                estado = 28;

                token.codigo = MENOR_IGUAL;

                return token;
            }
            else
            {
                estado = 0;
                ungetc(c, fd);

                token.codigo = MENOR_QUE;

                return token;
            }
            break;
        case 30:
            if (c == '=')
            {
                estado = 31;

                token.codigo = MAIOR_IGUAL;

                return token;
            }
            else
            {
                estado = 0;
                ungetc(c, fd);

                token.codigo = MAIOR_QUE;

                return token;
            }
            break;
        case 39:
            if (c == '&')
            {
                estado = 40;

                token.cat = OP_LOGIC;

                token.codigo = AND_LOGIC;

                return token;
            }
            else
            {
                estado = 41;

                token.cat = SINAL;

                ungetc(c, fd);

                token.codigo = REFERENCIA;

                return token;
            }
            break;
        case 42:
            if (c == '|')
            {
                estado = 43;

                token.cat = OP_LOGIC;

                token.codigo = OR_LOGIC;

                return token;
            }
            else
            {
                error(contLinha, c);
            }
            break;
        }
    }
}