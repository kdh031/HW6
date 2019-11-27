//Expression.h
#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <vector>
#include <iostream>
#include "Token.h"
using namespace std;

struct SubExpression 
{
	vector<Token> tokenized;
	vector<Token> postfix;
	bool open = true;
	bool valid = NULL;
	string identity = "";
	void display() {
		cout << "display: ";
		for (int i=0; i< (int)tokenized.size(); i++) {
			cout << tokenized[i].get_token();
		}
		cout << endl;
	}
};

//Declare enum
enum Exp_type {assignment, arithmetic, illegal};

class Expression
{
public:
	//Constructors
	Expression();
	Expression(const string& s);

	//Getters
	string get_original() const;
	bool get_valid() const;
	string get_postfix() const;
	vector<Token> get_tokenized() const;
	int openSubExpression(int before);
	int expEval();
	bool isIntExp();

	//Setter
	void set(string& s);

	//Others
	void display() const;
    string displayExpression() const;

private:
	//Data members
	string original;
	Exp_type type;
	vector<Token> tokenized;
	void tokenize(string s);
	int isOperator(string token);
	vector<Token> postfix;
	void transformToPostfix();
	bool valid;
	bool isValid();
	bool isAssignment();
	bool isAlternating(SubExpression* subExp);
	vector<SubExpression*> expStack;
	vector<int> values;
	int nextASCII();
	int expValue;
};

#endif /* EXPRESSION_H */