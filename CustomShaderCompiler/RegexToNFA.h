#pragma once
#include <iostream>
#include <string>
#include <stack>
#include <vector>

#define EPSILON 'ε'

#define OPEN '('
#define CLOSE ')'
#define STAR '*'
#define DOT '.'
#define OR '|'

enum class Symbol
{
	// Sorted on priority (Shunting yard algorithm)	
	Star = 2,
	Dot = 1,
	Or = 0
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
	case STAR:	return (int) Symbol::Star;
	case DOT:	return (int) Symbol::Dot;
	case OR:	return (int) Symbol::Or;
	default:	return -1;
	}
}

std::string postfix(const std::string& regex)
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

	while (!operatorStack.empty())
	{
		char top = operatorStack.top();
		output += top;
		operatorStack.pop();
	}

	return output;
}


struct Transition;

struct State
{
	int id;
	bool isEnd = false;
	std::vector<Transition> transitions;
};

struct Transition
{
	std::shared_ptr<State> from;
	char transition;
	std::shared_ptr<State> to;
};

struct Diagram
{
	std::shared_ptr<State> start;
	std::vector<std::shared_ptr<State>> states;
};

std::tuple<std::shared_ptr<State>, std::shared_ptr<State>> createStatesWithTransition(int idFrom, char transition, int idTo, Diagram& diagram)
{	
	auto from = std::make_shared<State>();
	from->id = idFrom;

	auto to = std::make_shared<State>();
	to->id = idTo;

	from->transitions.push_back({ from, transition, to });
	return std::make_tuple(from, to);
}

std::tuple<std::shared_ptr<State>, std::shared_ptr<State>> createStatesWithTransition(std::shared_ptr<State>& from, char transition, int id2, Diagram& diagram)
{
	auto to = std::make_shared<State>();
	to->id = id2;

	from->transitions.push_back({ from, transition, to });
	return std::make_tuple(from, to);
}

std::tuple<std::shared_ptr<State>, std::shared_ptr<State>> createStatesWithTransition(int idFrom, char transition, std::shared_ptr<State>& to, Diagram& diagram)
{
	auto from = std::make_shared<State>();
	from->id = idFrom;

	from->transitions.push_back({ from, transition, to });
	return std::make_tuple(from, to);
}


Diagram postfixToNFA(const std::string& regex)
{
	Diagram d;

	int idCount = 0;
	for (const char& c : regex)
	{
		if (!isOperator(c))
		{
			createStatesWithTransition(idCount, c, idCount + 1, d);
			idCount += 2;
			continue;	
		}

		Symbol o = (Symbol) getOperator(c);
		switch (o)
		{
		case Symbol::Star: break;
		case Symbol::Dot: break;
		case Symbol::Or:
			auto pair = createStatesWithTransition(idCount, EPSILON, idCount + 1, d);
			std::get<0>(pair);
			break;
			
		default: ;
		}
	}

	return d;
}

void regexToNFATest()
{
	std::string regex = "a.(a|b)*.b";
	std::string convertex = postfix(regex);

	std::cout << "Converted regex:\t" << regex << "\nTo postfix:\t\t" << convertex << std::endl;

	postfixToNFA(regex);
}
