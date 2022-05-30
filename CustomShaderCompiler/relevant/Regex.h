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
	std::string m_postfix;
	
public:
	Regex(const std::string& regex);
	~Regex() = default;

	enum class Symbol
	{
		None = -1,
		
		// Sorted on priority (Shunting yard algorithm)	
		Star = 2,
		Dot = 1,
		Or = 0
	};

	const std::string& getRegex(bool postfixNotation = false) const;

	static bool IsOperator(char c);
	
	// Result can be casted to the Symbol
	static Symbol GetOperator(char c);

private:
	static std::string postFix(const std::string& regex);
};
