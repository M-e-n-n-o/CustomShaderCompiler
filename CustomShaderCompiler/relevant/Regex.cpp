#include "Regex.h"
#include <stack>

Regex::Regex(const std::string& regex)
{
	m_regex = postFix(regex);
}

bool Regex::isOperator(char c)
{
	switch (c)
	{
	case OPEN:	return true;
	case CLOSE:	return true;
	case STAR:	return true;
	case DOT:	return true;
	case OR:	return true;
	default:	return false;
	}
}

int Regex::getOperatorPriority(char c)
{
	switch (c)
	{
	case STAR:	return (int) Symbol::Star;
	case DOT:	return (int) Symbol::Dot;
	case OR:	return (int) Symbol::Or;
	default:	return -1;
	}
}

std::string Regex::postFix(const std::string& regex)
{
	std::string output;
	std::stack<char> operatorStack;

	for (const char& c : regex)
	{
		if (!isOperator(c))
		{
			output += c;
			continue;
		}

		if (c == OPEN)
		{
			operatorStack.push(c);
		}
		else if (c == CLOSE)
		{
			while (!operatorStack.empty())
			{
				char top = operatorStack.top();

				if (top == OPEN)
				{
					operatorStack.pop();
					break;
				}

				output += top;
				operatorStack.pop();
			}
		}
		else
		{
			int o = getOperatorPriority(c);
			while (!operatorStack.empty())
			{
				char top = operatorStack.top();

				if (top == OPEN)
				{
					break;
				}

				if (top < o)
				{
					break;
				}

				output += top;
				operatorStack.pop();
			}

			operatorStack.push(c);
		}
	}

	while (!operatorStack.empty())
	{
		char top = operatorStack.top();
		output += top;
		operatorStack.pop();
	}

	return output;
}
