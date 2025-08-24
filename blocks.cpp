#include "blocks.h"
#include <cctype>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <unistd.h>
#include <sys/wait.h>
void FilterBlock::begin(std::stringstream &ss){
	ss>>std::ws;	
	std::string cmd;
	cmd = ss.str().substr(ss.tellg());
	if(!cmd.size()){
		std::cerr << "bad filter executable name" <<std::endl;
		exit(1);
	}
	subproc filter;
	if(pipe(filter.pipesfd)) {
		throw std::runtime_error(strerror(errno));
	}
	pid_t pid = fork();
	if(pid == -1)
		throw std::runtime_error("fork failed");
	if(!pid){
		close(filter.pipesfd[1]);
		dup2(filter.pipesfd[0], STDIN_FILENO);
		close(filter.pipesfd[0]);
		const char *argv[] = {"/bin/sh","-c",cmd.c_str(),NULL};
		execv("/bin/sh",const_cast<char* const*>(argv));
		exit(1);
	}
	filter.pid = pid;
	procs.push_back(filter);
}
void FilterBlock::end(std::stringstream &){
	subproc proc = procs.back();
	procs.pop_back();
	close(proc.pipesfd[1]);
	close(proc.pipesfd[0]);
	int status;
	waitpid(proc.pid,&status,0);
}
void FilterBlock::line(std::stringstream &ss){
	subproc proc = procs.back();
	std::string linestr = ss.str();
	size_t line_sz = linestr.size();
	const char *line_buf = linestr.c_str();
	write(proc.pipesfd[1],line_buf,line_sz);
	write(proc.pipesfd[1],"\n",1);
}

ExpansionBlock::ExpansionBlock(InputHandler *handler){
	_handler = handler;
}
void ExpansionBlock::begin(std::stringstream &){
	
}
void ExpansionBlock::end(std::stringstream &){
	
}
void ExpansionBlock::line(std::stringstream &ss){
	while(!ss.eof()){
		char c;
		ss.get(c);
		if(ss.eof()){
			std::cout << std::endl;
			return;
		}
		if(c == '\\'){
			ss.get(c);
			std::cout<<c;
			continue;
		} else if( c == '$'){
			ss.get(c);
			std::stringstream varname;
			while(std::isalnum(c)&&!ss.eof()){
				varname << c;
				ss.get(c);
			}
			// TODO expand variable
			auto it = _handler->variables.find(varname.str());
			if(it == _handler->variables.end()){
				continue;
			}
			std::cout << it->second;
			if(!ss.eof())
				std::cout<<c;
			else 
				std::cout<< std::endl;
			continue;
		}	
		std::cout<<c;
	}
}
