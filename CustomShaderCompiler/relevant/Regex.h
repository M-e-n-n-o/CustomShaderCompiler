#pragma once
#include <string>
#include <stack>
#include <set>
#include "Automata.h"

#define OPEN '('
#define CLOSE ')'
#define STAR '*'
#define DOT '.'
#define OR '|'

class Regex
{
private:
	std::string m_regex;
	std::string m_postfix;
	
public:
	Regex(const std::string& regex);
	~Regex() = default;

	const std::string& getRegex(bool postfixNotation = false);
	Automata constructAutomata() const;

private:
	enum class Symbol
	{
		None = -1,

		// Sorted on priority (Shunting yard algorithm)	
		Star = 2,
		Dot = 1,
		Or = 0
	};
	
	static bool IsOperator(char c);

	// Result can be casted to the Symbol
	static Symbol GetOperator(char c);
	
	static std::string postFix(const std::string& regex);

	// Generates a NFA using the Thompson's construction
	class AutomataBuilder
	{
	private:
		int m_stateCount = 0;

		std::set<char> m_symbols;

		std::stack<Automata> m_automatas;

	public:
		AutomataBuilder(const std::string& postfix);
		~AutomataBuilder() = default;

		AutomataBuilder& addSymbol(char symbol);
		AutomataBuilder& addUnion();
		AutomataBuilder& addConcatenation();
		AutomataBuilder& addClosure();

		Automata construct();
	};
};
