#pragma once
#include <set>
#include <iostream>
#include <vector>
#include "Transition.h"

template<typename T>
class Automata
{
private:
	std::set<T> m_states;
	std::set<T> m_startStates;
	std::set<T> m_finalStates;
	std::vector<char> m_symbols;
	std::set<Transition<T>> m_transitions;

public:
	Automata() = default;
	Automata(const std::string& regex);

	void setAlphabet(const std::initializer_list<char>& symbols)
	{
		m_symbols = symbols;
	}

	void addTransition(const Transition<T>& t)
	{
		m_transitions.insert(t);
		m_states.insert(t.getFromState());
		m_states.insert(t.getToState());
	}

	void addStartState(const T& t)
	{
		m_states.insert(t);
		m_startStates.insert(t);
	}

	void addFinalState(const T& t)
	{
		m_states.insert(t);
		m_finalStates.insert(t);
	}

	bool isDeterministic();

	bool validate(const T& start, const std::string& input);

	void printTransitions();
};

template <typename T>
bool Automata<T>::isDeterministic()
{
	bool isDFA = true;

	for (const T& from : m_states)
	{
		for (const char& symbol : m_symbols)
		{
			isDFA = isDFA && getToStates(from, symbol).size() == 1;
		}
	}

	return isDFA;
}

template <typename T>
bool Automata<T>::validate(const T& start, const std::string& input)
{	
	if (std::find(m_finalStates.begin(), m_finalStates.end(), start) != m_finalStates.end() && input.empty())
	{
		return true;
	}

	// std::cout << "Current state: " + start << ", input: " << input << std::endl;
	
	bool isValid = false;
	bool didTransition = false;
	for (const auto& t : m_transitions)
	{
		if (t.getFromState() != start)
		{
			continue;
		}

		// std::cout << "Possible transition with: " << t.getSymbol() << ", to: " << t.getToState() << std::endl;

		if (t.getSymbol() == input[0])
		{
			// std::cout << "Transition!" << std::endl;
			
			if (didTransition)
			{
				std::cout << "Can only check validation of deterministic automata!" << std::endl;
				return false;
			}

			isValid = validate(t.getToState(), input.substr(1));
			didTransition = true;
		}
	}

	if (didTransition)
	{
		return isValid;
	}

	return false;
}

template <typename T>
void Automata<T>::printTransitions()
{
	for (const Transition<T>& t : m_transitions)
	{
		std::cout << t.toString() << std::endl;
	}
}

template <typename T>
Automata<T>::Automata(const std::string& regex)
{

}
