//Token.cpp
#include <iostream>
#include <string>

#include "Token.h"
using namespace std;

//Constructors
Token::Token()
{
	type = INVALID;
	token = "";
	priority = -1;
}

Token::Token(string s)
{
	set(s);
}

//Helpers
bool Token::isCharacter(char c)
{
	int ascii = int(c);
	return ((ascii >= 65 && ascii <=90) || (ascii >= 97 && ascii <=122));
}

bool Token::isNumber(string s)
{
	if (s[0] == '0') { return false; }
	for (int i = 0; i < int(s.length()); i++)
	{
		if (isdigit(s[i]) == false) { return false; }
	}
    return true;
}

bool Token::isIdentifier(string s)
{
	if (!isCharacter(s[0])) { return false; }
	for (int i = 1; i < int(s.length()); i++)
	{
		if (!( isCharacter(s[i]) || isNumber(string(1, s[i])) )) { return false; }
	}
	return true;
}

//Getters
Token_type Token::get_type() const
{
	return type;
}

string Token::get_token() const
{
	return token;
}

int Token::get_priority() const
{
	return priority;
}

//Setter
void Token::set(string s)
{
	token = s;
	priority = -1;

	if(token == "(") { type = OpenBrace; }
	else if(token == ")") { type = CloseBrace; }
	else if(token == "=") { type = EQ; }
	else if(token == "+" || token == "-" || token == "*" || token == "/") { type = OP; }
	else if(isNumber(token)) { type = INT; }
	else if(isIdentifier(token)) { type = ID; }
	else { type = INVALID; }
}

//Others
int Token::value() const
{
	if(type == INT)
	{
		stringstream str(token);
		int num = 0;
		str >>num;
		return num;
	}
	else if(type == ID) { return -1; }
	return -2;
}

void Token::display() const
{
	//type based on num Token_type {ID, INT, OP, EQ, OpenBrace, CloseBrace, INVALID}
	if(type == 0) { cout << "type = " << "ID" << endl; }
	if(type == 1) { cout << "type = " << "INT" << endl; }
	if(type == 2) { cout << "type = " << "OP" << endl; }
	if(type == 3) { cout << "type = " << "EQ" << endl; }
	if(type == 4) { cout << "type = " << "OpenBrace" << endl; }
	else if(type == 5) { cout << "type = " << "CloseBrace" << endl; }
	else if (type == 6) { cout << "type = " << "INVALID" << endl; }
	cout << "token = " << get_token()  << " (value is "<<  value() << ")" << endl;
	cout << "priority = " << get_priority() << endl;
    cout << endl;
}
