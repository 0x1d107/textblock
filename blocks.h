#pragma once
#include <array>
#include <deque>
#include <sstream>
struct TextBlock{
	virtual void begin(std::stringstream & ss) =0;
	virtual void end(std::stringstream &ss) =0;
	virtual void line(std::stringstream &ss)=0;
	virtual ~TextBlock() = default;
};

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
