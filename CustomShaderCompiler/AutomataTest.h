#pragma once
#include <iostream>

struct Transition;

struct State
{
	int id;
	bool isEnd;
	std::vector<Transition> transitions;
};

struct Transition
{
	State* from;
	char transitionCharacter;
	State* to;
};

bool isValid(State* currentState, const std::string& input)
{
	if (currentState->isEnd && input.empty())
	{
		return true;
	}
	
	for (auto& transition : currentState->transitions)
	{
		if (transition.transitionCharacter == input[0])
		{
			return isValid(transition.to, input.substr(1));
		}
	}

	return false;
}

void automataTest()
{
	std::string regex = "b+";

	std::vector<State> states;
	states.push_back({ 0, false });
	for (int i = 0; i < regex.size(); i++)
	{
		if (regex[i] == '+' || regex[i] == '*')
		{
			continue;
		}

		std::string nextChars = regex.substr(i + 1);
		bool isEnd = true;
		for (int i = 0; i < nextChars.size(); i++)
		{
			if (nextChars[i] != '+' && nextChars[i] != '*')
			{
				isEnd = false;
			}
		}
		
		states.push_back({ i, isEnd });
	}

	for (int i = 0; i < states.size(); i++)
	{		
		State* currentState = &states[i];

		std::vector<Transition> transitions;
		const char nextChar = regex[i];

		if (nextChar == '+')
		{
			transitions.push_back({ currentState, regex[i - 1], currentState });
		} else if (nextChar == '*')
		{
			
		}

		if (i + 1 < states.size())
		{
			State* nextState = &states[i + 1];
			transitions.push_back({ currentState, nextChar, nextState });
		}
		
		currentState->transitions = transitions;
	}

	std::string input = "bbbbb";
	bool valid = isValid(&states[0], input);
	std::cout << "Regex: " << regex << ", input: " << input << ", is valid: " << valid << std::endl;
}