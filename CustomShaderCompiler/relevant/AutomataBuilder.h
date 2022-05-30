#pragma once
#include <stack>
#include <set>
#include "Automata.h"
#include "Regex.h"

/**
 * \brief Generates a NFA using the Thompson's construction
 */
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

	AutomataBuilder& addSymbol(char symbol);
	AutomataBuilder& addUnion();
	AutomataBuilder& addConcatenation();
	AutomataBuilder& addClosure();

	Automata construct();
};