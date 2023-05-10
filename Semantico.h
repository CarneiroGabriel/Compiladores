#ifndef SEMANTICO_H
#define SEMANTICO_H

#include "Token.h"
#include "SemanticError.h"
#include <list>



class Simbolo{
public:  std::string tipo;
    std::string id;
    bool inicializado = false;
    bool usado = false;
    bool parametro = false;
    bool funcao = false;
    bool vetor = false;
    int posVetor = false;
    int escopo;

    void DeclararTipo(std::string t);
    //void DeclararTipo(std::string t){
    //    tipo = t;
    //};

};


class Semantico
{
public:
    void executeAction(int action, const Token *token) throw (SemanticError );


    std::list<Simbolo> tabelaSimbolo;
    std::list<Simbolo> tabelaSimboloFuncoes;

};




#endif
