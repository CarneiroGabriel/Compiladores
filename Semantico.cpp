#include "Semantico.h"
#include "Constants.h"

#include <iostream>
#include <list>
#include<vector>
#include<stack>

using namespace std;

class Simbolo{
public: string tipo;
public: string id;
public: bool inicializado = false;
public: bool usado = false;
public: bool parametro = false;
public: bool funcao = false;
public: bool vetor = false;
public: int posVetor = false;
public: int escopo;

    void DeclararTipo(string t){
        tipo = t;
    }

};

Simbolo simboloVar;
list<Simbolo> tabelaSimbolo;
stack<list<Simbolo>> escopo;


void Semantico::executeAction(int action, const Token *token) throw (SemanticError )
{
    cout << "Ação: " << action << ", Token: "  << token->getId()
         << ", Lexema: " << token->getLexeme() << endl;


    simboloVar.escopo = (escopo.empty()) ? escopo.size(): 0;
    string lexema = token->getLexeme();

    switch(action){
        case 1:
            simboloVar.DeclararTipo(lexema);
            break;

        case 2:
            cout<<"\n escopo variavel:" <<simboloVar.escopo<<endl;
            for (const Simbolo &simboloFor : tabelaSimbolo){
                if(lexema == simboloFor.id && simboloVar.escopo == simboloFor.escopo){
                    //disparar erro semantico
                    cout<<"erro no semantico";
                    break;
                }else{
                    //inserir registro na lista de simbolos
                    //talvez precisa de um continue para o for
                    simboloVar.id = lexema;
                    tabelaSimbolo.push_front(simboloVar);
                    break;
                }
            }
            if(tabelaSimbolo.empty()){
                simboloVar.id = lexema;
                tabelaSimbolo.push_front(simboloVar);
                break;
            }

        case 18:
            simboloVar.escopo++;
            cout<<"\n" <<simboloVar.escopo<<endl;
            escopo.push(tabelaSimbolo);
            break;
        case 19:
            escopo.pop();
            break;
    }

}

