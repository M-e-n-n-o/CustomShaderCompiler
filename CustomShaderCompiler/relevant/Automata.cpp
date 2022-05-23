#include "Automata.h"

bool Automata::validate(const std::string& start, const std::string& input)
{
	if (std::find(m_finalStates.begin(), m_finalStates.end(), start) != m_finalStates.end() && input.empty())
	{
		return true;
	}

	// std::cout << "Current state: " + start << ", input: " << input << std::endl;

	bool isValid = false;
	bool didTransition = false;
	for (const auto& t : m_transitions)
	{
		if (t.from != start)
		{
			continue;
		}

		// std::cout << "Possible transition with: " << t.getSymbol() << ", to: " << t.getToState() << std::endl;

		if (t.symbol == input[0])
		{
			// std::cout << "Transition!" << std::endl;

			if (didTransition)
			{
				std::cout << "Can only check validation of deterministic automata!" << std::endl;
				return false;
			}

			isValid = validate(t.to, input.substr(1));
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
	for (const Transition& t : m_transitions)
	{
		std::cout << t.toString() << std::endl;
	}
}


// --------------------------------------------------------------------------
//									BUILDER
// --------------------------------------------------------------------------

Automata::Builder& Automata::Builder::addSymbol(char a)
{
	m_symbols.insert(a);

	Transition t = { std::to_string(stateCounter++), a, std::to_string(stateCounter++) };
	m_transitionStack.push(t);
	m_transitions.insert(t);
}

Automata::Builder& Automata::Builder::addUnion()
{
}

Automata::Builder& Automata::Builder::addConcatenation()
{
}

Automata::Builder& Automata::Builder::addClosure()
{
}

Automata Automata::Builder::construct()
{
}
