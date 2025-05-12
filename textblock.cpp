#include <functional>
#include <iostream>
#include <string>
#include <sstream>
#include <unordered_map>

typedef std::function<void(std::stringstream &)> sshandler_t;


void begin_block(std::stringstream & liness){
	std::cout << "Started block:"<< 1;
}

void init_commands( std::unordered_map<std::string,sshandler_t> &commands
){
	
	commands["block"] = begin_block;
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
			commands[cmd](liness);
		}
	}
}
