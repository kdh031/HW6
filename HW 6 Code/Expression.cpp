//Expression.cpp
#include "Expression.h"
#include "Token.h"

//Constructors
Expression::Expression()
{
	original = "";
	valid = false;
	type = illegal;
}

Expression::Expression(const string& s)
{
	string str = s;
	set(str);
}

//Getters
string Expression::get_original() const
{
	return original;
}

bool Expression::get_valid() const
{
	return valid;
}

vector<Token> Expression::get_tokenized() const
{
	return tokenized;
}

//Setter
void Expression::set(string& s)
{
	original = s;
	tokenized = tokenize(s);
	// assignment, arithmetic, illegal
	type = illegal;
	valid = isValid();
}

bool Expression::isValid()
{
	for (int i=0; i<int(tokenized.size()); i++) 
	{
		if (tokenized[i].get_type() == 6) { return false; }
	}
	return true;
}

//Others
vector<Token> Expression::tokenize(string s)
{
	vector<Token> list;
	string tkn = "";
	// for each character in the string
	for(int i=0; i<int(s.length()); i++)
	{	
		string chr = string(1, s[i]);
		// if space, discard, close previous token if open
		if (chr == " ")
		{
            if (tkn != "")
			{
				Token token(tkn);
				list.push_back(token);
				tkn = "";
			}
		}
		// else if single-char type, tokenize, close previous token if open
        else if (chr == "(" || chr == ")" || chr == "=" || chr == "+" || chr == "-" || chr == "*" || chr == "/") 
		{
			if (tkn != "")
			{
                Token token(tkn);
				list.push_back(token);
				tkn = "";
			}
			Token token(chr);
			list.push_back(token);
		}
		// else add to open token
		else { tkn = tkn + chr; }
	}
	if (tkn != "")
	{
		Token token(tkn);
		list.push_back(token);
	}
	return list;
}

void Expression::display() const
{
	cout << "original = " << original << endl;
	cout << "tokenized = " << displayExpression() << endl;
	cout << "number of tokens = " << tokenized.size() << endl;
	cout << "postfix =  " << endl;
	cout << "valid = " << valid << endl;
	//type based off enum Exp_type {assignment, arithmetic, illegal}
	if(type == 0) { cout << "type = " << "assignment" << endl; }
    if(type == 1) { cout << "type = " << "arithmetic" << endl; }
    if(type == 2) { cout << "type = " << "illegal"    << endl; }
    cout << endl;
}

string Expression::displayExpression() const
{
	string display = "";
	for(int i=0; i<int(tokenized.size()); i++)
	{
		display = display + tokenized[i].get_token();
	}
	return display;
}

