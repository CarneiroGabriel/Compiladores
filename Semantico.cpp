#include "Semantico.h"
#include "Constants.h"
#include "semantictable.h"

#include <iostream>
#include <list>
#include<stack>


using namespace std;

class Simbolo{
public:  string tipo;
         string id;
         bool inicializado = false;
         bool usado = false;
         bool parametro = false;
         bool funcao = false;
         bool vetor = false;
         int posVetor = false;
         int escopo;

    bool DescobrirTipo{
       // if(tipo == "string"){

        //}

    };
    void DeclararTipo(string t){
        tipo = t;
    };

};
SemanticTable TabelaSemantica;
Simbolo simboloVar;
Simbolo varInit;
list<Simbolo> tabelaSimbolo;
stack<list<Simbolo>> escopo;
int contadorEscopo =0;
stack<list<Simbolo>> tabelaSimbolosEscopo;


void Semantico::executeAction(int action, const Token *token) throw (SemanticError)
{
   // cout << "Ação: " << action << ", Token: "  << token->getId()
     //    << ", Lexema: " << token->getLexeme() << endl;


    simboloVar.escopo = contadorEscopo;
    string lexema = token->getLexeme();

    switch(action){
        case 1:
            simboloVar.DeclararTipo(lexema);
            break;

        case 2:
            cout<<"\n escopo variavel:" <<contadorEscopo<<endl;
            for (const Simbolo &simboloFor : tabelaSimbolo){
                if(lexema == simboloFor.id && contadorEscopo == simboloFor.escopo){
                    //disparar erro semantico
                    cout<<"erro semantico";
                    throw;
                    break;
                }else{
                    //inserir registro na lista de simbolos
                    //talvez precisa de um continue para o for
                    simboloVar.id = lexema;
                    simboloVar.escopo = contadorEscopo;
                    tabelaSimbolo.push_front(simboloVar);
                    break;
                }
            }
            if(tabelaSimbolo.empty()){
                simboloVar.id = lexema;
                simboloVar.escopo = contadorEscopo;
                tabelaSimbolo.push_front(simboloVar);
                break;
            }
            break;

        case 3:
            for (Simbolo simboloFor : tabelaSimbolo){
                if(lexema == simboloFor.id && simboloFor.usado){
                    cout<< "variavelfoi usada";
                }else if(lexema == simboloFor.id && contadorEscopo >= simboloFor.escopo){
                    //tabelaSimbolo.remove(simboloFor);
                    simboloFor.usado = true;
                    //simboloFor.escopo = contadorEscopo;
                    tabelaSimbolo.push_front(simboloFor);
                    break;
                }
            }
                break;

        case 4:
            for (Simbolo simboloFor : tabelaSimbolo){
                if(lexema == simboloFor.id && contadorEscopo >= simboloFor.escopo){
                    varInit = simboloFor;
                    cout<<"to ak "<< varInit.id<<endl;
                    break;
                }else{
                    //throw;
                    //lancar exessão
                    break;
                }
            }
                break;
        case 5:
                //tabelaSimbolo.remove(simboloVar);
                varInit.inicializado = true;
                tabelaSimbolo.push_front(varInit);


                /*switch(simboloVar.tipo){
                    case "int":
                    TabelaSemantica.atribType(0, 1);
                    break;

                }*/


                break;
        case 10:
                //tabelaSimbolo.remove(simboloVar);
                varInit =  tabelaSimbolo.front();
                tabelaSimbolo.pop_front();
                varInit.inicializado = true;
                tabelaSimbolo.push_front(varInit);
                break;

        case 18:
            cout<<"\n escopo:" <<simboloVar.escopo<<endl;
            escopo.push(tabelaSimbolo);
            contadorEscopo++;
            break;
        case 19:
            //SEI LA OQ FAZER NO FIM DO }
            break;

        case 37:
            contadorEscopo = 0;
            for(Simbolo sim : tabelaSimbolo){
                cout<<sim.tipo<<" "<<sim.id<<" escopo:"<<sim.escopo<<" init:"<<sim.inicializado<<" usada:"<<sim.usado<<endl;
            }
            tabelaSimbolo.clear();
            for(Simbolo sim : tabelaSimbolo){
                cout<<sim.tipo<<" "<<sim.id<<" escopo:"<<sim.escopo<<" init:"<<sim.inicializado<<" usada:"<<sim.usado<<endl;
            }
            tabelaSimbolosEscopo = escopo;
            while (!escopo.empty()) {
                escopo.pop();
            }

    }

}

