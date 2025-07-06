#pragma once
#include <deque>
#include <sstream>
#include "input_handler.h"
class FilterBlock :public TextBlock{
public:
	void begin(std::stringstream & ss) override;
	void end(std::stringstream &ss) override;
	void line(std::stringstream &ss) override;
private:
	struct subproc{
		int pipesfd[2];
		pid_t pid;
	};
	std::deque<subproc> procs;

};

class ExpansionBlock : public TextBlock{
	public:
	ExpansionBlock(InputHandler *handler);
	private:
	void begin(std::stringstream & ss) override;
	void end(std::stringstream &ss) override;
	void line(std::stringstream &ss) override;
	InputHandler *_handler;

};
