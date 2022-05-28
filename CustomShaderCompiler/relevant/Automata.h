#pragma once
#include <set>
#include <stack>
#include <memory>
#include "State.h"
#include "Regex.h"

class Automata
{
private:
	std::set<std::shared_ptr<State>> m_states;
	std::shared_ptr<State> m_startState;
	std::shared_ptr<State> m_finalState;
	std::set<char> m_alfabet;

public:
	Automata() = default;
	Automata(const std::set<std::shared_ptr<State>>& states, const std::shared_ptr<State>& startState, const std::shared_ptr<State>& finalState, const std::set<char>& alfabet);
	~Automata() = default;
	
	void addState(std::shared_ptr<State>& state);
	void addStates(std::set<std::shared_ptr<State>>& states);
	void setStartState(std::shared_ptr<State>& state);
	void setFinalState(std::shared_ptr<State>& state);
	void addSymbol(char symbol);
	void addSymbols(std::set<char>& symbols);

	std::set<std::shared_ptr<State>>& getStates() {	return m_states; }
	std::shared_ptr<State>& getStartState() { return m_startState; }
	std::shared_ptr<State>& getFinalState() { return m_finalState; }
	std::set<char>& getAlfabet() { return m_alfabet; }

	void makeDeterministic();
	
	bool validate(const std::string& input);
	
	void printTransitions();

private:
	bool validate(const std::shared_ptr<State>& start, const std::string& input);

	// Powerset construction
	std::vector<std::shared_ptr<State>> makeDeterministic(const std::shared_ptr<State>& start, const std::shared_ptr<State>& error);
};

class AutomataBuilder
{
private:
	int m_stateCount = 0;

	std::set<char> m_symbols;

	std::stack<Automata> m_automatas;

public:
	AutomataBuilder() = default;
	AutomataBuilder(Regex& regex);
	~AutomataBuilder() = default;

	// Thompson's construction
	AutomataBuilder& addSymbol(char symbol);
	AutomataBuilder& addUnion();
	AutomataBuilder& addConcatenation();
	AutomataBuilder& addClosure();

	Automata construct();
};