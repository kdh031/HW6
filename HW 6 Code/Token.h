//Token.h
#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <sstream>
using namespace std;

//Declare enum
enum Token_type {ID, INT, OP, EQ, OpenBrace, CloseBrace, INVALID};

class Token
{
public:
	//Constructors
	Token();
	Token(string s);

	//Getters
	Token_type get_type() const;
	string get_token() const;
	int get_priority() const;

	//Setter
	void set(string s);

	//Others
	int value() const;
	void display() const;

private:
	//Data members
	Token_type type;
	string token;
	int priority;
	bool isCharacter(char c);
	bool isNumber(string s);
	bool isIdentifier(string s);
};

#endif /* TOKEN_H */