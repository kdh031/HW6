// Homework 6
// By Kalia Hall

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
using namespace std;

//===============================Token class===============================================
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

//=====================================Expression class===============================================
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

//=====================================Sequence class===============================================
class Sequence
{
public:
	//Constructors
	Sequence();
	Sequence(const string& s);

	//Getters
	string get_original() const;
	bool get_valid() const;
	vector<Expression> get_parsed() const;

	//Setter
	void set(string& s);

	//Others
    string displaySequence() const;

private:
	//Data members
	string original;
	vector<Expression> parsed;
	bool valid;
	bool isValid();
	vector<Expression> parse(string s);
};


//===============================Token functions===============================================
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


//===============================Expression functions===============================================
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


//================================Sequence Functions========================================================
//Constructors
Sequence::Sequence()
{
	original = "";
	valid = false;
}

Sequence::Sequence(const string& s)
{
	string str = s;
	set(str);
}

//Getters
string Sequence::get_original() const
{
	return original;
}

bool Sequence::get_valid() const
{
	return valid;
}

vector<Expression> Sequence::get_parsed() const
{
	return parsed;
}

//Setter
void Sequence::set(string& s)
{
	original = s;
	parsed = parse(s);
	valid = isValid();
}

bool Sequence::isValid() {
	if (original.length() > 0) 
	{
		if (original.back() != ';') { return false; }
	} 
	else { return true; }

	vector<Expression> seq = get_parsed();
	for(int i=0; i<int(seq.size()); i++)
	{
		if (seq[i].get_original() == "") { return false; }
        if (! seq[i].get_valid()) { return false; }
    }
    return true;
}

//Others
vector<Expression> Sequence::parse(string s)
{
    vector<Expression> list;
    string delim = ";";
    size_t pos = 0;
    string exp;
    while ((pos = s.find(delim)) != string::npos) 
    {
        exp = s.substr(0, pos);
        Expression exp_obj = exp;
        list.push_back(exp_obj);
        s.erase(0, pos + delim.length());
    }
    return list;
}

string Sequence::displaySequence() const
{
	string display = "";
	for(int i=0; i<int(parsed.size()); i++)
	{
    	display = display + parsed[i].displayExpression() + ";";
    }
    return display;
}


//===============================MAIN===============================================
int main()
{
	Token token1("ab");
	token1.display();
	Token token2("(");
	token2.display();
	Token token3(")");
	token3.display();
	Token token4("*");
	token4.display();
	Token token5("+");
	token5.display();
	Token token6("12");
	token6.display();
	Token token7("012");
	token7.display();
	Token token8("=");
	token8.display();

	Expression exp1("a + b = 5");
	exp1.display();
	Expression exp2("(ab+cd)");
	exp2.display();
	Expression exp3("c12 + 44 = 5*g8");
	exp3.display();
	Expression exp4("1$%5+dfv*2@-@7===14 g");
	exp4.display();

	const string start  = "=== expression evaluation program starts ===";
	const string end  = "=== expression evaluation program ends ===";

	cout << endl << start << endl;

	bool loop = true;
	string input;
	string action;
	Sequence seq1;
	while (loop) 
	{
		while (input == "") 
		{
			cout << "input:";
			cin >> input;
		}
		string oldInput = seq1.get_original();
		if (action == "c") { seq1 = seq1.get_original() + input; } 
		else { seq1 = input; }	  
		bool valid = seq1.get_valid();
		if (!valid) 
		{ 
			cout << "Sequence is invalid." << endl; 
			seq1 = oldInput;
			input = "";
		}
		while (valid) 
		{
			cout << "action:";
			cin >> action; 
			if (action == "=") { cout << "evaluate (sequence: " << seq1.displaySequence() << ")" << endl; }
			else if (action == ">") { cout << "display prefix expression" << endl; }
			else if (action == "<") { cout << "display postfix expression" << endl; }
			else if (action == "f") { cout << "display parenthesized expression" << endl; }
			else if (action == "c") { input = ""; break; } 	
			else if (action == "s") { input = ""; break; } 	
			else if (action == "q") { loop = false; break; } 	
		}
	}
	cout << end << endl;
	return 0;
}