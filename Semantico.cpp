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
Simbolo *varInit; // usada pra iniciar variavel
Simbolo auxDeleteTable;// auxiliar para deletar simbolos repetidos da tabela
Warning warning;
list<Simbolo> tabelaSimboloAuxDelete;// auxiliar para deletar simbolos repetidos da tabela
stack<list<Simbolo>> escopo;
stack<int> operadoresUsados;
stack<int> atributosUsados;
int posVetor;
int tipoUsado;
int tipo0;
int tipo1;
int tipoAtr;
int tipoOperador;
int compativel;
int compativelAtr = 0;
bool VarExiste;
bool varUsadaPropria;

int ConverteTipo(string tipo){
    if(tipo == "int"){
        return 0;
    }else if(tipo == "float"){
        return 1;
    }else if(tipo == "char"){
        return 2;
    }else if(tipo == "string"){
        return 3;
    }else if(tipo == "bool"){
        return 4;
    }
}


void Semantico::executeAction(int action, const Token *token) throw (SemanticError)
{
    cout << "Ação: " << action << ", Token: "  << token->getId()
         << ", Lexema: " << token->getLexeme() << endl;

    simboloVar.escopo = escopo.empty() ? escopo.size(): 0;
    string lexema = token->getLexeme();

    switch(action){
        case 1:
            simboloVar.DeclararTipo(lexema);
            break;

        case 2:
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
            cout<<"Lexema caso 3 : "<<lexema;
            for (Simbolo simboloFor : tabelaSimbolo){
                auxDeleteTable = tabelaSimbolo.front();
                tabelaSimboloAuxDelete.push_front(auxDeleteTable);
                if(lexema == simboloFor.id && simboloFor.usado && escopo.size() >= simboloFor.escopo){
                    cout<< "variavelfoi usada";
                    tipoUsado = ConverteTipo(simboloFor.tipo);
                    atributosUsados.push(tipoUsado);
                    VarExiste = true;
                    if(!simboloFor.inicializado){
                        warning.id = simboloFor.id;
                        warning.escopo = simboloFor.escopo;
                        warning.aviso = "Usado sem ser iniciado";
                        listaWar.push_front(warning);
                    }

                }else if(lexema == simboloFor.id && escopo.size() >= simboloFor.escopo){
                    //tabelaSimbolo.remove(simboloFor);
                    if(auxDeleteTable.id == simboloFor.id  && auxDeleteTable.tipo == simboloFor.tipo  && auxDeleteTable.escopo <= simboloFor.escopo){
                        simboloFor.usado = true;
                        tabelaSimboloAuxDelete.pop_front();
                        tabelaSimboloAuxDelete.push_front(simboloFor);
                        tipoUsado = ConverteTipo(simboloFor.tipo);
                        VarExiste = true;
                        if(!simboloFor.inicializado){
                            warning.id = simboloFor.id;
                            warning.escopo = simboloFor.escopo;
                            warning.aviso = "Usado sem ser iniciado";
                            listaWar.push_front(warning);
                        }
                    }
                }
                tabelaSimbolo.pop_front();
            }
            if(varInit != nullptr){
                tipoUsado = ConverteTipo(varInit->tipo);
                atributosUsados.push(tipoUsado);
                varInit->usado = true;
                VarExiste = true;
                varUsadaPropria = false;

            }


            tabelaSimbolo.swap(tabelaSimboloAuxDelete);
            tabelaSimboloAuxDelete.clear();
            if(VarExiste){
                VarExiste = false;
                break;
            }
            throw SemanticError("Variavel nao exite", token->getPosition());
            break;

        case 4:
            for (Simbolo simboloFor : tabelaSimbolo){
                auxDeleteTable = tabelaSimbolo.front();
                tabelaSimboloAuxDelete.push_front(auxDeleteTable);
                if(lexema == simboloFor.id && escopo.size() >= simboloFor.escopo){
                    if(auxDeleteTable.id == simboloFor.id  && auxDeleteTable.tipo == simboloFor.tipo  && auxDeleteTable.escopo <= simboloFor.escopo){
                        varInit = &simboloFor;
                        cout<<"to ak "<< varInit->id<<endl;
                        tipoAtr = ConverteTipo(varInit->tipo);
                        VarExiste = true;
                        tabelaSimboloAuxDelete.pop_front();
                        varUsadaPropria = true;
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
                if(compativel == 0){
                varInit->inicializado = true;
                    tabelaSimbolo.push_front(*varInit);
                    break;
                }else if(compativel == 1){
                    varInit->inicializado = true;
                    tabelaSimbolo.push_front(*varInit);
                    warning.id = varInit->id;
                    warning.escopo = varInit->escopo;
                    warning.aviso = "Perda de precisao";
                    listaWar.push_front(warning);
                }else{
                  throw SemanticError("Tipo incompativel ", token->getPosition());
                }
                break;

        case 6:
                tipoUsado = 2;
                atributosUsados.push(tipoUsado);
                //char
                break;
        case 7:
                tipoUsado = 3;
                atributosUsados.push(tipoUsado);
                //string
                break;
        case 8:
                tipoUsado = 0;
                atributosUsados.push(tipoUsado);
                //int
                break;
        case 9:
                tipoUsado = 1;
                atributosUsados.push(tipoUsado);
                //float
                break;
        case 11:
                tipoUsado = 4;
                atributosUsados.push(tipoUsado);
                //bool
                break;
        case 10:
                //tabelaSimbolo.remove(simboloVar);
                varInit =  &tabelaSimbolo.front();
                tipoAtr = ConverteTipo(varInit->tipo);
                tabelaSimbolo.pop_front();

                if(!operadoresUsados.empty()){

                  compativelAtr = TabelaSemantica.resultType(tipoAtr, tipoUsado,tipoOperador);
                  tipoUsado = compativelAtr;
                  tipoOperador = NULL;
                }
                if( TabelaSemantica.atribType(tipoAtr, tipoUsado) == -1){
                  throw SemanticError("Tipo imcompativel ", token->getPosition());
                }else if(TabelaSemantica.atribType(tipoAtr, tipoUsado) == 1){
                  //war
                  compativel = 1;
                }else{
                  compativel = 0;
                };

                if(compativel == 0 ){
                  varInit->inicializado = true;
                  tabelaSimbolo.push_front(*varInit);
                  break;
                }else if(compativel == 1){
                  varInit->inicializado = true;
                  tabelaSimbolo.push_front(*varInit);
                  warning.id = varInit->id;
                  warning.escopo = varInit->escopo;
                  warning.aviso = "Perda de precisao";
                  listaWar.push_front(warning);
                }else{
                  throw SemanticError("Tipo incompativel ", token->getPosition());
                }
                break;

        case 12:

                if(!operadoresUsados.empty()){
                  while (!operadoresUsados.empty()) {
                    tipo0 = atributosUsados.top();
                    atributosUsados.pop();
                    tipo1 = atributosUsados.top();
                    atributosUsados.pop();
                    tipoOperador = operadoresUsados.top();
                    operadoresUsados.pop();

                    compativelAtr = TabelaSemantica.resultType(tipo0, tipo1,tipoOperador);
                    atributosUsados.push(compativelAtr);
                  }

                  tipoUsado = compativelAtr;
                }
                if( TabelaSemantica.atribType(tipoAtr, tipoUsado) == -1){
                  compativel = -1;
                }else if(TabelaSemantica.atribType(tipoAtr, tipoUsado) == 1){
                     //war
                     compativel = 1;
                }else{
                     compativel = 0;
                };

                break;

        case 13:
                tipoOperador = 0;
                operadoresUsados.push(tipoOperador);
                break;
        case 14:
                tipoOperador = 1;
                operadoresUsados.push(tipoOperador);
                break;
        case 15:
                tipoOperador = 2;
                operadoresUsados.push(tipoOperador);
                break;
        case 16:
                tipoOperador = 3;
                operadoresUsados.push(tipoOperador);
                break;
        case 17:
                tipoOperador = 4;
                operadoresUsados.push(tipoOperador);
                break;
        case 22:
                tipoOperador = 5;
                operadoresUsados.push(tipoOperador);
                break;

        case 18:
            cout<<"\n escopo:" <<simboloVar.escopo<<endl;
            escopo.push(tabelaSimbolo);
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
                        varInit = &simboloFor;
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
                    simboloVar.escopo = escopo.size() + 1;
                    tabelaSimbolo.push_front(simboloVar);
                    simboloVar.parametro = false;
                    break;
                     }
            }
            if(tabelaSimbolo.empty()){
                     simboloVar.parametro = true;
                     simboloVar.id = lexema;
                     simboloVar.escopo = escopo.size() + 1;
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
            while (!escopo.empty()) {
                escopo.pop();
            }

            break;

        case 30:

            auxDeleteTable = tabelaSimbolo.front();
            tabelaSimbolo.pop_front();
            auxDeleteTable.escopo++;
            tabelaSimbolo.push_front(auxDeleteTable);

            break;

        case 31:
            if(tipoAtr == 1 || tipoAtr == 0){
                compativel = 0;
            }
            break;

    }

}

