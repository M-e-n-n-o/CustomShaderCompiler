#include "Automata.h"
#include <iostream>

Automata::Automata(const std::set<std::shared_ptr<State>>& states, const std::shared_ptr<State>& startState,
	const std::shared_ptr<State>& finalState, const std::set<char>& alfabet) : m_states(states),
	m_startState(startState),
	m_finalState(finalState),
	m_alfabet(m_alfabet) {}

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

void Automata::setFinalState(std::shared_ptr<State>& state)
{
	m_states.insert(state);
	m_finalState = state;
}

void Automata::addSymbol(char symbol)
{
	m_alfabet.insert(symbol);
}

void Automata::addSymbols(std::set<char>& symbols)
{
	m_alfabet.insert(symbols.begin(), symbols.end());
}

void Automata::printTransitions()
{
	for (auto& state : m_states)
	{
		std::cout << state->toString() << std::endl;
	}
}

bool Automata::validate(const std::string& input)
{
	return validate(m_startState, input);
}

bool Automata::validate(const std::shared_ptr<State>& start, const std::string& input)
{
	if (start == m_finalState && input.empty())
	{
		return true;
	}

	bool isValid = false;
	bool didTransition = false;
	for (auto& transition : start->transitions)
	{
		if (transition->symbol == EPSILON)
		{
			std::cerr << "Automata cannot contain any epsilons!" << std::endl;
			return false;
		}
		
		if (transition->symbol == input[0])
		{
			if (didTransition)
			{
				std::cerr << "Can only check validation of deterministic automata!" << std::endl;
				return false;
			}

			isValid = validate(transition->to, input.substr(1));
			didTransition = true;
		}
	}

	if (didTransition)
	{
		return isValid;
	}

	return false;
}

void Automata::makeDeterministic()
{
	auto errorState = std::make_shared<State>("Error");
	for (auto& symbol : m_alfabet)
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
			if (!state->isDeterministic(m_alfabet))
			{
				auto states = makeDeterministic(state, errorState);
				
				toAdd.clear();
				toAdd.insert(toAdd.end(), states.begin(), states.end());
				
				done = false;
			}
		}

		if (first)
		{
			newStates.erase(std::find(newStates.begin(), newStates.end(), m_startState));
			first = false;
		}

		std::cout << "Current states: " << std::endl;
		for (auto& state : newStates)
			std::cout << state->name << std::endl;
		
		for (auto& state : toAdd)
		{
			std::cout << "Adding a new state: " << state->name << std::endl;
			
			auto pair = newStates.insert(state);
			if (!pair.second)
			{
				std::cout << "State already exists!" << std::endl;
				
				auto& transitions = (*pair.first)->transitions;
				for (int i = 0; i < transitions.size(); i++)
				{
					auto& transitionOld = transitions[i];
					
					for (auto& transitionNew : state->transitions)
					{
						if (transitionOld->symbol != transitionNew->symbol)
						{
							transitions.push_back(transitionNew);
						}
					}
				}
			}
		}

		// std::cout << "Amount of new states: " << newStates.size() << std::endl;
		
		if (done)
		{
			break;
		}
	}

	// TODO
	// Bugs fixen,
	// Eind states bepalen (https://www.youtube.com/watch?v=jMxuL4Xzi_A),
	// Alle states goed in de automaat zetten
}

std::set<std::shared_ptr<State>> Automata::makeDeterministic(const std::shared_ptr<State>& start, const std::shared_ptr<State>& error)
{
	std::set<std::shared_ptr<State>> startStates;
	startStates.insert(start);
	for (auto& transition : start->transitions)
	{
		if (transition->symbol == EPSILON)
		{			
			startStates.insert(transition->to);
		}
	}

	std::string stateName;
	for (auto& state : startStates)
	{
		stateName += state->name;
	}
	auto startState = std::make_shared<State>(stateName);

	std::vector<std::pair<char, std::set<std::shared_ptr<State>>>> possibleNextStates;
	for (auto& symbol : m_alfabet)
	{
		// std::cout << "Processing symbol: " << symbol << std::endl;
		
		auto statesPerSymbol = std::make_pair(symbol, std::set<std::shared_ptr<State>>());

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

		std::vector<std::shared_ptr<State>> epsilonTransitions(statesPerSymbol.second.begin(), statesPerSymbol.second.end());
		for (int i = 0; i < epsilonTransitions.size(); i++)
		{
			auto& state = epsilonTransitions[i];
			
			for (auto& transition : state->transitions)
			{
				if (transition->symbol == EPSILON)
				{
					epsilonTransitions.push_back(transition->to);
				}
			}
		}
		
		statesPerSymbol.second.insert(epsilonTransitions.begin(), epsilonTransitions.end());

		// std::cout << "Added states: ";
		// for (auto& state : statesPerSymbol.second)
		// {
		// 	std::cout << state->name << ", ";
		// }
		// std::cout << std::endl;
		
		possibleNextStates.push_back(statesPerSymbol);
	}

	std::set<std::shared_ptr<State>> states;
	states.insert(startState);

	std::cout << "Created start state: " << stateName << std::endl;
	
	for (auto& statesPerSymbol : possibleNextStates)
	{
		if (statesPerSymbol.second.empty())
		{
			auto transition = std::make_shared<Transition>(startState, statesPerSymbol.first, error);
			startState->transitions.push_back(transition);
			continue;
		}

		std::string stateName;
		for (auto& state : statesPerSymbol.second)
		{
			stateName += state->name;
		}
		
		auto nextState = std::make_shared<State>(stateName);
		auto transition = std::make_shared<Transition>(startState, statesPerSymbol.first, nextState);
		startState->transitions.push_back(transition);

		auto it = states.insert(nextState);

		if (it.second)
		{
			std::cout << "Created next state: " << stateName << std::endl;
		}
	}

	return states;
}


// --------------------------------------------------------------------------
//									BUILDER
// --------------------------------------------------------------------------

AutomataBuilder::AutomataBuilder(Regex& regex)
{
	for (auto& c : regex.getRegex())
	{
		if (!Regex::IsOperator(c))
		{
			addSymbol(c);
		} else
		{
			switch (Regex::GetOperator(c))
			{
			case Regex::Symbol::Star:	addClosure(); break;
			case Regex::Symbol::Dot:	addConcatenation(); break;
			case Regex::Symbol::Or:		addUnion(); break;
			case Regex::Symbol::None:	
			default: std::cerr << "Operator not supported!" << std::endl;
			}
		}
	}
}

AutomataBuilder& AutomataBuilder::addSymbol(char symbol)
{
	m_symbols.insert(symbol);

	auto from = std::make_shared<State>(std::to_string(m_stateCount++));
	auto to = std::make_shared<State>(std::to_string(m_stateCount++));
	
	auto transition = std::make_shared<Transition>(from, symbol, to);
	from->transitions.push_back(transition);

	Automata a;
	a.setStartState(from);
	a.setFinalState(to);
	a.addSymbol(symbol);
	m_automatas.push(a);

	return *this;
}

AutomataBuilder& AutomataBuilder::addUnion()
{	
	Automata top = m_automatas.top();
	m_automatas.pop();

	Automata second = m_automatas.top();
	m_automatas.pop();

	auto start = std::make_shared<State>(std::to_string(m_stateCount++));
	auto tStart1 = std::make_shared<Transition>(start, top.getStartState());
	auto tStart2 = std::make_shared<Transition>(start, second.getStartState());
	start->transitions.push_back(tStart1);
	start->transitions.push_back(tStart2);
	
	auto end = std::make_shared<State>(std::to_string(m_stateCount++));
	auto tEnd1 = std::make_shared<Transition>(top.getFinalState(), end);
	auto tEnd2 = std::make_shared<Transition>(second.getFinalState(), end);
	top.getFinalState()->transitions.push_back(tEnd1);
	second.getFinalState()->transitions.push_back(tEnd2);
	
	Automata a;
	a.setStartState(start);
	a.addStates(top.getStates());
	a.addStates(second.getStates());
	a.setFinalState(end);
	a.addSymbols(top.getAlfabet());
	a.addSymbols(second.getAlfabet());
	m_automatas.push(a);
	
	return *this;
}

AutomataBuilder& AutomataBuilder::addConcatenation()
{
	Automata top = m_automatas.top();
	m_automatas.pop();

	Automata second = m_automatas.top();
	m_automatas.pop();

	for (auto& state : second.getStates())
	{
		if (state == second.getFinalState())
		{
			continue;
		}

		for (auto& transition : state->transitions)
		{
			if (transition->to == second.getFinalState())
			{
				transition->to = top.getStartState();
			}
		}
	}

	const auto& it = std::find(second.getStates().begin(), second.getStates().end(), second.getFinalState());
	second.getStates().erase(it);
	
	Automata a;
	a.setStartState(second.getStartState());
	a.addStates(top.getStates());
	a.addStates(second.getStates());
	a.setFinalState(top.getFinalState());
	a.addSymbols(top.getAlfabet());
	a.addSymbols(second.getAlfabet());
	m_automatas.push(a);
	
	return *this;
}

AutomataBuilder& AutomataBuilder::addClosure()
{
	Automata top = m_automatas.top();
	m_automatas.pop();

	auto end = std::make_shared<State>(std::to_string(m_stateCount++));
	auto tEnd1 = std::make_shared<Transition>(top.getFinalState(), end);
	top.getFinalState()->transitions.push_back(tEnd1);
	
	auto tRepeat = std::make_shared<Transition>(top.getFinalState(), top.getStartState());
	top.getFinalState()->transitions.push_back(tRepeat);
	
	auto start = std::make_shared<State>(std::to_string(m_stateCount++));
	auto tStart1 = std::make_shared<Transition>(start, top.getStartState());
	auto tStart2 = std::make_shared<Transition>(start, end);
	start->transitions.push_back(tStart1);
	start->transitions.push_back(tStart2);

	Automata a;
	a.setStartState(start);
	a.addStates(top.getStates());
	a.setFinalState(end);
	a.addSymbols(top.getAlfabet());
	m_automatas.push(a);
	
	return *this;
}

Automata AutomataBuilder::construct()
{
	if (m_automatas.size() > 1 || m_automatas.empty())
	{
		std::cerr << "Could not construct an Automata" << std::endl;
		return Automata();
	}
	
	return m_automatas.top();
}