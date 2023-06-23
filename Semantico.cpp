#include "Semantico.h"
#include "Constants.h"
#include "semantictable.h"

#include <iostream>
#include <list>
#include<stack>
#include <algorithm>


using namespace std;

void Simbolo::DeclararTipo(std::string t){
    tipo = t;
};

bool isNumeric(std::string const &str)
{
    return !str.empty() && std::all_of(str.begin(), str.end(), ::isdigit);
}



SemanticTable TabelaSemantica;
Simbolo simboloVar; // usada pra auxiliar em inser??es na tabela
Simbolo varInit; // usada pra iniciar variavel
Simbolo auxDeleteTable;// auxiliar para deletar simbolos repetidos da tabela
Warning warning;
list<Simbolo> tabelaSimboloAuxDelete;// auxiliar para deletar simbolos repetidos da tabela
stack< string> valorAtr;
stack< string> valorAtrRev;
stack<list<Simbolo>> escopo;
stack<int> operadoresUsados;
stack<int> atributosUsados;
stack<int> auxOperadoresUsados;
stack<string> auxTextVetor;
stack<string> vetorOperando;
stack<int> auxTemporario;
stack<int> auxAtribuirVetorVar;
list<int> rotulos;
string oprel;
string textTemp;
string nomeFun;
int controleRotulos;
int contador;
int posVetor;
int tamVetor;
int tipoUsado;
int auxTemp1;
int auxTemp0;
int tipo0;
int tipo1;
int tipoAtr;
int tipoOperador;
int compativel;
int compativelAtr = 0;
int diferencaValorAtrVetor;
int auxOperacaoVetor1;
int auxOperacaoVetor2;
bool VarExiste;
bool varUsadaPropria;

string descobreBranchIf(string rel){
    if(rel == ">"){
        return "\n BLT";
    }else if(rel == "<"){
        return "\n BGT";
    }else if(rel == ">="){
        return "\n BLE";
    }else if(rel == "<="){
        return "\n BGE";
    }else if(rel == "=="){
        return "\n BNE";
    }else if(rel == "!="){
        return "\n BEQ";
    }
}

string descobreBranchWhile(string rel){
    if(rel == ">"){
        return "\n BGT";
    }else if(rel == "<"){
        return "\n BLT";
    }else if(rel == ">="){
        return "\n BGE";
    }else if(rel == "<="){
        return "\n BLE";
    }else if(rel == "=="){
        return "\n BEQ";
    }else if(rel == "!="){
        return "\n BNE";
    }


}


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
    cout << "A??o: " << action << ", Token: "  << token->getId()
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
            for (Simbolo simboloFor : tabelaSimbolo){
                auxDeleteTable = tabelaSimbolo.front();
                tabelaSimboloAuxDelete.push_front(auxDeleteTable);
                if(lexema == simboloFor.id && simboloFor.usado && escopo.size() >= simboloFor.escopo){
                    cout<< "variavelfoi usada";
                    tipoUsado = ConverteTipo(simboloFor.tipo);
                    atributosUsados.push(tipoUsado);
                    valorAtr.push(lexema);
                    cout<<"Lexema caso 3 : "<<valorAtr.top();
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
                        valorAtr.push(lexema);
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
            if(varInit.id == lexema && !varInit.vetor){
                tipoUsado = ConverteTipo(varInit.tipo);
                atributosUsados.push(tipoUsado);
                varInit.usado = true;
                valorAtr.push(lexema);
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
                if(lexema == simboloFor.id && escopo.size() >= simboloFor.escopo && !simboloFor.vetor){
                    if(auxDeleteTable.id == simboloFor.id  && auxDeleteTable.tipo == simboloFor.tipo  && auxDeleteTable.escopo <= simboloFor.escopo){
                        varInit = simboloFor;
                        cout<<"to ak "<< varInit.id<<endl;
                        tipoAtr = ConverteTipo(varInit.tipo);
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

                cout<<"tamanho do auxOperadores inicio 5 "<<auxOperadoresUsados.size()<<endl;


                while(!valorAtr.empty()){
                valorAtrRev.push(valorAtr.top());
                valorAtr.pop();
                }
                    cout<<"\n 218 line \n";

                if(!valorAtrRev.empty()){

                    if(isNumeric(valorAtrRev.top())){
                        text.append("\n LDI ");
                        text.append(valorAtrRev.top());
                        valorAtrRev.pop();
                    }else{
                        text.append("\n LD ");
                        text.append(valorAtrRev.top());
                        valorAtrRev.pop();
                    }
                    if(!temporarioUsado.empty() && !auxOperadoresUsados.empty() && valorAtrRev.empty()){
                        text.append("\n STO ");
                        text.append(to_string(temporarioDisponivel.top()));
                        cout<< "\n tamanho temporario disponiveis"<< temporarioDisponivel.size();
                        temporarioUsado.push(temporarioDisponivel.top());

                        temporarioDisponivel.pop();

                    }
                    while(!valorAtrRev.empty()){
                        if(auxOperadoresUsados.top() == 1 || auxOperadoresUsados.top() == 5){
                            if(isNumeric(valorAtrRev.top())){
                                text.append("\n SUBI ");
                                text.append(valorAtrRev.top());
                                valorAtrRev.pop();
                            }else{
                                text.append("\n SUB ");
                                text.append(valorAtrRev.top());
                                valorAtrRev.pop();
                            }
                        }else if(auxOperadoresUsados.top() == 0){
                            if(isNumeric(valorAtrRev.top()) ){
                                text.append("\n ADDI ");
                                text.append(valorAtrRev.top());
                                valorAtrRev.pop();
                            }else{
                                text.append("\n ADD ");
                                text.append(valorAtrRev.top());
                                valorAtrRev.pop();
                            }
                        }
                        auxOperadoresUsados.pop();
                    }

                     }

                    cout<<"\n 256 line \n";





                if(!auxTextVetor.empty()){

                    if(temporarioUsado.size() >= 3){
                        auxTemp0 =temporarioUsado.top();
                        temporarioUsado.pop();
                        auxTemp1 =temporarioUsado.top();
                        temporarioUsado.pop();
                        temporarioUsado.push(auxTemp0);
                        temporarioUsado.push(auxTemp1);

                    }

                    cout<<"\n 277 line \n";
                    while(!temporarioUsado.empty()){
                        auxTemporario.push(temporarioUsado.top());
                        temporarioUsado.pop();
                    }
                    while(!auxTemporario.empty()){
                        auxTextVetor.push(to_string(auxTemporario.top()));
                        auxTemporario.pop();
                    }

                    if(auxTextVetor.size() > 2){
                    text.append("\n LD ");
                    text.append(auxTextVetor.top());
                    auxTextVetor.pop();
                    }


                    while (!auxTextVetor.empty()) {
                        if(auxTextVetor.size() > 2){
                        if(auxOperadoresUsados.top() == 1){
                                text.append("\n SUB ");
                                text.append(auxTextVetor.top());
                                if(isNumeric(auxTextVetor.top()) ){
                                 temporarioDisponivel.push(stoi(auxTextVetor.top()));
                                }
                                auxTextVetor.pop();
                        }else if(auxOperadoresUsados.top() == 0){
                                text.append("\n ADD ");
                                text.append(auxTextVetor.top());
                                if(isNumeric(auxTextVetor.top()) ){
                                    temporarioDisponivel.push(stoi(auxTextVetor.top()));
                                }
                                auxTextVetor.pop();
                        }
                        }else{
                        text.append("\n STO ");
                        text.append(to_string(temporarioDisponivel.top()));
                        cout<< "\n tamanho temporario disponiveis"<< temporarioDisponivel.size();
                        temporarioUsado.push(temporarioDisponivel.top());
                        temporarioDisponivel.pop();
                        break;
                        }
                        if(!auxOperadoresUsados.empty()){
                        auxOperadoresUsados.pop();
                        }
                    }
                    if(auxTextVetor.size() >= 2){
                    text.append("\n LD ");
                    text.append(auxTextVetor.top());
                    auxTextVetor.pop();
                    text.append("\n STO $INDR");
                    text.append("\n LD ");
                    text.append(to_string(temporarioUsado.top()));
                    temporarioUsado.pop();
                    text.append("\n STOV ");
                    text.append(auxTextVetor.top());
                    auxTextVetor.pop();
                    }
                }else{

                    if(temporarioUsado.size() >= 2){
                    auxTemp0 =temporarioUsado.top();
                    temporarioUsado.pop();
                    auxTemp1 =temporarioUsado.top();
                    temporarioUsado.pop();
                    temporarioUsado.push(auxTemp0);
                    temporarioUsado.push(auxTemp1);

                    }
                    if(!temporarioUsado.empty()){

                    cout<<"tamanho do auxOperadores "<<auxOperadoresUsados.size()<<endl;


                    while(!temporarioUsado.empty()){
                        auxTemporario.push(temporarioUsado.top());
                        temporarioUsado.pop();
                    }
                    while(!auxTemporario.empty()){
                        auxAtribuirVetorVar.push(auxTemporario.top());
                        auxTemporario.pop();
                    }

                    cout<<"tamanho do auxAtribuirVetor a Var  "<<auxAtribuirVetorVar.size()<<endl;

                    while(!auxOperadoresUsados.empty()){cout << "\n entroou no while ";
                        text.append("\n LD ");
                        text.append(to_string(auxAtribuirVetorVar.top()));
                        auxAtribuirVetorVar.pop();

                        cout << "\n entroou no while ";

                        if(auxOperadoresUsados.top() == 1){
                                text.append("\n SUB ");
                                text.append(to_string(auxAtribuirVetorVar.top()));;
                                temporarioDisponivel.push(auxAtribuirVetorVar.top());
                                auxAtribuirVetorVar.pop();
                                break;
                        }else if(auxOperadoresUsados.top() == 0){
                                text.append("\n ADD ");
                                text.append(to_string(auxAtribuirVetorVar.top()));
                                temporarioDisponivel.push(auxAtribuirVetorVar.top());
                                auxAtribuirVetorVar.pop();
                            }
                        }
                    }
                text.append("\n STO ");
                text.append(varInit.id);
                }

                if(compativel == 0){
                varInit.inicializado = true;
                    tabelaSimbolo.push_front(varInit);
                    break;
                }else if(compativel == 1){
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
                valorAtr.push(lexema);

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
                varInit =  tabelaSimbolo.front();
                tipoAtr = ConverteTipo(varInit.tipo);
                tabelaSimbolo.pop_front();
                varInit.valor = stoi(valorAtr.top());
                valorAtr.pop();
                atributosUsados.pop();

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
                  varInit.inicializado = true;
                  tabelaSimbolo.push_front(varInit);
                  break;
                }else if(compativel == 1){
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
                auxOperadoresUsados.push(tipoOperador);
                break;
        case 14:
                tipoOperador = 1;
                operadoresUsados.push(tipoOperador);
                auxOperadoresUsados.push(tipoOperador);
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
                     nomeFun = lexema;
                     text.append("\n \t");
                     text.append(lexema);
                     text.append(" :");
                     break;
            }
            nomeFun = lexema;
            text.append("\n \t");
            text.append(lexema);
            text.append(" :");
            break;

        case 21:

            simboloVar = tabelaSimbolo.front();
            simboloVar.vetor = true;
            simboloVar.posVetor = tamVetor;
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
                     textTemp = lexema;
                     VarExiste = false;
                     break;
            }
            throw SemanticError("Funcao nao exite", token->getPosition());
            break;

        case 24:
            for (Simbolo simboloFor : tabelaSimbolo){
                     auxDeleteTable = tabelaSimbolo.front();
                     tabelaSimboloAuxDelete.push_front(auxDeleteTable);
                     if(lexema == simboloFor.id && escopo.size() >= simboloFor.escopo && simboloFor.vetor ){
                    if(auxDeleteTable.id == simboloFor.id  && auxDeleteTable.tipo == simboloFor.tipo  && auxDeleteTable.escopo <= simboloFor.escopo){
                        varInit = simboloFor;
                        tipoAtr = ConverteTipo(simboloVar.tipo);
                        VarExiste = true;
                        if(simboloFor.posVetor == tamVetor){
                        tabelaSimboloAuxDelete.pop_front();
                        }
                    }
                     }
                     tabelaSimbolo.pop_front();
            }
            tabelaSimbolo.swap(tabelaSimboloAuxDelete);
            tabelaSimboloAuxDelete.clear();



            if(VarExiste){
                     auxTextVetor.push( lexema);
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
                    vetorOperando.push(lexema);
                    VarExiste = true;
                    tipoUsado = ConverteTipo(simboloVar.tipo);
                     }else if(lexema == simboloFor.id && escopo.size() >= simboloFor.escopo){
                    if(auxDeleteTable.id == simboloFor.id  && auxDeleteTable.tipo == simboloFor.tipo  && auxDeleteTable.escopo <= simboloFor.escopo){
                        simboloFor.usado = true;
                        VarExiste = true;
                        tabelaSimboloAuxDelete.pop_front();
                        tabelaSimboloAuxDelete.push_front(simboloFor);
                        tipoUsado = ConverteTipo(simboloVar.tipo);
                        vetorOperando.push(lexema);
                    }
                     }
                     tabelaSimbolo.pop_front();
            }
            auxOperacaoVetor1 = valorAtr.size();
            tabelaSimbolo.swap(tabelaSimboloAuxDelete);
            tabelaSimboloAuxDelete.clear();
            if(VarExiste){
                     VarExiste = false;
                     break;
            }
            throw SemanticError("Vetor nao exite", token->getPosition());
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
                    simboloVar.nomeFun = nomeFun;
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
            tamVetor = stoi(lexema);
            //definir tamanho do vetor
            //cout << posVetor;
            break;

        case 28:

            posVetor = stoi(lexema);
            diferencaValorAtrVetor = valorAtr.size() -auxOperacaoVetor1;

            cout<<"tamanho do auxOperadores 28 "<<auxOperadoresUsados.size()<<endl;
            for (int i = 0; i < diferencaValorAtrVetor; i++) {
                     valorAtrRev.push(valorAtr.top());
                     valorAtr.pop();
            }

            if(!vetorOperando.empty()){

                    if(isNumeric(valorAtrRev.top())){
                        text.append("\n LDI ");
                        text.append(valorAtrRev.top());
                        valorAtrRev.pop();
                    }else{
                        text.append("\n LD ");
                        text.append(valorAtrRev.top());
                        valorAtrRev.pop();
                    }

                     if(!valorAtrRev.empty()){
                        cout<<"chegou aqui valor atr !empty 28";

                     for (int i = 1; i < diferencaValorAtrVetor; i++) {
                    if(auxOperadoresUsados.top() == 1){
                    if(isNumeric(valorAtrRev.top())){
                        text.append("\n SUBI ");
                        text.append(valorAtrRev.top());
                        valorAtrRev.pop();
                    }else{
                        text.append("\n SUB ");
                        text.append(valorAtrRev.top());
                        valorAtrRev.pop();
                    }
                     }else if(auxOperadoresUsados.top() == 0){
                    if(isNumeric(valorAtrRev.top()) ){
                        text.append("\n ADDI ");
                        text.append(valorAtrRev.top());
                        valorAtrRev.pop();
                    }else{
                        text.append("\n ADD ");
                        text.append(valorAtrRev.top());
                        valorAtrRev.pop();
                    }
                     }
                     auxOperadoresUsados.pop();
                     }
                     }
                     text.append("\n STO $INDR");
                     text.append("\n LDV ");
                     text.append(vetorOperando.top());
                     vetorOperando.pop();

                     if(diferencaValorAtrVetor == 1 && temporarioUsado.empty()){}else{
                     temporarioUsado.push(temporarioDisponivel.top());
                     temporarioDisponivel.pop();
                     text.append("\n STO ");
                     text.append(to_string(temporarioUsado.top()));
                     break;

                     };

            }

            if(auxOperadoresUsados.empty() && !valorAtrRev.empty()){
                     if(isNumeric(valorAtrRev.top())){
                    text.append("\n LDI ");
                    text.append(valorAtrRev.top());
                     }else{
                    text.append("\n LD ");
                    text.append(valorAtrRev.top());
                     }
                     valorAtrRev.pop();
                     temporarioUsado.push(temporarioDisponivel.top());
                     temporarioDisponivel.pop();
                     text.append("\n STO ");
                     text.append(to_string(temporarioUsado.top()));

            };

            if(!valorAtrRev.empty()){

                    if(isNumeric(valorAtrRev.top())){
                        text.append("\n LDI ");
                        text.append(valorAtrRev.top());
                        valorAtrRev.pop();
                    }else{
                        text.append("\n LD ");
                        text.append(valorAtrRev.top());
                        valorAtrRev.pop();
                    }
                    while(!valorAtrRev.empty()){
                        if(auxOperadoresUsados.top() == 1){
                        if(isNumeric(valorAtrRev.top())){
                                text.append("\n SUBI ");
                                text.append(valorAtrRev.top());
                                valorAtrRev.pop();
                        }else{
                                text.append("\n SUB ");
                                text.append(valorAtrRev.top());
                                valorAtrRev.pop();
                        }
                        }else if(auxOperadoresUsados.top() == 0){
                        if(isNumeric(valorAtrRev.top()) ){
                                text.append("\n ADDI ");
                                text.append(valorAtrRev.top());
                                valorAtrRev.pop();
                        }else{
                                text.append("\n ADD ");
                                text.append(valorAtrRev.top());
                                valorAtrRev.pop();
                        }
                        auxOperadoresUsados.pop();

                        if(auxTextVetor.size() > 1){
                        text.append("\n STO $INDR");
                        text.append("\n LDV ");
                        text.append(auxTextVetor.top());
                        auxTextVetor.pop();
                        }


                    }
                    }
                    temporarioUsado.push(temporarioDisponivel.top());
                    temporarioDisponivel.pop();
                    text.append("\n STO ");
                    text.append(to_string(temporarioUsado.top()));
            }
            //text.append("\n STO $indr");
            //acessar o indice do vetor
            //cout << posVetor;
            break;


        case 29:
            while (!escopo.empty()) {
                escopo.pop();
            }


            /*for(Simbolo simboloFor : tabelaSimbolo){
                if(varInit.id != simboloFor.id && varInit.escopo != simboloFor.escopo){
                    contador++;
                }else{
                    break;
                }
            }
            if(contador == tabelaSimbolo.size()){
                tabelaSimbolo.push_front(varInit);
            }*/

            for (auto it = tabelaSimbolo.rbegin(); it != tabelaSimbolo.rend(); ++it){
                if(it->funcao == 1){continue;}
                data.append(it->id);
                if(it->posVetor != 0){
                    data.append(" : 0");
                    for(int i = 1; i< it->posVetor;i++){
                        data.append(",0");
                    }
                    data.append("\n");
                }else if(it->valor != 0){
                    data.append(": ");
                    data.append(to_string(it->valor));
                    data.append("\n");
                }else{
                    data.append(" : 0 \n");
                }

            }

            for (auto it = tabelaSimboloFuncoes.rbegin(); it != tabelaSimboloFuncoes.rend(); ++it){
                data.append(it->nomeFun);
                data.append("_");
                data.append(it->id);
                if(it->posVetor != 0){
                    data.append(" : 0");
                    for(int i = 1; i< it->posVetor;i++){
                        data.append(",0");
                    }
                    data.append("\n");
                }else if(it->valor != 0){
                    data.append(": ");
                    data.append(to_string(it->valor));
                    data.append("\n");
                }else{
                    data.append(" : 0 \n");
                }

            }

            cout<<".DATA \n";
            cout<<data;
            cout<<".TEXT \n";
            cout<<text;

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

        case 32:
            //cin
            if(lexema == "]"){
                text.append("\n LD $in_port \n STOV ");
                text.append(auxTextVetor.top());
                tabelaSimbolo.push_front(varInit);
                auxTextVetor.pop();
                break;
            }
            text.append("\n LD $in_port \n STO");
            text.append(lexema);
            break;
        case 33:
            text.append("\n STO $out_port");
            //cout
            break;
        case 34:
            if(lexema == "]"){
                text.append("\n LDV ");
                text.append(auxTextVetor.top());
                tabelaSimbolo.push_front(varInit);
                auxTextVetor.pop();
                break;
            }
            text.append("\n LD ");
            text.append(lexema);
            //cout id
            break;
        case 35:
            text.append("\n LDI ");
            text.append(lexema);
            //cout numero
            break;

        case 22:
            oprel = lexema;
            tipoOperador = 5;
            operadoresUsados.push(tipoOperador);
            auxOperadoresUsados.push(tipoOperador);

            while(!valorAtr.empty()){
                valorAtrRev.push(valorAtr.top());
                valorAtr.pop();
            }

            if(isNumeric(valorAtrRev.top())){
                text.append("\n LDI ");
                text.append(valorAtrRev.top());
                valorAtrRev.pop();
            }else{
                text.append("\n LD ");
                text.append(valorAtrRev.top());
                valorAtrRev.pop();
            }

            text.append("\n STO ");
            text.append(to_string(temporarioDisponivel.top()));
            temporarioUsado.push(temporarioDisponivel.top());
            temporarioDisponivel.pop();

            break;



        case 36:

            while(!valorAtr.empty()){
                valorAtrRev.push(valorAtr.top());
                valorAtr.pop();
            }

            if(isNumeric(valorAtrRev.top())){
                text.append("\n LDI ");
                text.append(valorAtrRev.top());
                valorAtrRev.pop();
            }else{
                text.append("\n LD ");
                text.append(valorAtrRev.top());
                valorAtrRev.pop();
            }
            text.append("\n STO ");
            text.append(to_string(temporarioDisponivel.top()));
            temporarioUsado.push(temporarioDisponivel.top());
            temporarioDisponivel.pop();


            while(!temporarioUsado.empty()){
                auxTemporario.push(temporarioUsado.top());
                temporarioUsado.pop();
            }

            text.append("\n LD ");
            text.append(to_string(auxTemporario.top()));
            temporarioDisponivel.push(auxTemporario.top());
            auxTemporario.pop();

            text.append("\n SUB ");
            text.append(to_string(auxTemporario.top()));
            temporarioDisponivel.push(auxTemporario.top());
            auxTemporario.pop();

            break;

        case 37:
            controleRotulos++;
            rotulos.push_front(controleRotulos);
            oprel = descobreBranchIf(oprel);
            text.append(oprel);
            text.append(" R");
            text.append(to_string(controleRotulos));




            break;

        case 38:
            text.append("\n \t R");
            text.append(to_string(controleRotulos));
            text.append(" :");
            break;

        case 39:
            controleRotulos++;
            rotulos.push_front(controleRotulos);
            text.append("\n JMP");
            text.append(" R");
            text.append(to_string(controleRotulos));
            controleRotulos--;
            rotulos.pop_front();
            text.append("\n \t R");
            text.append(to_string(controleRotulos));
            text.append(" :");
            controleRotulos++;
            rotulos.push_front(controleRotulos);

            break;

        case 41:
            controleRotulos++;
            rotulos.push_front(controleRotulos);
            text.append("\n \t R");
            text.append(to_string(controleRotulos));
            text.append(" :");
            break;

        case 42:
            controleRotulos++;
            rotulos.push_front(controleRotulos);
            oprel = descobreBranchIf(oprel);
            text.append(oprel);
            text.append(" R");
            text.append(to_string(controleRotulos));
            break;

        case 43:
            controleRotulos--;
            rotulos.pop_front();
            text.append("\n JMP");
            text.append(" R");
            text.append(to_string(controleRotulos));
            controleRotulos++;
            rotulos.push_front(controleRotulos);
            text.append("\n \t R");
            text.append(to_string(controleRotulos));
            break;

        case 44:
            controleRotulos++;
            rotulos.push_front(controleRotulos);
            text.append("\n \t R");
            text.append(to_string(controleRotulos));
            text.append(" :");
            break;

        case 45:
            oprel = descobreBranchWhile(oprel);
            text.append(oprel);
            text.append(" R");
            text.append(to_string(controleRotulos));
            break;

        case 46:
            controleRotulos++;
            rotulos.push_front(controleRotulos);
            text.append("\n \t R");
            text.append(to_string(controleRotulos));
            text.append(" :");
            break;
        case 47:

            controleRotulos++;
            rotulos.push_front(controleRotulos);
            oprel = descobreBranchWhile(oprel);
            text.append(oprel);
            text.append(" R");
            text.append(to_string(controleRotulos));
            break;

        case 48:

            while(!valorAtr.empty()){
                valorAtrRev.push(valorAtr.top());
                valorAtr.pop();
            }

            if(!valorAtrRev.empty()){

                if(isNumeric(valorAtrRev.top())){
                    textTemp.append("\n LDI ");
                    textTemp.append(valorAtrRev.top());
                    valorAtrRev.pop();
                }else{
                    textTemp.append("\n LD ");
                    textTemp.append(valorAtrRev.top());
                    valorAtrRev.pop();
                }
                if(!temporarioUsado.empty() && !auxOperadoresUsados.empty() && valorAtrRev.empty()){
                    textTemp.append("\n STO ");
                    textTemp.append(to_string(temporarioDisponivel.top()));
                    cout<< "\n tamanho temporario disponiveis"<< temporarioDisponivel.size();
                    temporarioUsado.push(temporarioDisponivel.top());

                    temporarioDisponivel.pop();

                }
                while(!valorAtrRev.empty()){
                    if(auxOperadoresUsados.top() == 1){
                        if(isNumeric(valorAtrRev.top())){
                        textTemp.append("\n SUBI ");
                        textTemp.append(valorAtrRev.top());
                        valorAtrRev.pop();
                        }else{
                        textTemp.append("\n SUB ");
                        textTemp.append(valorAtrRev.top());
                        valorAtrRev.pop();
                        }
                    }else if(auxOperadoresUsados.top() == 0){
                        if(isNumeric(valorAtrRev.top()) ){
                        textTemp.append("\n ADDI ");
                        textTemp.append(valorAtrRev.top());
                        valorAtrRev.pop();
                        }else{
                        textTemp.append("\n ADD ");
                        textTemp.append(valorAtrRev.top());
                        valorAtrRev.pop();
                        }
                    }
                    auxOperadoresUsados.pop();
                }

            }
            break;

        case 50:
            text.append(textTemp);
            textTemp.clear();
            controleRotulos--;
            rotulos.pop_front();
            text.append("\n JMP R");
            text.append(to_string(controleRotulos));

            controleRotulos++;
            rotulos.push_front(controleRotulos);
            text.append("\n \t R");
            text.append(to_string(controleRotulos));
            text.append(" :");
            break;

        case 51:
            text.append("\n \t Main :");
            break;

        case 52:
            text.append("\n CALL ");
            text.append(textTemp);
            break;
        case 53:
            //gera retorno funcao
            break;
        case 54:
            //lista termo
            while(!valorAtr.empty()){
                valorAtrRev.push(valorAtr.top());
                valorAtr.pop();
            }
            cout<<"\n 1253 line \n";

            while(!valorAtrRev.empty()){

                if(isNumeric(valorAtrRev.top())){
                    text.append("\n LDI ");
                    text.append(valorAtrRev.top());
                    valorAtrRev.pop();
                }else{
                    text.append("\n LD ");
                    text.append(valorAtrRev.top());
                    valorAtrRev.pop();
                }
            }
            break;

    }

}

