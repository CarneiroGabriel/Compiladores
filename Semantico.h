#ifndef SEMANTICO_H
#define SEMANTICO_H

#include <iostream>
#include "Token.h"
#include "SemanticError.h"

using namespace std;

class Semantico
{
public:
    void extracted(string &lexema);
    void executeAction(int action, const Token *token) throw (SemanticError );
};

#endif
