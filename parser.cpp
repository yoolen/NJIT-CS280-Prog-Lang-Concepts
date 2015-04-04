#include "p2lex.h"
#include <iostream>
#include <fstream>
using namespace std;

enum NODETYPES {
	NODE,	// Node with children
	LEAF	// ID, INT, STRING, NULL
};

class PTree {
	Token type;
	string value;
	PTree *left;
	PTree *right;

public:
	PTree(PTree *left, PTree *right = 0) {
		this->left = left;
		this->right = right;
	}

	string getValue(){
		return this->value;
	}

	Token getType(){
		return this->type;
	}
};

class PTreeStmtList : public PTree {
// Statement list composed of statement followed by statement or list of statements
public:
	PTreeStmtList(PTree *s1, PTree *s2 = 0) : PTree(s1, s2) {}
};

class PTreeStmt : public PTree {
// Statement composed of keyword SET or PRINT
private:
	string keyword;
public:
	PTreeStmt(string keyword, PTree *s1, PTree *s2 = 0) : PTree(0, s2) {
		this->keyword = keyword;
		this->left = *keyword;
	}
};

extern PTree *Program(istream *br);
extern PTree *StmtList(istream *br);
extern PTree *Stmt(istream *br);
extern PTree *Expr(istream *br);
extern PTree *Term(istream *br);
extern PTree *Primary(istream *br);

// Program ::= StmtList
PTree *Program(istream *br)
{
	return StmtList(br);
}

// StmtList ::= Stmt | Stmt StmtList
PTree *StmtList(istream *br)
{
	PTree *stmt;

	stmt = Stmt(br);

	if( stmt )
		return new PTreeStmtList( stmt, StmtList(br) );
	return 0;
}

PTree *Stmt(istream *br)
{
	PTree *expr;
	string lexeme;
	Token token = getToken(br, lexeme);
	switch(token){
		case PRINT:
			cout << "PRINT found!" << endl;
			return new PTree(lexeme, Expr(br));
			//expr = Expr(br);
			break;
		case SET:
			cout << "SET found!" << endl;
			token = getToken(br, lexeme);
			if(token != ID){
				cout << "Error, SET not followed by ID." << endl;
				return 0;
			}
			return new PTree(lexeme, 0);
			break;
		default:
			cout << "ERROR!" << endl;
			break;
	}

	if( expr )

	return 0;
}

PTree *Expr(istream *br)
{
	return 0;
}

PTree *Term(istream *br)
{
	return 0;
}

PTree *Primary(istream *br)
{
	return 0;
}


int main(int argc, char *argv[])
{
	istream *br;
	ifstream infile;
	PTree *petri;

	if(argc == 1)
		br = &cin;
	else if(argc != 2){
		cout << "Incorrect parameters!";
		return 0;
	}
	else{
		infile.open(argv[1]);
		if(infile.is_open())
			br = &infile;
		else{
			cout << argv[1] << " can't be opened!" << endl;
			return 0;
		}
	}
	petri = Program(br);
	return 0;
}
