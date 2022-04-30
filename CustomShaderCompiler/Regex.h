#pragma once
#include <string>
#include <vector>

class Regex
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

	Regex* m_left;
	Regex* m_right;

	Operator m_operator;
	std::string m_terminals;

public:
	Regex();
	Regex(const std::string& expression);

	Regex* plus();
	Regex* star();
	Regex* orr(Regex* other);
	Regex* dot(Regex* other);

	bool isValid(const std::string& sequence, int maxSteps);
	std::vector<std::string> getLanguage(int maxSteps);
};