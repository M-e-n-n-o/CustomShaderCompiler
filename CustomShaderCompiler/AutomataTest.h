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

	bool valid = false;
	bool didTransition = false;
	for (auto& transition : currentState->transitions)
	{
		if (transition.transitionCharacter == input[0])
		{
			if (didTransition)
			{
				std::cout << "The automata is not deterministic!" << std::endl;
				return false;
			}
			
			valid = isValid(transition.to, input.substr(1));
			didTransition = true;
		}
	}

	if (didTransition)
	{
		return valid;
	}

	return false;
}

std::vector<State> generateStates(const std::string& regex)
{
	std::vector<State> states;
	states.push_back({ 0, false });

	int id = 0;
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

		id++;
		states.push_back({ id, isEnd });
	}

	return states;
}

void addStateTransitions(const std::string& regex, std::vector<State>& states)
{
	int charNumber = 0;
	for (int i = 0; i < states.size(); i++)
	{
		State* currentState = &states[i];

		State* nextState = nullptr;
		if (i + 1 < states.size())
		{
			nextState = &states[i + 1];
		}

		std::vector<Transition> transitions;
		char nextChar = regex[charNumber];

		if (nextChar == '+')
		{
			transitions.push_back({ currentState, regex[charNumber - 1], currentState });
			nextChar = regex[charNumber + 1];
			charNumber++;
		}
		else if (nextChar == '*')
		{
			transitions.push_back({ currentState, regex[charNumber - 1], currentState });
			nextChar = regex[charNumber + 1];
			charNumber++;

			State* previousState = &states[i - 1];

			if (nextState != nullptr)
			{
				previousState->transitions.push_back({ previousState, nextChar, nextState });
			}
			else
			{
				previousState->isEnd = true;
			}
		}

		if (nextState != nullptr)
		{
			transitions.push_back({ currentState, nextChar, nextState });
		}

		currentState->transitions = transitions;
		charNumber++;
	}
}

void printStateTransitions(std::vector<State>& states)
{
	for (int i = 0; i < states.size(); i++)
	{
		State* currentState = &states[i];
		std::cout << "State: " << currentState->id << ", is an end state: " << currentState->isEnd << std::endl;
		for (int j = 0; j < currentState->transitions.size(); j++)
		{
			std::cout << currentState->transitions[j].from->id << "-" << currentState->transitions[j].transitionCharacter << "-> " << currentState->transitions[j].to->id << std::endl;
		}
		std::cout << std::endl;
	}
}

void automataTest()
{
	std::string regex = "b*b";

	// Generate the states
	std::vector<State> states = generateStates(regex);

	// Generate the transitions
	addStateTransitions(regex, states);

	// Print transitions
	printStateTransitions(states);

	// Validate
	std::string input = "b";
	bool valid = isValid(&states[0], input);
	std::cout << "Regex: " << regex << ", input: " << input << ", is valid: " << valid << std::endl;
}