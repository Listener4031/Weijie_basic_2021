/*
 * File: Basic.cpp
 * ---------------
 * This file is the starter project for the BASIC interpreter.
 */

#include <cctype>
#include <iostream>
#include <string>
#include <cstring>
#include "exp.h"
#include "parser.h"
#include "program.h"
#include "../StanfordCPPLib/error.h"
#include "../StanfordCPPLib/tokenscanner.h"

#include "../StanfordCPPLib/simpio.h"
#include "../StanfordCPPLib/strlib.h"

using namespace std;

/* Function prototypes */

void processLine(string line, Program &program, EvalState &state);

/* Main program */

int main() {
    EvalState state;
    Program program;
    //cout << "Stub implementation of BASIC" << endl;
    while (true) {
        try {
            string input = getLine();
            if (input.empty())
                return 0;
            processLine(input, program, state);
        } catch (ErrorException &ex) {
            cout << ex.getMessage() << endl;
        }
    }
    return 0;
}

/*
 * Function: processLine
 * Usage: processLine(line, program, state);
 * -----------------------------------------
 * Processes a single line entered by the user.  In this version of
 * implementation, the program reads a line, parses it as an expression,
 * and then prints the result.  In your implementation, you will
 * need to replace this method with one that can respond correctly
 * when the user enters a program line (which begins with a number)
 * or one of the BASIC commands, such as LIST or RUN.
 */

void processLine(string line, Program &program, EvalState &state){
    TokenScanner scanner;
    scanner.ignoreWhitespace();
    scanner.scanNumbers();
    scanner.setInput(line);
    string get_token;
    string get_token_run;
    get_token=scanner.nextToken();
    if(get_token[0]>='0'&&get_token[0]<='9'){
        int get_int=0;
        for(int i=0;i<get_token.size();i++){
            get_int*=10;
            get_int+=int(get_token[i]-'0');
        }
        if(!scanner.hasMoreTokens()) program.removeSourceLine(get_int);
        else{
            string real_line;
            int p=get_token.size();
            while(line[p]==' ') p++;
            real_line=line.substr(p,line.size()-p);
            program.addSourceLine(get_int,real_line);
        }
    }
    else{
        if(get_token=="QUIT"){
            exit(0);
        }
        if(get_token=="LET"){
            int p=get_token.size();
            while(line[p]==' ') p++;
            if(line[p]=='L'&&line[p+1]=='E'&&line[p+2]=='T'){
                cout << "SYNTAX ERROR" << endl;
                exit(0);
            }
            string get_token_extra=scanner.nextToken();
            p+=get_token_extra.size();
            while(line[p]==' '||line[p]=='=') p++;
            scanner.setInput(line.substr(p,line.size()-p));
            Expression *exp4 = parseExp(scanner);
            int value = exp4->eval(state);
            state.setValue(get_token_extra,value);
            delete exp4;
        }
        if(get_token=="LIST"){
            for(program.iter_number_to_sourceline=program.number_to_sourceline.begin();program.iter_number_to_sourceline!=program.number_to_sourceline.end();program.iter_number_to_sourceline++){
                cout << program.iter_number_to_sourceline->first << " " << program.iter_number_to_sourceline->second << endl;
            }
        }
        if(get_token=="PRINT"){
            int p=get_token.size();
            while(line[p]==' ') p++;
            scanner.setInput(line.substr(p,line.size()-p));
            Expression *exp = parseExp(scanner);
            int value = exp->eval(state);
            cout << value << endl;
            delete exp;
        }
        if(get_token=="CLEAR"){
            program.clear();
            state.symbol_table.clear();
        }
        if(get_token=="INPUT"){
            get_token=scanner.nextToken();
            bool ck_legal=true;
            while(ck_legal){
                cout << " ? ";
                string a;
                getline(cin,a);
                //TokenScanner scanner2;
                //scanner2.ignoreWhitespace();
                //scanner2.scanNumbers();
                bool ck_a=true;
                for(int i=1;i<a.size();i++){
                    if(!(a[i]>='0'&&a[i]<='9')) ck_a=false;
                }
                if(ck_a){
                    if(a[0]>='0'&&a[0]<='9'){
                        //scanner2.setInput(a);
                        ck_legal=false;
                        //Expression *exp = parseExp(scanner2);
                        //int value_input = exp->eval(state);
                        int get_int=0;
                        for(int i=0;i<a.size();i++){
                            get_int*=10;
                            get_int+=int(a[i]-'0');
                        }
                        state.setValue(get_token,get_int);
                        //delete exp;
                    }
                    else if(a[0]=='-'){
                        //scanner2.setInput(a.substr(1,a.size()-1));
                        ck_legal=false;
                        //Expression *exp = parseExp(scanner2);
                        //int value_input = exp->eval(state);
                        int get_int=0;
                        for(int i=1;i<a.size();i++){
                            get_int*=10;
                            get_int+=int(a[i]-'0');
                        }
                        state.setValue(get_token,-get_int);
                        //delete exp;
                    }
                    else{
                        cout << "INVALID NUMBER" << endl;
                        continue;
                    }
                }
                else{
                    cout << "INVALID NUMBER" << endl;
                    continue;
                }
            }
        }
        if(get_token=="RUN"){
            TokenScanner scanner_run;
            scanner_run.ignoreWhitespace();
            scanner_run.scanNumbers();
            program.iter_number_to_sourceline=program.number_to_sourceline.begin();
            while(program.iter_number_to_sourceline!=program.number_to_sourceline.end()){
                scanner_run.setInput(program.iter_number_to_sourceline->second);
                get_token_run=scanner_run.nextToken();
                if(get_token_run=="REM"){
                    program.iter_number_to_sourceline++;
                    continue;
                }
                if(get_token_run=="END") break;
                if(get_token_run=="LET"){
                    int p=get_token_run.size();
                    while(program.iter_number_to_sourceline->second[p]==' ') p++;
                    if(program.iter_number_to_sourceline->second[p]=='L'&&program.iter_number_to_sourceline->second[p+1]=='E'&&program.iter_number_to_sourceline->second[p+2]=='T'){
                        cout << "SYNTAX ERROR" << endl;
                        exit(0);
                    }
                    get_token_run=scanner_run.nextToken();
                    p+=get_token_run.size();
                    while(program.iter_number_to_sourceline->second[p]==' '||program.iter_number_to_sourceline->second[p]=='=') p++;
                    scanner_run.setInput(program.iter_number_to_sourceline->second.substr(p,program.iter_number_to_sourceline->second.size()-p));
                    Expression *exp = parseExp(scanner_run);
                    int value = exp->eval(state);
                    state.setValue(get_token_run,value);
                    delete exp;
                    program.iter_number_to_sourceline++;
                    continue;
                }
                if(get_token_run=="IF"){
                    int p=get_token_run.size();
                    while(program.iter_number_to_sourceline->second[p]==' ') p++;
                    const int p1=p;
                    while(program.iter_number_to_sourceline->second[p]!='>'&&program.iter_number_to_sourceline->second[p]!='='&&program.iter_number_to_sourceline->second[p]!='<') p++;
                    const int p2=p;
                    TokenScanner scanner_run_if;
                    scanner_run_if.ignoreWhitespace();
                    scanner_run_if.scanNumbers();
                    scanner_run_if.setInput(program.iter_number_to_sourceline->second.substr(p1,p2-p1));
                    Expression *exp1 = parseExp(scanner_run_if);
                    const int value_left = exp1->eval(state);
                    delete exp1;
                    p++;
                    while(program.iter_number_to_sourceline->second[p]==' ') p++;
                    const int p3=p;
                    while(!(program.iter_number_to_sourceline->second[p]=='T'&&program.iter_number_to_sourceline->second[p+1]=='H'&&program.iter_number_to_sourceline->second[p+2]=='E'&&program.iter_number_to_sourceline->second[p+3]=='N')) p++;
                    const int p4=p;
                    scanner_run_if.setInput(program.iter_number_to_sourceline->second.substr(p3,p4-p3));
                    Expression *exp2 = parseExp(scanner_run_if);
                    const int value_right = exp2->eval(state);
                    delete exp2;
                    bool ck=false;
                    if(program.iter_number_to_sourceline->second[p2]=='<'&&value_left<value_right) ck=true;
                    if(program.iter_number_to_sourceline->second[p2]=='='&&value_left==value_right) ck=true;
                    if(program.iter_number_to_sourceline->second[p2]=='>'&&value_left>value_right) ck=true;
                    if(ck){
                        p+=4;
                        while(program.iter_number_to_sourceline->second[p]==' ') p++;
                        int p5=p;
                        scanner_run_if.setInput(program.iter_number_to_sourceline->second.substr(p5,program.iter_number_to_sourceline->second.size()-p5));
                        Expression *exp3 = parseExp(scanner_run_if);
                        const int value_goto = exp3->eval(state);
                        delete exp3;
                        program.iter_number_to_sourceline=program.number_to_sourceline.find(value_goto);
                        if(program.iter_number_to_sourceline==program.number_to_sourceline.end()){
                            cout << "LINE NUMBER ERROR" << endl;

                        }
                        //LINE NUMBER ERROR
                        continue;
                    }
                    else{
                        program.iter_number_to_sourceline++;
                        continue;
                    }
                    
                }
                if(get_token_run=="PRINT"){
                    int p=get_token_run.size();
                    while(program.iter_number_to_sourceline->second[p]==' ') p++;
                    scanner_run.setInput(program.iter_number_to_sourceline->second.substr(p,program.iter_number_to_sourceline->second.size()-p));
                    Expression *exp = parseExp(scanner_run);
                    int value = exp->eval(state);
                    cout << value << endl;
                    delete exp;
                    program.iter_number_to_sourceline++;
                    continue;
                }
                if(get_token_run=="GOTO"){
                    int p=get_token_run.size();
                    while(program.iter_number_to_sourceline->second[p]==' ') p++;
                    scanner_run.setInput(program.iter_number_to_sourceline->second.substr(p,program.iter_number_to_sourceline->second.size()-p));
                    Expression *exp = parseExp(scanner_run);
                    int value = exp->eval(state);
                    program.iter_number_to_sourceline=program.number_to_sourceline.find(value);
                    if(program.iter_number_to_sourceline==program.number_to_sourceline.end()){
                        cout << "LINE NUMBER ERROR" << endl;

                    }
                    delete exp;
                    continue;
                }
                if(get_token_run=="INPUT"){
                    get_token_run=scanner_run.nextToken();
                    bool ck_legal=true;
                    while(ck_legal){
                        cout << " ? ";
                        string a;
                        getline(cin,a);
                        //TokenScanner scanner2;
                        //scanner2.ignoreWhitespace();
                        //scanner2.scanNumbers();
                        bool ck_a=true;
                        for(int i=1;i<a.size();i++){
                            if(!(a[i]>='0'&&a[i]<='9')) ck_a=false;
                        }
                        if(ck_a){
                            if(a[0]>='0'&&a[0]<='9'){
                                //scanner2.setInput(a);
                                ck_legal=false;
                                //Expression *exp = parseExp(scanner2);
                                //int value_input = exp->eval(state);
                                int get_int=0;
                                for(int i=0;i<a.size();i++){
                                    get_int*=10;
                                    get_int+=int(a[i]-'0');
                                }
                                state.setValue(get_token_run,get_int);
                                //delete exp;
                            }
                            else if(a[0]=='-'){
                                //scanner2.setInput(a.substr(1,a.size()-1));
                                ck_legal=false;
                                //Expression *exp = parseExp(scanner2);
                                //int value_input = exp->eval(state);
                                int get_int=0;
                                for(int i=1;i<a.size();i++){
                                    get_int*=10;
                                    get_int+=int(a[i]-'0');
                                }
                                state.setValue(get_token_run,-get_int);
                                //delete exp;
                            }
                            else{
                                cout << "INVALID NUMBER" << endl;
                                continue;
                            }
                        }
                        else{
                            cout << "INVALID NUMBER" << endl;
                            continue;
                        }
                    }
                    program.iter_number_to_sourceline++;
                    continue;
                }
            }
            
        }
    }
    //Expression *exp = parseExp(scanner);
    //int value = exp->eval(state);
    //cout << value << endl;
    //delete exp;
}

/*
 int p=get_token_run.size();
 while(program.iter_number_to_sourceline->second[p]==' ') p++;
 const int p1=p;
 while(program.iter_number_to_sourceline->second[p]!='='&&program.iter_number_to_sourceline->second[p]!='>'&&program.iter_number_to_sourceline->second[p]!='<') p++;
 const int p2=p;
 scanner_run.setInput(program.iter_number_to_sourceline->second.substr(p1,p-p1));
 Expression *exp1 = parseExp(scanner_run);
 const int value_left = exp1->eval(state);
 delete exp1;
 p++;
 while(program.iter_number_to_sourceline->second[p]==' ') p++;
 const int p3=p;
 scanner_run.setInput(program.iter_number_to_sourceline->second.substr(p,program.iter_number_to_sourceline->second.size()-p));
 Expression *exp2 = parseExp(scanner_run);
 const int value_right = exp2->eval(state);
 delete exp2;
 bool ck=false;
 if(program.iter_number_to_sourceline->second[p2]=='<'&&value_left<value_right) ck=true;
 if(program.iter_number_to_sourceline->second[p2]=='='&&value_left==value_right) ck=true;
 if(program.iter_number_to_sourceline->second[p2]=='>'&&value_left>value_right) ck=true;
 if(ck){
     get_token_run=scanner_run.nextToken();
     cout << get_token << endl;
     get_token_run=scanner_run.nextToken();
     cout << get_token << endl;
     get_token_run=scanner_run.nextToken();
     cout << get_token << endl;
     continue;
 }
 else{
     program.iter_number_to_sourceline++;
     continue;
 }
 
 */
/*
 const int p1=scanner_run.getPosition();
 while(scanner_run.nextToken()!="<"&&scanner_run.nextToken()!=">"&&scanner_run.nextToken()!="="){
     get_token_run=scanner_run.nextToken();
 }
 const int p2=scanner_run.getPosition();
 TokenScanner scanner_run_if;
 scanner_run_if.ignoreWhitespace();
 scanner_run_if.scanNumbers();
 scanner_run_if.setInput(program.iter_number_to_sourceline->second.substr(p1,p2-p1));
 Expression *exp1 = parseExp(scanner_run_if);
 const int value_left = exp1->eval(state);
 delete exp1;
 get_token_run=scanner_run.nextToken();
 const int p3=scanner_run.getPosition();
 while(scanner_run.nextToken()!="THEN") get_token_run=scanner_run.nextToken();
 const int p4=scanner_run.getPosition();
 scanner_run_if.setInput(program.iter_number_to_sourceline->second.substr(p3,p4-p3));
 Expression *exp2 = parseExp(scanner_run_if);
 const int value_right = exp2->eval(state);
 delete exp2;
 bool ck=false;
 if(program.iter_number_to_sourceline->second[p2]=='<'&&value_left<value_right) ck=true;
 if(program.iter_number_to_sourceline->second[p2]=='='&&value_left==value_right) ck=true;
 if(program.iter_number_to_sourceline->second[p2]=='>'&&value_left>value_right) ck=true;
 if(ck){
     get_token_run=scanner_run.nextToken();
     cout << get_token << endl;
     get_token_run=scanner_run.nextToken();
     cout << get_token << endl;
     get_token_run=scanner_run.nextToken();
     cout << get_token << endl;
     continue;
 }
 else{
     program.iter_number_to_sourceline++;
     continue;
 }
 */
