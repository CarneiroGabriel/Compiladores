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

    bool DescobrirTipo{
       // if(tipo == "string"){

        //}

    };
    void DeclararTipo(string t){
        tipo = t;
    };

};

Simbolo simboloVar;
list<Simbolo> tabelaSimbolo;
stack<list<Simbolo>> escopo;
stack<list<Simbolo>> tabelaSimbolosEscopo;


void Semantico::executeAction(int action, const Token *token) throw (SemanticError)
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
            cout<<"\n escopo variavel:" <<escopo.size()<<endl;
            for (const Simbolo &simboloFor : tabelaSimbolo){
                if(lexema == simboloFor.id && escopo.size() == simboloFor.escopo){
                    //disparar erro semantico
                    cout<<"erro semantico";
                    //throw;
                    break;
                }else{
                    //inserir registro na lista de simbolos
                    //talvez precisa de um continue para o for
                    simboloVar.id = lexema;
                    simboloVar.escopo = escopo.size();
                    tabelaSimbolo.push_front(simboloVar);
                    break;
                }
            }
            if(tabelaSimbolo.empty()){
                simboloVar.id = lexema;
                tabelaSimbolo.push_front(simboloVar);
                break;
            }
            break;

        case 3:
            for (Simbolo simboloFor : tabelaSimbolo){
                if(lexema == simboloFor.id && escopo.size() >= simboloFor.escopo){
                    //tabelaSimbolo.remove(simboloFor);
                    simboloFor.usado = true;
                    tabelaSimbolo.push_front(simboloFor);
                    break;
                }
            }
                break;

        case 4:
            for (Simbolo simboloFor : tabelaSimbolo){
                if(lexema == simboloFor.id && escopo.size() >= simboloFor.escopo){
                    Simbolo atribuirValor = simboloFor;
                }else{
                    //throw;
                    //lancar exessão
                    break;
                }
            }
                break;


        case 18:
            simboloVar.escopo++;
            cout<<"\n escopo:" <<simboloVar.escopo<<endl;
            escopo.push(tabelaSimbolo);
            break;
        case 19:
            //SEI LA OQ FAZER NO FIM DO }
            break;

        case 37:
            for(Simbolo sim : tabelaSimbolo){
                cout<<sim.tipo<<" "<<sim.id<<" escopo:"<<sim.escopo<<endl;
            }
            tabelaSimbolo.clear();
            tabelaSimbolosEscopo = escopo;
            while (!escopo.empty()) {
                escopo.pop();
            }

    }

}

