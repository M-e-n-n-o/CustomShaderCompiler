#pragma once
#include <set>
#include <iostream>
#include <stack>
#include "Transition.h"
#include "Regex.h"

class Automata
{
private:
	std::set<std::string> m_states;
	std::set<std::string> m_startStates;
	std::set<std::string> m_finalStates;
	std::set<char> m_symbols;
	std::set<Transition> m_transitions;

public:
	Automata() = default;
	Automata(const std::set<std::string>& states, const std::set<std::string>& startStates, const std::set<std::string>& finalStates, const std::set<char>& symbols, const std::set<Transition>& transitions)
		: m_states(states), m_startStates(startStates), m_finalStates(finalStates), m_symbols(symbols), m_transitions(transitions) {}
	~Automata() = default;
	
	void setAlphabet(const std::set<char>& symbols)
	{
		m_symbols = symbols;
	}

	void addTransition(const Transition& t)
	{
		m_transitions.insert(t);
		m_states.insert(t.getFromState());
		m_states.insert(t.getToState());
	}

	void addStartState(const std::string& t)
	{
		m_states.insert(t);
		m_startStates.insert(t);
	}

	void addFinalState(const std::string& t)
	{
		m_states.insert(t);
		m_finalStates.insert(t);
	}

	bool validate(const std::string& start, const std::string& input);

	void printTransitions();

	class Builder
	{
	private:
		std::set<std::string> m_states;
		std::set<std::string> m_startStates;
		std::set<std::string> m_finalStates;
		std::set<char> m_symbols;
		std::set<Transition> m_transitions;

		std::stack<Transition> m_transitionStack;
	
	public:
		Builder() = default;
		~Builder() = default;

		Builder& addSymbol(char a);
		Builder& addUnion();
		Builder& addConcatenation();
		Builder& addClosure();

		Automata construct();
	};
};