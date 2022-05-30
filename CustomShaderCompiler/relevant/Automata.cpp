#include "Automata.h"
#include <iostream>

Automata::Automata(const std::set<std::shared_ptr<State>>& states, const std::shared_ptr<State>& startState,
	const std::shared_ptr<State>& finalState, const std::set<char>& alfabet) : m_states(states),
	m_startState(startState),
	m_alfabet(alfabet)
{
	m_finalStates.insert(finalState);
}

Automata::Automata(const std::set<std::shared_ptr<State>>& states, const std::shared_ptr<State>& startState,
	const std::set<std::shared_ptr<State>>& finalStates, const std::set<char>& alfabet) : m_states(states),
	m_startState(startState),
	m_finalStates(finalStates),
	m_alfabet(alfabet) {}

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
	if (input.empty())
	{		
		for (auto& finalState : m_finalStates)
		{			
			if (start == finalState)
			{
				return true;
			}
		}
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

		// std::cout << "Current states: " << std::endl;
		// for (auto& state : newStates)
		// 	std::cout << state->name << std::endl;
		
		for (auto& state : toAdd)
		{
			// std::cout << "Adding a new state: " << state->name << std::endl;
			
			auto pair = newStates.insert(state);
			if (!pair.second)
			{
				// std::cout << "State already exists!" << std::endl;
				
				auto& transitions = (*pair.first)->transitions;
				for (auto& transitionNew : state->transitions)
				{
					// if (transitionNew->to == state)
					// {
					// 	transitionNew->to->printRandom();
					// 	std::cout << transitionNew->to->transitions.size();
					// 	transitionNew->to->transitions[0]->to->transitions[0]->to->printRandom();
					// }
					
					
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
						// std::cout << "Detected new transition for char: " << transitionNew->symbol << std::endl;

						transitionNew->to = findState(transitionNew->to->name, newStates);
						
						transitions.push_back(transitionNew);

						// auto t = std::make_shared<Transition>((*pair.first), transitionNew->symbol, findState(transitionNew->to->name, newStates));
						// transitions.push_back(t);
						
					}
				}
			}
		}
		
		if (done)
		{
			break;
		}
	}

	m_states.clear();
	m_states = newStates;

	std::set<std::shared_ptr<State>> finalStates;
	for (auto& state : newStates)
	{
		if (state->name == m_startState->name)
		{
			// std::cout << state->name << " is the start state" << std::endl;
			m_startState = state;
		}

		if (state->name.find((*m_finalStates.begin())->name) != std::string::npos)
		{
			// std::cout << state->name << " is an end state" << std::endl;
			finalStates.insert(state);
		}
	}

	m_finalStates = finalStates;
}

std::set<std::shared_ptr<State>> Automata::makeDeterministic(const std::shared_ptr<State>& start, const std::shared_ptr<State>& error)
{	
	std::set<std::shared_ptr<State>> startStates;

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
	
	for (auto& transition : start->transitions)
	{
		if (transition->symbol == EPSILON)
		{			
			startStates.insert(transition->to);
		}
	}

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

	// std::cout << "Start state: " << stateName << std::endl;
	
	std::vector<std::pair<char, std::set<std::shared_ptr<State>>>> possibleNextStates;
	for (auto& symbol : m_alfabet)
	{		
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
	
	for (auto& statesPerSymbol : possibleNextStates)
	{
		if (statesPerSymbol.second.empty())
		{
			// std::cout << "Created error transition to char: " << statesPerSymbol.first << std::endl;
			auto transition = std::make_shared<Transition>(startState, statesPerSymbol.first, error);
			startState->transitions.push_back(transition);
			continue;
		}

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
		
		auto nextState = std::make_shared<State>(stateName);
		auto it = states.insert(nextState);
		
		if (!it.second)
		{
			// std::cout << "Created transition to itself for char: " << statesPerSymbol.first << std::endl;
			auto transition = std::make_shared<Transition>(startState, statesPerSymbol.first);
			startState->transitions.push_back(transition);

			// std::cout << startState->toString() << std::endl;
			// startState->transitions[0]->to->printRandom();
			// std::cout << startState->transitions[0]->to->transitions.size();
			// startState->transitions[0]->to->transitions[0]->to->printRandom();
			
		} else
		{
			// std::cout << "Created next state: " << stateName << " for char: " << statesPerSymbol.first << std::endl;
			auto transition = std::make_shared<Transition>(startState, statesPerSymbol.first, nextState);
			startState->transitions.push_back(transition);
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
	a.addFinalState(to);
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

	auto& topFinalState = *top.getFinalStates().begin();
	auto& secondFinalState = *second.getFinalStates().begin();
	
	auto end = std::make_shared<State>(std::to_string(m_stateCount++));
	auto tEnd1 = std::make_shared<Transition>(topFinalState, end);
	auto tEnd2 = std::make_shared<Transition>(secondFinalState, end);
	topFinalState->transitions.push_back(tEnd1);
	secondFinalState->transitions.push_back(tEnd2);
	
	Automata a;
	a.setStartState(start);
	a.addStates(top.getStates());
	a.addStates(second.getStates());
	a.addFinalState(end);
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

	auto topFinalState = *top.getFinalStates().begin();
	auto& secondFinalState = *second.getFinalStates().begin();
	
	for (auto& state : second.getStates())
	{
		if (state == secondFinalState)
		{
			continue;
		}

		for (auto& transition : state->transitions)
		{
			if (transition->to == secondFinalState)
			{
				transition->to = top.getStartState();
			}
		}
	}

	const auto& it = std::find(second.getStates().begin(), second.getStates().end(), secondFinalState);
	second.getStates().erase(it);
	
	Automata a;
	a.setStartState(second.getStartState());
	a.addStates(top.getStates());
	a.addStates(second.getStates());
	a.addFinalState(topFinalState);
	a.addSymbols(top.getAlfabet());
	a.addSymbols(second.getAlfabet());
	m_automatas.push(a);
	
	return *this;
}

AutomataBuilder& AutomataBuilder::addClosure()
{
	Automata top = m_automatas.top();
	m_automatas.pop();

	auto& topFinalState = *top.getFinalStates().begin();
	
	auto end = std::make_shared<State>(std::to_string(m_stateCount++));
	auto tEnd1 = std::make_shared<Transition>(topFinalState, end);
	topFinalState->transitions.push_back(tEnd1);
	
	auto tRepeat = std::make_shared<Transition>(topFinalState, top.getStartState());
	topFinalState->transitions.push_back(tRepeat);
	
	auto start = std::make_shared<State>(std::to_string(m_stateCount++));
	auto tStart1 = std::make_shared<Transition>(start, top.getStartState());
	auto tStart2 = std::make_shared<Transition>(start, end);
	start->transitions.push_back(tStart1);
	start->transitions.push_back(tStart2);

	Automata a;
	a.setStartState(start);
	a.addStates(top.getStates());
	a.addFinalState(end);
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