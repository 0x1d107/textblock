#pragma once
#include <string>
#include <deque>
#include <variant>
#include <unordered_map>


class FexpEvaluator {
public:
	using Value = std::variant<std::string,int>;
	using Function = std::function<void(std::deque<Value> &)>;
	FexpEvaluator();
	bool evalutate(const std::string &);
	const std::string *peek();
	void  pop();
	void define_function(const std::string &name, Function func);
private:
	std::deque<Value> _values;
	std::unordered_map<std::string,Function> _functions;

};
