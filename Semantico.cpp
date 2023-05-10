#include "Semantico.h"
#include "Constants.h"
#include "semantictable.h"

#include <iostream>
#include <list>
#include<stack>


using namespace std;

void Simbolo::DeclararTipo(std::string t){
    tipo = t;
};

SemanticTable TabelaSemantica;
Simbolo simboloVar; // usada pra auxiliar em inserções na tabela
Simbolo varInit; // usada pra iniciar variavel
Simbolo auxDeleteTable;// auxiliar para deletar simbolos repetidos da tabela
Warning warning;
//MAIN
//list<Simbolo> tabelaSimbolo;
//list<Simbolo> tabelaSimboloFuncoes;
//--------
list<Simbolo> tabelaSimboloAuxDelete;// auxiliar para deletar simbolos repetidos da tabela
stack<list<Simbolo>> escopo;
int posVetor;
int contadorEscopo =0;
int tipoUsado;
int tipoAtr;
int tipoOperador;
int compativel;
int compativelAtr = 0;
bool VarExiste;
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
            //cout<<"\n escopo variavel:" <<contadorEscopo<<endl;
            for (const Simbolo &simboloFor : tabelaSimbolo){
                if(lexema == simboloFor.id && escopo.size() == simboloFor.escopo){
                    //disparar erro semantico
                    //cout<<"erro semantico";
                    throw SemanticError("Variavel declarada 2 vezes", token->getPosition());
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
                auxDeleteTable = tabelaSimbolo.front();
                tabelaSimboloAuxDelete.push_front(auxDeleteTable);
                if(lexema == simboloFor.id && simboloFor.usado){
                    cout<< "variavelfoi usada";
                    tipoUsado = ConverteTipo(simboloVar.tipo);
                }else if(lexema == simboloFor.id && escopo.size() >= simboloFor.escopo){
                    //tabelaSimbolo.remove(simboloFor);
                    if(auxDeleteTable.id == simboloFor.id  && auxDeleteTable.tipo == simboloFor.tipo  && auxDeleteTable.escopo <= simboloFor.escopo){
                        simboloFor.usado = true;
                        tabelaSimboloAuxDelete.pop_front();
                        tabelaSimboloAuxDelete.push_front(simboloFor);
                        tipoUsado = ConverteTipo(simboloVar.tipo);
                    }

                    //simboloFor.escopo = contadorEscopo;
                    //tabelaSimbolo.push_front(simboloFor);
                }
                tabelaSimbolo.pop_front();
            }
            tabelaSimbolo.swap(tabelaSimboloAuxDelete);
            tabelaSimboloAuxDelete.clear();
            break;

        case 4:
            for (Simbolo simboloFor : tabelaSimbolo){
                auxDeleteTable = tabelaSimbolo.front();
                tabelaSimboloAuxDelete.push_front(auxDeleteTable);
                if(lexema == simboloFor.id && escopo.size() >= simboloFor.escopo){
                    if(auxDeleteTable.id == simboloFor.id  && auxDeleteTable.tipo == simboloFor.tipo  && auxDeleteTable.escopo <= simboloFor.escopo){
                        varInit = simboloFor;
                        cout<<"to ak "<< varInit.id<<endl;
                        tipoAtr = ConverteTipo(simboloVar.tipo);
                        VarExiste = true;
                        tabelaSimboloAuxDelete.pop_front();
                    }

                }
                tabelaSimbolo.pop_front();
            }
            tabelaSimbolo.swap(tabelaSimboloAuxDelete);
            tabelaSimboloAuxDelete.clear();
            if(VarExiste){
                VarExiste = false;
                break;
            }
                throw SemanticError("Variavel nao exite", token->getPosition());
                break;

        case 5:
                //tabelaSimbolo.remove(simboloVar);
                if(compativel == 0 || compativelAtr == 0 ){
                varInit.inicializado = true;
                tabelaSimbolo.push_front(varInit);
                break;
                }else if(compativel == 1 || compativelAtr == 1 ){
                varInit.inicializado = true;
                tabelaSimbolo.push_front(varInit);
                warning.id = varInit.id;
                warning.escopo = varInit.escopo;
                warning.aviso = "Perda de precisao";
                listaWar.push_front(warning);
                }else{
                  throw SemanticError("Tipo incompativel ", token->getPosition());
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
                if(TabelaSemantica.resultType(tipoAtr, tipoUsado,tipoOperador) == -1){
                  throw SemanticError("Tipo imcompativel ", token->getPosition());
                } else if(TabelaSemantica.resultType(tipoAtr, tipoUsado,tipoOperador) == 1){
                    compativelAtr = 1;
                } else{
                    compativelAtr = 0;
                }


                if( TabelaSemantica.atribType(tipoAtr, tipoUsado) == -1){
                     throw SemanticError("Tipo imcompativel ", token->getPosition());
                }else if(TabelaSemantica.atribType(tipoAtr, tipoUsado) == 1){
                     //war
                     compativel = 1;
                }else{
                     compativel = 0;
                };

                break;

        case 13:
                tipoOperador = 0;
                break;
        case 14:
                tipoOperador = 1;
                break;
        case 15:
                tipoOperador = 2;
                break;
        case 16:
                tipoOperador = 3;
                break;
        case 17:
                tipoOperador = 4;
                break;
        case 22:
                tipoOperador = 5;
                break;

        case 18:
            cout<<"\n escopo:" <<simboloVar.escopo<<endl;
            escopo.push(tabelaSimbolo);
            tabelaSimbolosEscopo.push(tabelaSimbolo);
            break;
        case 19:
            escopo.pop();
            if(escopo.size() == 0){
                for (Simbolo simboloFor : tabelaSimbolo){
                    auxDeleteTable = tabelaSimbolo.front();
                    tabelaSimboloAuxDelete.push_front(auxDeleteTable);
                    if( simboloFor.escopo == 1){
                             if( auxDeleteTable.escopo == simboloFor.escopo){

                             tabelaSimboloFuncoes.push_front(simboloFor);
                             tabelaSimboloAuxDelete.pop_front();
                             }

                    }
                    tabelaSimbolo.pop_front();
                }
                tabelaSimbolo.swap(tabelaSimboloAuxDelete);
                tabelaSimboloAuxDelete.clear();
            }
            break;

        case 20:
            for (const Simbolo &simboloFor : tabelaSimbolo){
                     if(lexema == simboloFor.id && escopo.size() == simboloFor.escopo){
                    throw SemanticError("Variavel || funcao declarada 2 vezes", token->getPosition());
                    break;
                     }else{
                    //inserir registro na lista de simbolos
                    //talvez precisa de um continue para o for
                    simboloVar.funcao = true;
                    simboloVar.id = lexema;
                    simboloVar.escopo = escopo.size();
                    tabelaSimbolo.push_front(simboloVar);
                    simboloVar.funcao = false;
                    break;
                     }
            }
            if(tabelaSimbolo.empty()){
                     simboloVar.funcao = true;
                     simboloVar.id = lexema;
                     simboloVar.escopo = escopo.size();
                     tabelaSimbolo.push_front(simboloVar);
                     simboloVar.funcao = false;
                     break;
            }
            break;

        case 21:

            simboloVar = tabelaSimbolo.front();
            simboloVar.vetor = true;
            simboloVar.posVetor = posVetor;
            tabelaSimbolo.pop_front();
            tabelaSimbolo.push_front(simboloVar);
            simboloVar.vetor = false;
            simboloVar.posVetor = 0;
            break;

        case 23:
            for (Simbolo simboloFor : tabelaSimbolo){
                     auxDeleteTable = tabelaSimbolo.front();
                     tabelaSimboloAuxDelete.push_front(auxDeleteTable);
                     if(lexema == simboloFor.id && escopo.size() >= simboloFor.escopo && simboloFor.funcao){
                    if(auxDeleteTable.id == simboloFor.id  && auxDeleteTable.tipo == simboloFor.tipo  && auxDeleteTable.escopo <= simboloFor.escopo){
                        tipoAtr = ConverteTipo(simboloVar.tipo);
                        simboloFor.usado = true;
                        simboloFor.inicializado = true;
                        VarExiste = true;
                        tabelaSimboloAuxDelete.pop_front();
                        tabelaSimboloAuxDelete.push_front(simboloFor);
                    }
                     }
                     tabelaSimbolo.pop_front();
            }
            tabelaSimbolo.swap(tabelaSimboloAuxDelete);
            tabelaSimboloAuxDelete.clear();
            if(VarExiste){
                     VarExiste = false;
                     break;
            }
            throw SemanticError("Funcao nao exite", token->getPosition());
            break;

        case 24:
            for (Simbolo simboloFor : tabelaSimbolo){
                     auxDeleteTable = tabelaSimbolo.front();
                     tabelaSimboloAuxDelete.push_front(auxDeleteTable);
                     if(lexema == simboloFor.id && escopo.size() >= simboloFor.escopo && simboloFor.vetor && simboloFor.posVetor >= posVetor){
                    if(auxDeleteTable.id == simboloFor.id  && auxDeleteTable.tipo == simboloFor.tipo  && auxDeleteTable.escopo <= simboloFor.escopo){
                        varInit = simboloFor;
                        tipoAtr = ConverteTipo(simboloVar.tipo);
                        VarExiste = true;
                        if(simboloFor.posVetor == posVetor){
                        tabelaSimboloAuxDelete.pop_front();
                        }
                    }
                     }
                     tabelaSimbolo.pop_front();
            }
            tabelaSimbolo.swap(tabelaSimboloAuxDelete);
            tabelaSimboloAuxDelete.clear();
            if(VarExiste){
                     VarExiste = false;
                     break;
            }
            throw SemanticError("vetor nao exite", token->getPosition());
            break;

        case 25:

            for (Simbolo simboloFor : tabelaSimbolo){
                     auxDeleteTable = tabelaSimbolo.front();
                     tabelaSimboloAuxDelete.push_front(auxDeleteTable);
                     if(lexema == simboloFor.id && simboloFor.usado){
                    cout<< "variavelfoi usada";
                    tipoUsado = ConverteTipo(simboloVar.tipo);
                     }else if(lexema == simboloFor.id && escopo.size() >= simboloFor.escopo){
                    if(auxDeleteTable.id == simboloFor.id  && auxDeleteTable.tipo == simboloFor.tipo  && auxDeleteTable.escopo <= simboloFor.escopo && simboloFor.posVetor >= posVetor){
                        simboloFor.usado = true;
                        tabelaSimboloAuxDelete.pop_front();
                        tabelaSimboloAuxDelete.push_front(simboloFor);
                        tipoUsado = ConverteTipo(simboloVar.tipo);
                    }

                    //simboloFor.escopo = contadorEscopo;
                    //tabelaSimbolo.push_front(simboloFor);
                     }
                     tabelaSimbolo.pop_front();
            }
            tabelaSimbolo.swap(tabelaSimboloAuxDelete);
            tabelaSimboloAuxDelete.clear();
            break;


        case 26:
            for (const Simbolo &simboloFor : tabelaSimbolo){
                     if(lexema == simboloFor.id && escopo.size() == simboloFor.escopo){
                    throw SemanticError("Variavel || funcao declarada 2 vezes", token->getPosition());
                    break;
                     }else{
                    //inserir registro na lista de simbolos
                    //talvez precisa de um continue para o for
                    simboloVar.parametro = true;
                    simboloVar.id = lexema;
                    simboloVar.escopo = escopo.size();
                    tabelaSimbolo.push_front(simboloVar);
                    simboloVar.parametro = false;
                    break;
                     }
            }
            if(tabelaSimbolo.empty()){
                     simboloVar.parametro = true;
                     simboloVar.id = lexema;
                     simboloVar.escopo = escopo.size();
                     tabelaSimbolo.push_front(simboloVar);
                     simboloVar.parametro = false;
                     break;
            }
            break;

        case 27:
            posVetor = stoi(lexema);
            //cout << posVetor;
            break;

        case 28:
            posVetor = stoi(lexema);
            //cout << posVetor;
            break;


        case 29:
            tabelaSimbolosEscopo.push(tabelaSimbolo);

            contadorEscopo = 0;
            /*for(Simbolo sim : tabelaSimbolo){
                     cout<<sim.tipo<<" "<<sim.id<<" escopo:"<<sim.escopo<<" init:"<<sim.inicializado<<" usada:"<<sim.usado<<" funcao:"<<sim.funcao<<" vetor:"<<sim.vetor<<" posVetor:"<<sim.posVetor<<" paremtro:"<<sim.parametro<<endl;
            }
            tabelaSimbolo.clear();

            cout << tabelaSimboloFuncoes.size();
            /*
            for(Simbolo sim : tabelaSimboloFuncoes){
                     cout<< "-------------------------------------------------------------------------";
                cout<<sim.tipo<<" "<<sim.id<<" escopo:"<<sim.escopo<<" init:"<<sim.inicializado<<" usada:"<<sim.usado<<" funcao:"<<sim.funcao<<" vetor:"<<sim.vetor<<" posVetor:"<<sim.posVetor<<" paremtro:"<<sim.parametro<<endl;
            }*/
            tabelaSimbolosEscopo = escopo;
            while (!escopo.empty()) {
                escopo.pop();
            }

            break;

    }

}

