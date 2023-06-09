#ifndef SEMANTICO_H
#define SEMANTICO_H

#include "Token.h"
#include "SemanticError.h"
#include <list>
#include <stack>

using namespace std;



class Simbolo{
public:
    string tipo;
    string id;
    string nomeFun;
    bool inicializado = false;
    bool usado = false;
    bool parametro = false;
    bool funcao = false;
    bool vetor = false;
    int posVetor = false;
    int valor = 0;
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
    stack<int> temporarioDisponivel;
    stack<int> temporarioUsado;
    string data;
    string text;

};




#endif
