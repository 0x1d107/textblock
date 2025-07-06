#include "tokens.h"

std::string get_parens(std::istream &ss,char open = '[',char close=']'){
	std::stringstream parenss;
	char c;
	int counter=0;
	ss.get(c);
	if(c!=open||ss.eof())
		return "";
	counter++;
	parenss << c;
	while(counter > 0 && ! ss.eof()){
		ss.get(c);
		if(c == open)
			counter++;
		if(c == close)
			counter--;
		parenss << c;
	}	
	return parenss.str();
}
