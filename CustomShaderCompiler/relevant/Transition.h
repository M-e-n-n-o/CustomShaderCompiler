#pragma once
#include <string>

#define EPSILON '$'

template<typename T>
class Transition
{
private:
	T m_from;
	char m_symbol;
	T m_to;

	int m_hash;

public:
	Transition(const T& from, char symbol, const T& to): m_from(from), m_symbol(symbol), m_to(to)
	{
		std::hash<T> hasher;
		m_hash = hasher(from + symbol + to);
	}
	
	Transition(const T& fromOrTo, char symbol): Transition(fromOrTo, symbol, fromOrTo) { }
	Transition(const T& from, const T& to): Transition(from, EPSILON, to) {}

	friend bool operator<(const Transition<T>& l, const Transition<T>& r)
	{
		return l.m_hash < r.m_hash;
	}
	
	const T& getFromState() const
	{
		return m_from;
	}

	const char& getSymbol() const
	{
		return m_symbol;
	}

	const T& getToState() const
	{
		return m_to;
	}

	const std::string toString() const
	{
		return getFromState() + "\t+\t" + getSymbol() + "\t-->\t" + getToState();
	}
};
