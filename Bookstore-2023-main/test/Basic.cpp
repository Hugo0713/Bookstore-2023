/*
 * File: Basic.cpp
 * ---------------
 * This file is the starter project for the BASIC interpreter.
 */

#include <cctype>
#include <iostream>
#include <string>
#include "exp.hpp"
#include "parser.hpp"
#include "program.hpp"
#include "Utils/error.hpp"
#include "Utils/tokenScanner.hpp"
#include "Utils/strlib.hpp"


/* Function prototypes */
Expression *exp0 = nullptr;
void processLine(std::string line, Program &program, EvalState &state);

/* Main program */

int main() {
    EvalState state;
    Program program;
    //cout << "Stub implementation of BASIC" << endl;
    while (true) {
        try {
            std::string input;
            getline(std::cin, input);
            if (input.empty())
                return 0;
            processLine(input, program, state);
        } catch (ErrorException &ex) {
            std::cout << ex.getMessage() << std::endl;
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

void processLine(std::string line, Program &program, EvalState &state) {
    TokenScanner scanner;
    scanner.ignoreWhitespace();
    scanner.scanNumbers();
    scanner.setInput(line);
    
    std::string token1 = scanner.nextToken();
    if(scanner.getTokenType(token1) == NUMBER)
    {
        program.addSourceLine(stoi(token1), line);
        return;
    }

    if(token1 == "QUIT")
    {
        program.clear();
        //state.Clear();
        exit(0);
    }   
    if(token1 == "LIST")
    {
        program.list();
    }
    if(token1 == "LET")
    {
        LetStmt temp("pre " + line);
        temp.execute(state, program);       
    }
    if(token1 == "CLEAR")
    {
        program.clear();
        state.Clear();
    }
    if(token1 == "RUN")
    {
        program.run(state);
    }
    if(token1 == "PRINT")
    {
        //Expression *exp0 = nullptr;
        exp0 = readE(scanner);
        std::cout << exp0->eval(state) << "\n";
        delete exp0;
    }
    if(token1 == "INPUT")
    {
        InputStmt temp("pre " + line);
        temp.execute(state, program);
    }
    if(token1 == "HELP")
    {
        std::cout << "Yet another basic interpreter" << "\n";
    }
}

