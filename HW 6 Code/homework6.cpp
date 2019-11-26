// Homework 6
// By Kalia Hall
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

#include "Sequence.h"
#include "Expression.h"
#include "Token.h"

using namespace std;

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

	Expression exp01("((2+3)*4)");
	exp01.display();
	Expression exp02("a1+b2-cc");
	exp02.display();
	Expression exp03("a1=1");
	exp03.display();
	Expression exp04("a+b-c*d/e");
	exp04.display();
	Expression exp05("(a+(b-d)*d)/e");
	exp05.display();
	Expression exp06("5*((2+3)*(4+5))+4");
	exp06.display();

	Expression exp07("3+ + 4");
	exp07.display();
	Expression exp08(")2+3");
	exp08.display();
	Expression exp09("(2+3))");
	exp09.display();
	Expression exp10("a b + c");
	exp10.display();

	Expression exp0("((2+3)*(4+5))+4");
	exp0.display();
	Expression exp1("a + b = 5");
	exp1.display();
	Expression exp11("(a + b) = 5");
	exp11.display();
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