#ifndef SEMANTICO_H
#define SEMANTICO_H

#include "Token.h"
#include "SemanticError.h"
#include <list>

using namespace std;



class Simbolo{
public:
    string tipo;
    string id;
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

class Warning{
public:
    int escopo;
    string id;
    string aviso;
};


class Semantico
{
public:
    void executeAction(int action, const Token *token) throw (SemanticError );


    list<Warning> listaWar;
    list<Simbolo> tabelaSimbolo;
    list<Simbolo> tabelaSimboloFuncoes;

};




#endif
