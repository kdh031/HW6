//Expression.h
#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <vector>
#include <iostream>
#include "Token.h"
using namespace std;

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
	vector<Token> get_tokenized() const;

	//Setter
	void set(string& s);

	//Others
	void display() const;
    string displayExpression() const;

private:
	//Data members
	string original;
	vector<Token> tokenized;
	vector<Token> postfix;
	bool valid;
	bool isValid();
	Exp_type type;
	vector<Token> tokenize(string s);
};

#endif /* EXPRESSION_H */