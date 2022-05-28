#pragma once
#include <string>
#include <vector>
#include <memory>

#define EPSILON '$'

class State;

class Transition
{
public:
	std::shared_ptr<State> from;
	char symbol;
	std::shared_ptr<State> to;
	
	Transition(const std::shared_ptr<State>& from, char symbol, const std::shared_ptr<State>& to) : from(from), symbol(symbol), to(to) {}
	Transition(const std::shared_ptr<State>& fromOrTo, char symbol) : Transition(fromOrTo, symbol, fromOrTo) { }
	Transition(const std::shared_ptr<State>& from, const std::shared_ptr<State>& to) : Transition(from, EPSILON, to) {}
};

class State
{
private:
	int hash;

public:
	std::string name;
	std::vector<std::shared_ptr<Transition>> transitions;

	State(const std::string& name) : name(name)
	{
		std::hash<std::string> hasher;
		hash = hasher(name);
	}

	friend bool operator<(const State& l, const State& r)
	{
		return l.hash < r.hash;
	}

	const std::string toString() const
	{
		std::string s;
		for (auto& transition : transitions)
		{
			s += name + "\t-- " + transition->symbol + " -->\t" + transition->to->name + "\n";
		}
	
		return s;
	}
};