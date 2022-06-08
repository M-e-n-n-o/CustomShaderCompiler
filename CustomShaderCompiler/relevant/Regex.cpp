#include "Regex.h"
#include <iostream>
#include <stack>
#include <memory>
#include "State.h"

Regex::Regex(const std::string& regex)
{
	m_regex = regex;
}

const std::string& Regex::getRegex(bool postfixNotation)
{
	if (postfixNotation)
	{
		if (m_postfix.empty())
		{
			m_postfix = postFix(m_regex);
		}
		
		return m_postfix;
	}

	return m_regex;
}

Automata Regex::constructAutomata()
{
	return AutomataBuilder(getRegex(true)).construct();
}

bool Regex::IsOperator(char c)
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

Regex::Operator Regex::GetOperator(char c)
{
	switch (c)
	{
	case STAR:	return Operator::Star;
	case DOT:	return Operator::Dot;
	case OR:	return Operator::Or;
	default:	return Operator::None;
	}
}

std::string Regex::postFix(const std::string& regex)
{
	std::string output;
	std::stack<char> operatorStack;

	for (const char& c : regex)
	{		
		if (!IsOperator(c))
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
			int o = (int) GetOperator(c);
			while (!operatorStack.empty())
			{
				char top = operatorStack.top();

				if (top == OPEN)
				{
					break;
				}

				if ((int) GetOperator(top) < o)
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


// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
//										Automata Builder
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------


Regex::AutomataBuilder::AutomataBuilder(const std::string& postfix)
{	
	for (int i = 0; i < postfix.size(); i++)
	{
		const char& c = postfix[i];
		
		if (!Regex::IsOperator(c))
		{
			if (c != WORD_SPLIT)
			{
				addSymbol(std::string(1, c));
				continue;
			}
			
			i++;
			std::string symbol;
			while (postfix[i] != WORD_SPLIT)
			{
				symbol += postfix[i];
				i++;
			}

			addSymbol(symbol);			
		}
		else
		{			
			switch (Regex::GetOperator(c))
			{
			case Regex::Operator::Star:	addClosure(); break;
			case Regex::Operator::Dot:	addConcatenation(); break;
			case Regex::Operator::Or:	addUnion(); break;
			case Regex::Operator::None:
			default: std::cerr << "Operator not supported!" << std::endl;
			}
		}
	}
}

Regex::AutomataBuilder& Regex::AutomataBuilder::addSymbol(const std::string& symbol)
{	
	auto letter = std::make_shared<Word>(symbol);
	auto pair = m_symbols.insert(letter);
	
	auto from = std::make_shared<State>(std::to_string(m_stateCount++));
	auto to = std::make_shared<State>(std::to_string(m_stateCount++));

	auto transition = std::make_shared<Transition>(from, letter, to);
	from->transitions.push_back(transition);

	Automata a;
	a.setStartState(from);
	a.addFinalState(to);
	a.addSymbol(letter);
	m_automatas.push(a);

	return *this;
}

Regex::AutomataBuilder& Regex::AutomataBuilder::addUnion()
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
	a.addSymbols(top.getLanguage());
	a.addSymbols(second.getLanguage());
	m_automatas.push(a);

	return *this;
}

Regex::AutomataBuilder& Regex::AutomataBuilder::addConcatenation()
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
	a.addSymbols(top.getLanguage());
	a.addSymbols(second.getLanguage());
	m_automatas.push(a);

	return *this;
}

Regex::AutomataBuilder& Regex::AutomataBuilder::addClosure()
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
	a.addSymbols(top.getLanguage());
	m_automatas.push(a);

	return *this;
}

Automata Regex::AutomataBuilder::construct()
{
	if (m_automatas.size() > 1 || m_automatas.empty())
	{
		std::cerr << "Could not construct an Automata" << std::endl;
		return Automata();
	}

	return m_automatas.top();
}