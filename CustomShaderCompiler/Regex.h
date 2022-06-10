#pragma once
#include <string>
#include <stack>
#include <set>
#include "Automata.h"
#include "Symbol.h"

#define OPEN '('
#define CLOSE ')'
#define STAR '*'
#define DOT '.'
#define OR '|'

#define WORD_SPLIT '\''

class Regex
{
private:
	std::string m_regex;
	std::string m_postfix;
	
public:
	Regex(const std::string& regex);
	~Regex() = default;

	const std::string& getRegex(bool postfixNotation = false);
	Automata constructAutomata();

private:
	enum class Operator
	{
		None = -1,

		// Sorted on priority (Shunting yard algorithm)	
		Star = 2,
		Dot = 1,
		Or = 0
	};
	
	static bool IsOperator(char c);

	// Result can be casted to the Symbol
	static Operator GetOperator(char c);
	
	static std::string postFix(const std::string& regex);

	// Generates a NFA using the Thompson's construction
	class AutomataBuilder
	{
	private:
		int m_stateCount = 0;

		std::set<std::shared_ptr<Symbol>> m_symbols;

		std::stack<Automata> m_automatas;

	public:
		AutomataBuilder(const std::string& postfix);
		~AutomataBuilder() = default;

		AutomataBuilder& addSymbol(const std::string& symbol);
		AutomataBuilder& addUnion();
		AutomataBuilder& addConcatenation();
		AutomataBuilder& addClosure();

		Automata construct();
	};
};
