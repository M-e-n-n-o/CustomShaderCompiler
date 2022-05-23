#include "Automata.h"

Automata::Automata(const std::set<std::shared_ptr<State>>& states, const std::shared_ptr<State>& startState,
	const std::shared_ptr<State>& finalState, const std::set<char>& symbols) : m_states(m_states),
	m_startState(startState),
	m_finalState(finalState),
	m_symbols(m_symbols) {}

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
	m_symbols.insert(symbol);
}

void Automata::addSymbols(std::set<char>& symbols)
{
	m_symbols.insert(symbols.begin(), symbols.end());
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

	// std::cout << "Current state: " + start << ", input: " << input << std::endl;

	bool isValid = false;
	bool didTransition = false;
	for (auto& transition : start->transitions)
	{
		// std::cout << "Possible transition with: " << t.getSymbol() << ", to: " << t.getToState() << std::endl;

		if (transition->symbol == input[0])
		{
			// std::cout << "Transition!" << std::endl;

			if (didTransition)
			{
				std::cout << "Can only check validation of deterministic automata!" << std::endl;
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

void Automata::printTransitions()
{
	for (auto& state : m_states)
	{
		std::cout << state->toString() << std::endl;
	}
}


// --------------------------------------------------------------------------
//									BUILDER
// --------------------------------------------------------------------------

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
	Automata& top = m_automatas.top();
	m_automatas.pop();

	Automata& second = m_automatas.top();
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
	a.addSymbols(top.getSymbols());
	a.addSymbols(second.getSymbols());
	m_automatas.push(a);
	
	return *this;
}

AutomataBuilder& AutomataBuilder::addConcatenation()
{	
	return *this;
}

AutomataBuilder& AutomataBuilder::addClosure()
{	
	return *this;
}

Automata AutomataBuilder::construct()
{
	return Automata();
}

// TODO Deze methodes afmaken + een methode maken om de NFA om te zetten naar een DFA
// https://medium.com/swlh/visualizing-thompsons-construction-algorithm-for-nfas-step-by-step-f92ef378581b