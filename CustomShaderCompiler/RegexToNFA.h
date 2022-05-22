#pragma once
#include <string>
#include <stack>

#define OPEN '('
#define CLOSE ')'
#define STAR '*'
#define DOT '.'
#define OR '|'

enum class Symbol
{
	// Sorted on priority (Shunting yard algorithm)
	Open = 3,
	Close = 3,
	
	Star = 2,
	Dot = 1,
	Or = 0
};

struct Operator
{
	Symbol symbol;
	char character;
};

bool isOperator(char c)
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

int getOperator(char c)
{
	switch (c)
	{
	case OPEN:	return (int) Symbol::Open;
	case CLOSE:	return (int) Symbol::Close;
	case STAR:	return (int) Symbol::Star;
	case DOT:	return (int) Symbol::Dot;
	case OR:	return (int) Symbol::Or;
	default:	return -1;
	}
}

std::string postfix(std::string& regex)
{
	std::string output;
	std::stack<char> operatorStack;

	for (char& c : regex)
	{
		if (!isOperator(c))
		{
			output += c;
		}
		else
		{
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
				int o = getOperator(c);
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
	}

	while (!operatorStack.empty())
	{
		char top = operatorStack.top();
		output += top;
		operatorStack.pop();
	}

	return output;
}


void regexToNFATest()
{
	std::string r = "a.(a|b)*.b";
	std::string regex = postfix(r);

	std::cout << "Converted regex:\t" << r << "\nTo postfix:\t\t" << regex << std::endl;
}
