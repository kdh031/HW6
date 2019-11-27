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

string Expression::get_postfix() const
{
	string pf;
	if (postfix.size() > 0) 
	{
		for (int i=0; i< (int)postfix.size(); i++) 
		{
			pf += postfix[i].get_token() + " ";
		}
	} else { pf = "invalid"; }
	return pf;
}

vector<Token> Expression::get_tokenized() const
{
	return tokenized;
}

//Setter
void Expression::set(string& s)
{
	original = s;
	tokenize(s);
	valid = isValid();

	if (valid && isAssignment()) { type = assignment;} 
	else if (valid) { type = arithmetic; } 
	else { type = illegal; }

	if (type == arithmetic) 
	{
		transformToPostfix(); 
		expValue = expEval();
	} 
	else { expValue = 0; }
}

void Expression::transformToPostfix() 
{
	vector<Token> stack;
	vector<Token> output;

	for (int i=0; i< (int)tokenized.size(); i++) 
	{
		string token = tokenized[i].get_token();
		int tokenLevel = isOperator(token);
		 // Left parenthesis
		if (token == "(") { stack.push_back(tokenized[i]); } 
		// Right parenthesis
		else if (token == ")") 
		{                      
			Token t = stack.back();
			while (t.get_token() != "(") 
			{
				if (isOperator(t.get_token())) { output.push_back(t); }
				stack.pop_back();
				t = stack.back();
			}
			stack.pop_back();
		} 
		else if (tokenLevel > 0) 				// Operator
		{                    
			if (stack.size() > 0) {
				string last = stack.back().get_token();
				int lastLevel = isOperator(last);
				if (lastLevel > 0) 
				{
					if (lastLevel >= tokenLevel) 
					{
						Token t = stack.back();
						output.push_back(t);
						output.pop_back();
					}
				}
			}
			stack.push_back(token);
		}
		else if (tokenized[i].get_type() == 0 || tokenized[i].get_type() == 1) 
		{
			output.push_back(token);                    // Operand     
		}
	}
	Token t;
	while (stack.size() > 0) 
	{
		t = stack.back();
		if (isOperator(t.get_token() )) { output.push_back(t); }
		stack.pop_back();
	}
	postfix = output; 
}

int Expression::isOperator(string token) 
{
	if (token == "*" || token == "/") { return 2; }
	else if (token == "+" || token == "-") { return 1; }
	else { return 0; }
}

int Expression::openSubExpression(int before) 
{
	for (int i=before-1; i>=0; i--) 
	{
  		if (expStack[i]->open) { return i; }
  	}
	return -1;
}

int Expression::nextASCII() 
{   // Find an unused variable identity for subexpression substitution
	// Read expression and list existing identities with single alpha
	if (values.size() == 0) 
	{  // First time through only
		for (int i=0; i<int(tokenized.size()); i++) 
		{
			string token = tokenized[i].get_token();
			if (token.length() == 1) 
			{   // Only check single character tokens
				int ascii = (int)token[0];
				// Use lower case alpha characters only
				if (ascii > 96 && ascii < 123) { values.push_back(ascii); }
			}
		}
	} 
	// Find the next available identitiy ascii value 
	for (int j=97; j<123; j++) 
	{
		bool available = true;
		for (int k=0; k< (int)values.size(); k++) 
		{
			if (j == values[k]) { available = false; } // If already used
		}
		if (available) 
		{
			// Add found ascii value to list
			values.push_back(j);
			return j;
		}
	}
	return -1;
}

bool Expression::isAssignment() // Check for assignment operator
{
	for (int i=0; i<int(tokenized.size()); i++) 
	{
		if (tokenized[i].get_type() == 3) { return true; }
	}
	return false;
}

bool Expression::isAlternating(SubExpression* subExp) // Check for infix expression
{
	vector<Token> set = subExp->tokenized; // set = subExpressionTokens
	int setLength = set.size();
	if (setLength == 0 || setLength == 1 || setLength == 2) { return false; } // Minimum 3
	// Check 1st, 3rd, 5th, etc. for ID or INT types
	for (int i=0; i<setLength; i+=2) 
	{
		if (set[i].get_type() != 0 && set[i].get_type() != 1 ) 
		{
			return false;
		} 
	}
	// Check 2nd, 4th, etc. for OP or EQ types
	for (int i=1; i<setLength; i+=2) 
	{
		if (set[i].get_type() != 2 && set[i].get_type() != 3 ) 
		{
			return false;
		} 
	}
	return true;
}

bool Expression::isValid()
{
	// Check for invalid tokens
	for (int i=0; i<int(tokenized.size()); i++) 
	{
		if (tokenized[i].get_type() == 6) { return false; }
	}

	// Parse subexpressions
	for (int i=0; i<int(tokenized.size()); i++) 
	{
		string token = tokenized[i].get_token();

		if (token == "(") 
		{
			// Create new subexpression
			SubExpression* subExp = new SubExpression;
			expStack.push_back(subExp);
		} 
		else if (token == ")") 
		{
			// Add token and close subexpression 
			int lastOpenIndex = openSubExpression(expStack.size());
			if (lastOpenIndex < 0) { break; } // Probably an invalid expression
			expStack[lastOpenIndex]->open = false;

			// Give the subexpression an identity
			int ascii = nextASCII();
	      	string identity = std::string(1, char(ascii));
	      	expStack[lastOpenIndex]->identity = identity;

			// Add the identity to the previous subexpression if it exists
			int previousOpenIndex = openSubExpression(lastOpenIndex);
			if (previousOpenIndex > -1) 
			{
				expStack[previousOpenIndex]->tokenized.push_back(identity);
			} 
			else 
			{
				// If there are remaining tokens create a new subexpression
				if (i < int(tokenized.size()-1)) 
				{
					// First move all subexpressions down one
					int expStackLastIndex = expStack.size() - 1;
					expStack.push_back(expStack[expStackLastIndex]); // Move last one
					for (int j=expStackLastIndex-1; j >= 0; j--) 
					{   // Move others
						expStack[j+1] = expStack[j];
					}

					// Open a new subexpression using the identity above the first subexpression
					SubExpression* subExp = new SubExpression;
					subExp->tokenized.push_back(identity);
					expStack[0] = subExp;
				}
			}
		} 
		else 
		{
			// Add non-parenthesis token to subexpression
			int lastOpenIndex = openSubExpression(expStack.size());
			if (lastOpenIndex > -1) 
			{
				expStack[lastOpenIndex]->tokenized.push_back(tokenized[i]);
			} 
			else 
			{
				// Create new subexpression and add token
				SubExpression* subExp = new SubExpression;
				subExp->tokenized.push_back(tokenized[i]);
				expStack.push_back(subExp);
			}
		}
	}
    
    // If there are no valid subexpressions, it is not a valid expression
	if (expStack.size() == 0) { return false; }  
	// All subexpressions should alternate between id/integer and operator/assignment
    for (int i=0; i< (int)expStack.size(); i++) 
	{
    	if (!isAlternating(expStack[i])) { return false; }
    }
  
	return true;
}


int Expression::expEval ( )
{
	vector<string> stack;
	int answer = 0;

	if(isIntExp())
	{
		for (int i=0; i<int(postfix.size()); i++)
		{
			string token = postfix[i].get_token();
			if (postfix[i].get_type()  == 1)
			{
				stack.push_back(token);
			}
			else if (postfix[i].get_type() == 2)
			{
				string second = stack.back();
				stack.pop_back();
				string first = stack.back();
				stack.pop_back();
				if ( token == "+")
				{
					answer = stoi(first) + stoi(second);
				}
				else if (token == "-")
				{
					answer = stoi(first) - stoi(second);
				}
				else if (token == "/")
				{
					answer = stoi(first) / stoi(second);
				}        
				else if (token == "*")
				{
					answer = stoi(first) * stoi(second);
				}
				stringstream ss;
				ss<<answer;
				string s;
				ss>>s;   
				stack.push_back(s);
			}
		}
	}
	return answer;
}


bool Expression::isIntExp ( )
{
	//check all are integers or operators /,*,+,-
	for (int i=0; i<(int)postfix.size(); i++)
	{
		if(postfix[i].get_type() != 1 && postfix[i].get_type() != 2)
		{
			return false;
		}
	}
	return true;
}


//Others
void Expression::tokenize(string s)
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
	tokenized = list;
}

void Expression::display() const
{
	cout << "original = " << original << endl;
	cout << "tokenized = " << displayExpression() << endl;
	cout << "value = " << expValue << endl;
	cout << "number of tokens = " << tokenized.size() << endl;
	cout << "postfix = ";
	if (postfix.size() > 0) 
	{
		for (int i=0; i< (int)postfix.size(); i++) 
		{
			cout << postfix[i].get_token() << " ";
		}
	} 
	else { cout << "invalid"; }
	cout << endl;
	cout << "valid = " << (valid ? "true" : "false") << endl;
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

