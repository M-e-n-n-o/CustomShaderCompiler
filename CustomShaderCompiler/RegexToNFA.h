﻿#pragma once
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


void postfixToNFA(const std::string& regex)
{
	// Java code gebruiken van Hans
}

void regexToNFATest()
{
	std::string regex = "a.(a|b)*.b";
	std::string convertex = postfix(regex);

	std::cout << "Converted regex:\t" << regex << "\nTo postfix:\t\t" << convertex << std::endl;

	postfixToNFA(regex);
}