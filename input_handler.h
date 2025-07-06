#pragma once
#include <string>
#include <unordered_map>
#include <functional>
#include <deque>
#include <memory>

#include "intfs.h"

typedef std::function<void(std::stringstream &)> sshandler_t;
class InputHandler{
public:
	InputHandler();
	void push_input(std::istream *input);
	void process_input();
	void pop_input();
	std::unordered_map<std::string,std::string> variables;
private:
	void init_commands();
	void begin_block(std::stringstream  &liness);
	void end_block(std::stringstream &liness);
	void include_command(std::stringstream &liness);
	void swap_input(std::stringstream &liness);
	void setenv_command(std::stringstream &ss);
	void setvar_command(std::stringstream &ss);
	void quote_command(std::stringstream &ss);
	template <typename T> void register_command(const std::string &name, T func){
		commands[name] = std::bind(func,this,std::placeholders::_1);
	}
	private: //fields
	std::unordered_map<std::string,sshandler_t>commands;
	std::string cmd_prefix="#%";
	std::deque<TextBlock *> block_stack;
	std::deque<std::istream *> input_stack; 
	std::unordered_map<std::string,std::unique_ptr<TextBlock>> block_handlers;
};
