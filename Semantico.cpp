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
list<Simbolo> tabelaSimboloBkp;
stack<list<Simbolo>> escopo;
int contadorEscopo =0;
int tipoUsado;
int tipoAtr;
int tipoOperador;
int compativel;
stack<list<Simbolo>> tabelaSimbolosEscopo;

int ConverteTipo(string tipo){
    if(tipo == "int"){
        return 0;
    }else if(tipo == "float"){
        return 1;
    }else if(tipo == "char"){
        return 2;
    }else if(tipo == "string"){
        return 3;
    }else if(tipo == "int"){
        return 4;
    }
}


void Semantico::executeAction(int action, const Token *token) throw (SemanticError)
{
   // cout << "Ação: " << action << ", Token: "  << token->getId()
     //    << ", Lexema: " << token->getLexeme() << endl;


    contadorEscopo = escopo.empty() ? escopo.size(): 0;
    simboloVar.escopo = contadorEscopo;
    string lexema = token->getLexeme();

    switch(action){
        case 1:
            simboloVar.DeclararTipo(lexema);
            break;

        case 2:
            cout<<"\n escopo variavel:" <<contadorEscopo<<endl;
            for (const Simbolo &simboloFor : tabelaSimbolo){
                if(lexema == simboloFor.id && escopo.size() == simboloFor.escopo){
                    //disparar erro semantico
                    cout<<"erro semantico";
                    throw SemanticError("Erro semantico ", token->getPosition());
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
                simboloVar.escopo = escopo.size();
                tabelaSimbolo.push_front(simboloVar);
                break;
            }
            break;

        case 3:
            for (Simbolo simboloFor : tabelaSimbolo){
                if(lexema == simboloFor.id && simboloFor.usado){
                    cout<< "variavelfoi usada";
                    tipoUsado = ConverteTipo(simboloVar.tipo);
                }else if(lexema == simboloFor.id && escopo.size() >= simboloFor.escopo){
                    //tabelaSimbolo.remove(simboloFor);
                    simboloFor.usado = true;
                    tipoUsado = ConverteTipo(simboloVar.tipo);
                    //simboloFor.escopo = contadorEscopo;
                    tabelaSimbolo.push_front(simboloFor);
                    break;
                }
            }
                break;

        case 4:
            for (Simbolo simboloFor : tabelaSimbolo){
                if(lexema == simboloFor.id && escopo.size() >= simboloFor.escopo){
                    varInit = simboloFor;
                    cout<<"to ak "<< varInit.id<<endl;
                    tipoAtr = ConverteTipo(simboloVar.tipo);
                    break;
                }else{
                    throw SemanticError("Erro semantico ", token->getPosition());
                    //lancar exessão
                    break;
                }
            }
                break;
        case 5:
                //tabelaSimbolo.remove(simboloVar);
                if(compativel == 0){
                varInit.inicializado = true;
                tabelaSimbolo.push_front(varInit);
                break;
                }else{
                  throw SemanticError("Erro semantico ", token->getPosition());
                }
                break;

        case 6:
                tipoUsado = 2;
                //char
                break;
        case 7:
                tipoUsado = 3;
                //string
                break;
        case 8:
                tipoUsado = 0;
                //int
                break;
        case 9:
                tipoUsado = 1;
                //float
                break;
        case 11:
                tipoUsado = 4;
                //bool
                break;
        case 10:
                //tabelaSimbolo.remove(simboloVar);
                varInit =  tabelaSimbolo.front();
                tabelaSimbolo.pop_front();
                varInit.inicializado = true;
                tabelaSimbolo.push_front(varInit);
                break;
        case 12:
                if( TabelaSemantica.atribType(tipoAtr, tipoUsado) == -1){
                     throw SemanticError("Erro semantico ", token->getPosition());
                }else if(TabelaSemantica.atribType(tipoAtr, tipoUsado) == 1){
                     //war
                }else{
                     compativel = 0;
                };

                break;

        case 13:
                tipoOperador = 0;
                break;
        case 14:
                tipoOperador = 0;
                break;
        case 15:
                tipoOperador = 0;
                break;
        case 16:
                tipoOperador = 0;
                break;
        case 17:
                tipoOperador = 0;
                break;

        case 18:
            cout<<"\n escopo:" <<simboloVar.escopo<<endl;
            escopo.push(tabelaSimbolo);
            tabelaSimbolosEscopo.push(tabelaSimbolo);
            break;
        case 19:
            tabelaSimboloBkp.swap(tabelaSimbolo);
            tabelaSimbolo.clear();
            for(Simbolo sim : tabelaSimbolo){
                     if(sim.escopo == escopo.size()){
                     tabelaSimbolo.pop_front();
                     }
            }

            escopo.pop();

            break;

        case 20:

            break;

        case 37:
            tabelaSimbolosEscopo.push(tabelaSimbolo);

            contadorEscopo = 0;
            for(Simbolo sim : tabelaSimboloBkp){
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

