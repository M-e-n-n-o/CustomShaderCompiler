#include "Automata.h"
#include <iostream>

Automata::Automata(const std::set<std::shared_ptr<State>>& states, const std::shared_ptr<State>& startState,
	const std::shared_ptr<State>& finalState, const std::set<std::shared_ptr<Symbol>>& alfabet) : m_states(states),
	m_startState(startState),
	m_language(alfabet)
{
	m_finalStates.insert(finalState);
}

Automata::Automata(const std::set<std::shared_ptr<State>>& states, const std::shared_ptr<State>& startState,
	const std::set<std::shared_ptr<State>>& finalStates, const std::set<std::shared_ptr<Symbol>>& alfabet) : m_states(states),
	m_startState(startState),
	m_finalStates(finalStates),
	m_language(alfabet) {}

void Automata::addState(std::shared_ptr<State>& state)
{
	m_states.insert(state);
}

void Automata::addStates(std::set<std::shared_ptr<State>>& states)
{
	m_states.insert(states.begin(), states.end());
}

void Automata::setStartState(std::shared_ptr<State>& state)
{
	m_states.insert(state);
	m_startState = state;
}

void Automata::addFinalState(std::shared_ptr<State>& state)
{
	m_states.insert(state);
	m_finalStates.insert(state);
}

void Automata::addSymbol(const std::shared_ptr<Symbol>& symbol)
{
	m_language.insert(symbol);
}

void Automata::addSymbols(std::set<std::shared_ptr<Symbol>>& symbols)
{
	m_language.insert(symbols.begin(), symbols.end());
}

void Automata::printTransitions()
{
	for (auto& state : m_states)
	{		
		std::cout << state->toString() << std::endl;
	}
}

std::shared_ptr<State> Automata::findState(const std::string& stateName)
{
	for (auto& state : m_states)
	{
		if (state->name == stateName)
		{
			return state;
		}
	}

	return nullptr;
}

std::shared_ptr<State> Automata::findState(const std::string& stateName, std::set<std::shared_ptr<State>>& states)
{
	for (auto& state : states)
	{
		if (state->name == stateName)
		{
			return state;
		}
	}

	return nullptr;
}

bool Automata::validate(const std::string& input)
{
	std::vector<std::string> list;
	for (const char& c : input)
	{
		list.emplace_back(1, c);
	}

	return validate(list);
}

bool Automata::validate(const std::vector<std::string>& input)
{
	return validate(m_startState, input);
}

bool Automata::validate(const std::shared_ptr<State>& start, const std::vector<std::string>& input)
{	
	if (input.empty())
	{
		for (auto& finalState : m_finalStates)
		{
			if (start == finalState)
			{
				return true;
			}
		}

		return false;
	}

	bool isValid = false;
	bool didTransition = false;
	for (auto& transition : start->transitions)
	{
		if (transition->symbol->validate(EPSILON))
		{
			std::cerr << "Automata cannot contain any epsilons!" << std::endl;
			return false;
		}
		
		if (transition->symbol->validate(input[0]))
		{
			if (didTransition)
			{
				std::cerr << "Can only check validation of deterministic automata!" << std::endl;
				return false;
			}

			isValid = validate(transition->to, std::vector<std::string>(input.begin() + 1, input.end()));
			didTransition = true;
		}
	}

	if (didTransition)
	{
		return isValid;
	}

	return false;
}

bool Automata::isDeterministic()
{
	for (auto& state : m_states)
	{
		if (!state->isDeterministic(m_language))
		{
			return false;
		}
	}

	return true;
}

void Automata::makeDeterministic()
{
	if (isDeterministic())
	{
		std::cout << "Automata is already deterministic" << std::endl;
		return;
	}
	
	// Create the error state
	auto errorState = std::make_shared<State>("Error");
	for (auto& symbol : m_language)
	{
		auto transition = std::make_shared<Transition>(errorState, symbol);		
		errorState->transitions.push_back(transition);
	}	

	std::set<std::shared_ptr<State>> newStates;
	newStates.insert(m_startState);
	newStates.insert(errorState);

	bool first = true;
	while (true)
	{
		bool done = true;
		std::vector<std::shared_ptr<State>> toAdd;

		for (auto& state : newStates)
		{
			// Are all the new states deterministic?
			if (!state->isDeterministic(m_language))
			{
				// If not, make the state deterministic by defining a transition for every character in the alfabet and generate the states that are needed for these transitions
				auto states = makeDeterministic(state, errorState);
				
				toAdd.clear();
				toAdd.insert(toAdd.end(), states.begin(), states.end());
				
				done = false;
			}
		}

		// Remove the original start state from the new states (this was only added to have a starting point for the algorithm)
		if (first)
		{
			newStates.erase(std::find(newStates.begin(), newStates.end(), m_startState));			
			first = false;
		}

		for (auto& state : toAdd)
		{
			// Add each new generated state to the new states
			auto pair = newStates.insert(state);

			// If the state already existed, transfer the transitions from the new state, which did not already exist, to the original
			if (!pair.second)
			{				
				auto& transitions = (*pair.first)->transitions;
				for (auto& transitionNew : state->transitions)
				{
					bool exists = false;
					for (auto& transitionOld : transitions)
					{						
						if (transitionOld->symbol == transitionNew->symbol)
						{
							exists = true;
						}
					}

					if (!exists)
					{
						auto to = findState(transitionNew->to->name, newStates);
						if (to != nullptr)
						{
							transitionNew->to = to;	
						}
						
						transitions.push_back(transitionNew);
					}
				}
			}
		}

		// Only stop iterating till all states are deterministic
		if (done)
		{
			break;
		}
	}

	// Set the new states
	m_states.clear();
	m_states = newStates;

	std::set<std::shared_ptr<State>> finalStates;
	for (auto& state : newStates)
	{
		if (state->name == m_startState->name)
		{
			m_startState = state;
		}

		if (state->name.find((*m_finalStates.begin())->name) != std::string::npos)
		{
			finalStates.insert(state);
		}
	}

	m_finalStates = finalStates;
}

std::set<std::shared_ptr<State>> Automata::makeDeterministic(const std::shared_ptr<State>& start, const std::shared_ptr<State>& error)
{	
	std::set<std::shared_ptr<State>> startStates;

	// Get the original states from the given start state (the start state can be a combined state)
	std::string name = start->name;
	std::string delimiter = ", ";
	int pos = 0;
	std::string token;
	while ((pos = name.find(delimiter)) != std::string::npos)
	{
		token = name.substr(0, pos);
		name.erase(0, pos + delimiter.length());

		startStates.insert(findState(token));
	}
	startStates.insert(findState(name));

	// Check if there are any other possible start states by using epsilon transitions
	for (auto& transition : start->transitions)
	{
		if (transition->symbol->validate(EPSILON))
		{			
			startStates.insert(transition->to);
		}
	}

	// Determine the state name
	std::string stateName;
	int i = 0;
	for (auto& state : startStates)
	{
		if (i == 0)
		{
			stateName += state->name;
		} else
		{
			stateName += delimiter + state->name;
		}

		i++;
	}
	auto startState = std::make_shared<State>(stateName);

	// For each symbol of the alfabet
	std::vector<std::pair<std::shared_ptr<Symbol>, std::set<std::shared_ptr<State>>>> possibleNextStates;
	for (auto& symbol : m_language)
	{		
		auto statesPerSymbol = std::make_pair(symbol, std::set<std::shared_ptr<State>>());

		// Check if there are any possible states to go to with the symbol
		for (auto& state : startStates)
		{			
			for (auto& transition : state->transitions)
			{
				if (transition->symbol == symbol)
				{
					statesPerSymbol.second.insert(transition->to);
				}
			}
		}

		// Check if there are any other possible states to go to from the already found possible states by using epsilon transitions
		std::vector<std::shared_ptr<State>> epsilonTransitions(statesPerSymbol.second.begin(), statesPerSymbol.second.end());
		for (int i = 0; i < epsilonTransitions.size(); i++)
		{
			auto& state = epsilonTransitions[i];
			
			for (auto& transition : state->transitions)
			{
				if (transition->symbol->validate(EPSILON))
				{
					epsilonTransitions.push_back(transition->to);
				}
			}
		}
		
		statesPerSymbol.second.insert(epsilonTransitions.begin(), epsilonTransitions.end());
		possibleNextStates.push_back(statesPerSymbol);
	}

	// Put all the generated states in a set
	std::set<std::shared_ptr<State>> states;
	states.insert(startState);

	// For every symbol in the alfabet, generate a transition for the start state
	for (auto& statesPerSymbol : possibleNextStates)
	{
		// If there were no possible transitions found, make a transition to the error state
		if (statesPerSymbol.second.empty())
		{
			auto transition = std::make_shared<Transition>(startState, statesPerSymbol.first, error);
			startState->transitions.push_back(transition);
			continue;
		}

		// Generate a new name for the combined states
		std::string stateName;
		int i = 0;
		for (auto& state : statesPerSymbol.second)
		{
			if (i == 0)
			{
				stateName += state->name;
			}
			else
			{
				stateName += delimiter + state->name;
			}
			
			i++;
		}

		// Generate a new state the start state will be able to transition to
		auto nextState = std::make_shared<State>(stateName);
		auto pair = states.insert(nextState);

		if (!pair.second)
		{
			// If the state already existed (only happens when a transition is to itself), generate a transition to itself
			auto transition = std::make_shared<Transition>(startState, statesPerSymbol.first);
			startState->transitions.push_back(transition);			
		} else
		{
			// If not, generate a normal transition to the new state
			auto transition = std::make_shared<Transition>(startState, statesPerSymbol.first, nextState);
			startState->transitions.push_back(transition);
		}
	}

	return states;
}
