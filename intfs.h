#pragma once
#include <sstream>
struct TextBlock{
	virtual void begin(std::stringstream & ss) =0;
	virtual void end(std::stringstream &ss) =0;
	virtual void line(std::stringstream &ss)=0;
	virtual ~TextBlock() = default;
};
