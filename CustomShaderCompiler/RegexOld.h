#pragma once
#include <string>
#include <vector>

class RegexOld
{
private:
	enum class Operator
	{
		One,
		Plus,
		Star,
		Or,
		Dot
	};

	RegexOld* m_left;
	RegexOld* m_right;

	Operator m_operator;
	std::string m_terminals;

public:
	RegexOld();
	RegexOld(const std::string& expression);

	RegexOld* plus();
	RegexOld* star();
	RegexOld* orr(RegexOld* other);
	RegexOld* dot(RegexOld* other);

	bool isValid(const std::string& sequence, int maxSteps);
	std::vector<std::string> getLanguage(int maxSteps);
};