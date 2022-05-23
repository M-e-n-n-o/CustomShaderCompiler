#pragma once
#include <string>
#include <vector>

#define EPSILON '$'

struct Transition;

struct State
{
public:
	int id;
	std::vector<Transition> transitions;
};

struct Transition
{
public:
	std::string from;
	char symbol;
	std::string to;

	int m_hash;

	Transition(const std::string& from, char symbol, const std::string& to): from(from), symbol(symbol), to(to)
	{
		std::hash<std::string> hasher;
		m_hash = hasher(from + symbol + to);
	}
	
	Transition(const std::string& fromOrTo, char symbol): Transition(fromOrTo, symbol, fromOrTo) { }
	Transition(const std::string& from, const std::string& to): Transition(from, EPSILON, to) {}

	friend bool operator<(const Transition& l, const Transition& r)
	{
		return l.m_hash < r.m_hash;
	}

	const std::string toString() const
	{
		return from + "\t+\t" + symbol + "\t-->\t" + to;
	}
};
