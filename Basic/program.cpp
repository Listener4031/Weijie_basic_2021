/*
 * File: program.cpp
 * -----------------
 * This file is a stub implementation of the program.h interface
 * in which none of the methods do anything beyond returning a
 * value of the correct type.  Your job is to fill in the bodies
 * of each of these methods with an implementation that satisfies
 * the performance guarantees specified in the assignment.
 */

#include <string>
#include "program.h"
#include "statement.h"

#include <map>

using namespace std;

Program::Program(){
    // Replace this stub with your own code
    clear();
}

Program::~Program() {
    // Replace this stub with your own code
    clear();
}

void Program::clear() {
    // Replace this stub with your own code
    number_to_sourceline.clear();
    for(iter_number_to_statement_ptr=number_to_statement_ptr.begin();iter_number_to_statement_ptr!=number_to_statement_ptr.end();iter_number_to_statement_ptr++){
        if(iter_number_to_statement_ptr->second!=NULL){
            delete iter_number_to_statement_ptr->second;
            iter_number_to_statement_ptr->second=NULL;
        }
    }
    number_to_statement_ptr.clear();
    size_of_program=0;
}

void Program::addSourceLine(int lineNumber, string line) {
    // Replace this stub with your own code
    if(number_to_sourceline.count(lineNumber)){
        iter_number_to_sourceline=number_to_sourceline.find(lineNumber);
        iter_number_to_statement_ptr=number_to_statement_ptr.find(lineNumber);
        iter_number_to_sourceline->second=line;
        if(iter_number_to_statement_ptr->second!=NULL){
            delete iter_number_to_statement_ptr->second;
            iter_number_to_statement_ptr->second=NULL;
        }
    }
    else{
        number_to_sourceline.insert(pair<int,std::string>(lineNumber,line));
        number_to_statement_ptr.insert(pair<int,Statement*>(lineNumber,NULL));
        size_of_program++;
    }
}

void Program::removeSourceLine(int lineNumber) {
    // Replace this stub with your own code
    if(!number_to_sourceline.count(lineNumber)) return;
    else{
        iter_number_to_statement_ptr=number_to_statement_ptr.find(lineNumber);
        if(iter_number_to_statement_ptr->second!=NULL){
            delete iter_number_to_statement_ptr->second;
            iter_number_to_statement_ptr->second=NULL;
        }
        number_to_statement_ptr.erase(lineNumber);
        iter_number_to_sourceline=number_to_sourceline.find(lineNumber);
        number_to_sourceline.erase(lineNumber);
        size_of_program--;
    }
}

string Program::getSourceLine(int lineNumber) {
    // Replace this stub with your own code
    if(!number_to_sourceline.count(lineNumber)) return "";
    else{
        iter_number_to_sourceline=number_to_sourceline.find(lineNumber);
        return iter_number_to_sourceline->second;
    }
}

void Program::setParsedStatement(int lineNumber, Statement *stmt) {
    // Replace this stub with your own code
    iter_number_to_statement_ptr=number_to_statement_ptr.find(lineNumber);
    if(iter_number_to_statement_ptr->second!=NULL){
        delete iter_number_to_statement_ptr->second;
        //iter_number_to_statement_ptr->second=new Statement (*stmt);
    }
    //else iter_number_to_statement_ptr->second=new Statement (*stmt);
}

Statement *Program::getParsedStatement(int lineNumber) {
    // Replace this stub with your own code
    iter_number_to_statement_ptr=number_to_statement_ptr.find(lineNumber);
    return iter_number_to_statement_ptr->second;
}

int Program::getFirstLineNumber() {
    // Replace this stub with your own code
    if(size_of_program==0) return -1;
    else{
        iter_number_to_sourceline=number_to_sourceline.begin();
        return iter_number_to_sourceline->first;
    }
}

int Program::getNextLineNumber(int lineNumber) {
    // Replace this stub with your own code
    iter_number_to_sourceline=number_to_sourceline.find(lineNumber);
    iter_number_to_sourceline++;
    if(iter_number_to_sourceline==number_to_sourceline.end()) return -1;
    else return iter_number_to_sourceline->first;
}
