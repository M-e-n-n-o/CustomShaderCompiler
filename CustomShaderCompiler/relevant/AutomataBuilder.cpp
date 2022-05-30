#include "AutomataBuilder.h"
#include <memory>
#include "State.h"

AutomataBuilder::AutomataBuilder(Regex& regex)
{
	for (auto& c : regex.getRegex(true))
	{
		if (!Regex::IsOperator(c))
		{
			addSymbol(c);
		}
		else
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