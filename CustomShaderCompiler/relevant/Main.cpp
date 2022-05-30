#include <iostream>
#include "Automata.h"
#include "Regex.h"

int main()
{
	Regex regex("a.(a|b)*.b");
	
	Automata m = regex.constructAutomata();
	m.makeDeterministic();
	m.printTransitions();
	
	bool isValid = m.validate("ab");
	std::cout << isValid << std::endl;

	isValid = m.validate("a");
	std::cout << isValid << std::endl;
	
	isValid = m.validate("b");
	std::cout << isValid << std::endl;
	
	isValid = m.validate("bb");
	std::cout << isValid << std::endl;
}