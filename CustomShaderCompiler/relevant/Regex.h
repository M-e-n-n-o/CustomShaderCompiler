#pragma once
#include <string>

#define OPEN '('
#define CLOSE ')'
#define STAR '*'
#define DOT '.'
#define OR '|'

class Regex
{
private:
	std::string m_regex;
	
public:
	Regex(const std::string& regex);
	~Regex() = default;

	enum class Symbol
	{
		// Sorted on priority (Shunting yard algorithm)	
		Star = 2,
		Dot = 1,
		Or = 0
	};

	const std::string& getRegex() const { return m_regex; }

private:
	static bool isOperator(char c);
	
	// Result can be casted to the Symbol
	static int getOperatorPriority(char c);
	
	static std::string postFix(const std::string& regex);
};
