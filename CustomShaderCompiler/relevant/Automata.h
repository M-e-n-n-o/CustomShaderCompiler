#pragma once
#include <set>
#include <memory>
#include "State.h"
#include "Symbol.h"

class Automata
{
private:
	std::set<std::shared_ptr<State>> m_states;
	std::shared_ptr<State> m_startState;
	std::set<std::shared_ptr<State>> m_finalStates;
	std::set<std::shared_ptr<Symbol>> m_language;

public:
	Automata() = default;
	Automata(const std::set<std::shared_ptr<State>>& states, const std::shared_ptr<State>& startState, const std::shared_ptr<State>& finalState, const std::set<std::shared_ptr<Symbol>>& alfabet);
	Automata(const std::set<std::shared_ptr<State>>& states, const std::shared_ptr<State>& startState, const std::set<std::shared_ptr<State>>& finalStates, const std::set<std::shared_ptr<Symbol>>& alfabet);
	~Automata() = default;
	
	void addState(std::shared_ptr<State>& state);
	void addStates(std::set<std::shared_ptr<State>>& states);
	void setStartState(std::shared_ptr<State>& state);
	void addFinalState(std::shared_ptr<State>& state);
	void addSymbol(const std::shared_ptr<Symbol>& symbol);
	void addSymbols(std::set<std::shared_ptr<Symbol>>& symbols);

	std::set<std::shared_ptr<State>>& getStates() {	return m_states; }
	std::shared_ptr<State>& getStartState() { return m_startState; }
	std::set<std::shared_ptr<State>>& getFinalStates() { return m_finalStates; }
	std::set<std::shared_ptr<Symbol>>& getLanguage() { return m_language; }

	void makeDeterministic();

	bool validate(const std::string& input);
	bool validate(const std::vector<std::string>& input);
	
	void printTransitions();

private:
	bool validate(const std::shared_ptr<State>& start, const std::vector<std::string>& input);

	std::shared_ptr<State> findState(const std::string& stateName);
	std::shared_ptr<State> findState(const std::string& stateName, std::set<std::shared_ptr<State>>& states);
	
	// Powerset construction
	std::set<std::shared_ptr<State>> makeDeterministic(const std::shared_ptr<State>& start, const std::shared_ptr<State>& error);
};