//
//  gettoken.cpp
//  p2lexer
//
//  Created by Gerard Ryan on 2/26/15.
//  Copyright (c) 2015 Gerard Ryan. All rights reserved.
//

#include "p2lex.h"
#include <cctype>
#include <cstdlib>

enum LexState {
    START,
    INID,
    ININT,
    INSTRING,
    INCOMMENT,
    ONESLASH
};

inline Token
keywordCheck(const string& word)
{
    if (word == "set") {
        return SET;
    }
    else if (word == "print") {
        return PRINT;
    }
    return ID;
}

int	currLine = 1;

static bool pushedBack = false;
static Token pushedToken;
static string pushedLexeme;

bool
pushbackToken(Token t, string lexeme)
{
	if( pushedBack ) return false; // cannot push more than 1

	pushedToken = t;
	pushedLexeme = lexeme;
	pushedBack = true;

	return true;
}

Token
getToken(istream *br, string& lexeme)
{
	if( pushedBack ) {
		pushedBack = false;
		lexeme = pushedLexeme;
		return pushedToken;
	}

    LexState state = START;
    lexeme.clear();
    
    int inchar;
    
    while ( (inchar = br->get()) ) {
    	if( inchar == '\n' ) currLine++;
        switch (state) {
            case START:
                if (isspace(inchar)) {
                    continue;
                }
                
                lexeme += inchar;
                
                if (inchar == '+') {
                    return PLUS;
                }
                if (inchar == '-') {
                    return MINUS;
                }
                if (inchar == '*') {
                    return STAR;
                }
                if (inchar == ';') {
                    return SC;
                }
                
                if (inchar == '/') {
                    state = ONESLASH;
                }
                else if (inchar == '"') {
                    state = INSTRING;
                }
                else if (isdigit(inchar)) {
                    state = ININT;
                }
                else if (isalpha(inchar)) {
                    state = INID;
                }
                else
                    return DONE;
                
                break;
                
            case INID:
                if (isalpha(inchar)) {
                    lexeme += inchar;
                    continue;
                }
                
                br->putback(inchar);
                return keywordCheck(lexeme);
                break;
                
            case ININT:
                if (isdigit(inchar)) {
                    lexeme += inchar;
                    continue;
                }
                
                br->putback(inchar);
                return INT;
                break;
                
            case INSTRING:
                lexeme += inchar;
                if (inchar == '\n') {
                    return DONE;
                }
                if (inchar != '"') {
                    continue;
                }
                
                return STRING;
                break;
                
            case INCOMMENT:
                if (inchar != '\n') {
                    continue;
                }
                state = START;
                break;
                
            case ONESLASH:
                if (inchar != '/') {
                    br->putback(inchar);
                    return SLASH;
                }
                lexeme.clear();
                state = INCOMMENT;
                break;
                
            default:
                cerr << "This should never happen!" << endl;
                exit(1);
                break;
        }
    }
    
    // I reached an end of file... perhaps it happened in the middle of something...
    if (state == INID) {
        return keywordCheck(lexeme);
    }
    else if (state == ININT) {
        return INT;
    }
    else if (state == ONESLASH) {
        return SLASH;
    }
    else {
        return DONE;
    }
}
