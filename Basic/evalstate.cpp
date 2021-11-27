/*
 * File: evalstate.cpp
 * -------------------
 * This file implements the EvalState class, which defines a symbol
 * table for keeping track of the value of identifiers.  The public
 * methods are simple enough that they need no individual documentation.
 */

#include <string>
#include "evalstate.h"

#include "../StanfordCPPLib/map.h"

#include <map>

using namespace std;

/* Implementation of the EvalState class */

EvalState::EvalState() {
    /* Empty */
    
    symbol_table.clear();
}

EvalState::~EvalState() {
    /* Empty */
    
    symbol_table.clear();
}

void EvalState::setValue(string var, int value) {
    //symbolTable.put(var, value);
    
    if(symbol_table.count(var)){
        iter_symbol_table=symbol_table.find(var);
        iter_symbol_table->second=value;
    }
    else symbol_table.insert(pair<std::string,int>(var,value));
}

int EvalState::getValue(string var) {
    //return symbolTable.get(var);
    
    return symbol_table.find(var)->second;
}

bool EvalState::isDefined(string var) {
    //return symbolTable.containsKey(var);
    if(symbol_table.count(var)) return true;
    else return false;
}



