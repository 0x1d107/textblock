#include <deque>
#include <functional>
#include <iostream>
#include <memory>
#include <memory>
#include <string>
#include <sstream>
#include <unordered_map>
#include "blocks.h"

typedef std::function<void(std::stringstream &)> sshandler_t;


std::unordered_map<std::string,std::unique_ptr<TextBlock>> block_handlers;
std::deque<TextBlock *> block_stack;

void begin_block(std::stringstream  &liness){
	std::string name;
	liness >>name;
	if(block_handlers.count(name)){
		block_handlers[name]->begin(liness);
		block_stack.push_back(block_handlers[name].get());
		std::cerr << "Started block:"<< name<<std::endl;
	}
}

void end_block(std::stringstream &liness){
	block_stack.back()->end(liness);
	block_stack.pop_back();
	std::cerr << "Ended block"<<std::endl;
}

void init_commands( std::unordered_map<std::string,sshandler_t> &commands){
	commands["block"] = begin_block;
	block_handlers["filter"] = std::make_unique<FilterBlock>();
	commands["end"] = end_block;

}



int main(){
	std::unordered_map<std::string,sshandler_t>commands;
	init_commands(commands);

	const std::string cmd_prefix="#%";
	for(std::string line; std::getline(std::cin,line);){
		std::stringstream liness(line);
		if(line.substr(0,cmd_prefix.size()) == cmd_prefix){
			liness.ignore(cmd_prefix.size());
			std::string cmd;
			liness >> cmd;
			if(commands.count(cmd))
				commands[cmd](liness);
		}else {
			if(block_stack.size())
				block_stack.back()->line(liness);
			else
				std::cout << line<<std::endl;
		}
	}
}
