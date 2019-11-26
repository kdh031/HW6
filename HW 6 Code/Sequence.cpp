//Sequence.cpp
#include "Sequence.h"
#include "Expression.h"
#include "Token.h"

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
