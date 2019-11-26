//Sequence.h

#ifndef SEQUENCE_H
#define SEQUENCE_H

#include <vector>
#include <iostream>
#include "Token.h"
#include "Expression.h"

using namespace std;

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

#endif /* SEQUENCE_H */