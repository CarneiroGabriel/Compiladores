#ifndef CONSTANTS_H
#define CONSTANTS_H

enum TokenId 
{
    EPSILON  = 0,
    DOLLAR   = 1,
    t_BOOL_LIT = 2,
    t_ID = 3,
    t_BOOL_KEY = 4,
    t_INT_KEY = 5,
    t_INT_LIT = 6,
    t_CHAR_KEY = 7,
    t_CHAR_LIT = 8,
    t_STRING_KEY = 9,
    t_STRING_LIT = 10,
    t_FLOAT_KEY = 11,
    t_FLOAT_LIT = 12,
    t_CONST_KEY = 13,
    t_VAR_KEY = 14,
    t_VOID_KEY = 15,
    t_FUNCTION_KEY = 16,
    t_CONTINUE_KEY = 17,
    t_IF_KEY = 18,
    t_ELSE_KEY = 19,
    t_BEGIN_KEY = 20,
    t_END_KEY = 21,
    t_WHILE_KEY = 22,
    t_FOR_KEY = 23,
    t_BREAK_KEY = 24,
    t_RETURN_KEY = 25,
    t_MAIN_KEY = 26,
    t_ERROR_KEY = 27,
    t_PRINTF_KEY = 28,
    t_SCANF_KEY = 29,
    t_DO_KEY = 30,
    t_SWITCH_KEY = 31,
    t_CASE_KEY = 32,
    t_DEFAULT_KEY = 33,
    t_CIN_KEY = 34,
    t_COUT_KEY = 35,
    t_IGUAL_LIT = 36,
    t_SUM_LIT = 37,
    t_SUB_LIT = 38,
    t_MULTI_LIT = 39,
    t_DIV_LIT = 40,
    t_MOD_LIT = 41,
    t_IGUAL_LOG_KEY = 42,
    t_MENOR_KEY = 43,
    t_MAIOR_KEY = 44,
    t_MENOR_IGUAL_KEY = 45,
    t_MAIOR_IGUAL_KEY = 46,
    t_DIFERENTE_KEY = 47,
    t_OR_KEY = 48,
    t_AND_KEY = 49,
    t_NOT_KEY = 50,
    t_SRIGHT_KEY = 51,
    t_SLEFT_KEY = 52,
    t_AND_BIT_KEY = 53,
    t_OR_BIT_KEY = 54,
    t_XOR_BIT_KEY = 55,
    t_NOT_BIT_KEY = 56,
    t_HEX_LIT = 57,
    t_BIN_LIT = 58,
    t_PONTO_LIT = 59,
    t_VIRGULA_LIT = 60,
    t_PONTO_VIRGULA = 61,
    t_DOIS_PONTOS_LIT = 62,
    t_ABRE_PARENTES_LIT = 63,
    t_FECHA_PARENTES_LIT = 64,
    t_ABRE_CONCHETES_LIT = 65,
    t_FECHA_CONCHETES_LIT = 66,
    t_ABRE_CHAVES_LIT = 67,
    t_FECHA_CHAVES_LIT = 68
};

const int STATES_COUNT = 60;

extern int SCANNER_TABLE[STATES_COUNT][256];

extern int TOKEN_STATE[STATES_COUNT];

extern int SPECIAL_CASES_INDEXES[70];

extern const char *SPECIAL_CASES_KEYS[28];

extern int SPECIAL_CASES_VALUES[28];

extern const char *SCANNER_ERROR[STATES_COUNT];

const int FIRST_SEMANTIC_ACTION = 127;

const int SHIFT  = 0;
const int REDUCE = 1;
const int ACTION = 2;
const int ACCEPT = 3;
const int GO_TO  = 4;
const int ERROR  = 5;

extern const int PARSER_TABLE[306][157][2];

extern const int PRODUCTIONS[135][2];

extern const char *PARSER_ERROR[306];

#endif
