#pragma once
#include <set>
#include <memory>
#include "State.h"

class Automata
{
private:
	std::set<std::shared_ptr<State>> m_states;
	std::shared_ptr<State> m_startState;
	std::set<std::shared_ptr<State>> m_finalStates;
	std::set<char> m_alfabet;

public:
	Automata() = default;
	Automata(const std::set<std::shared_ptr<State>>& states, const std::shared_ptr<State>& startState, const std::shared_ptr<State>& finalState, const std::set<char>& alfabet);
	Automata(const std::set<std::shared_ptr<State>>& states, const std::shared_ptr<State>& startState, const std::set<std::shared_ptr<State>>& finalStates, const std::set<char>& alfabet);
	~Automata() = default;
	
	void addState(std::shared_ptr<State>& state);
	void addStates(std::set<std::shared_ptr<State>>& states);
	void setStartState(std::shared_ptr<State>& state);
	void addFinalState(std::shared_ptr<State>& state);
	void addSymbol(char symbol);
	void addSymbols(std::set<char>& symbols);

	std::set<std::shared_ptr<State>>& getStates() {	return m_states; }
	std::shared_ptr<State>& getStartState() { return m_startState; }
	std::set<std::shared_ptr<State>>& getFinalStates() { return m_finalStates; }
	std::set<char>& getAlfabet() { return m_alfabet; }

	void makeDeterministic();
	
	bool validate(const std::string& input);
	
	void printTransitions();

private:
	bool validate(const std::shared_ptr<State>& start, const std::string& input);

	std::shared_ptr<State> findState(const std::string& stateName);
	std::shared_ptr<State> findState(const std::string& stateName, std::set<std::shared_ptr<State>>& states);
	
	// Powerset construction
	std::set<std::shared_ptr<State>> makeDeterministic(const std::shared_ptr<State>& start, const std::shared_ptr<State>& error);
};