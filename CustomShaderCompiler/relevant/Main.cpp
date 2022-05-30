#include <iostream>
#include "Automata.h"
#include "Regex.h"

int main()
{
	Regex regex("a.(a|b)*.b");

	Automata m = regex.constructAutomata();
	m.makeDeterministic();
	m.printTransitions();
	
	bool isValid = m.validate("abab");
	std::cout << isValid << std::endl;

	isValid = m.validate("aab");
	std::cout << isValid << std::endl;
	
	isValid = m.validate("aaab");
	std::cout << isValid << std::endl;
	
	isValid = m.validate("aba");
	std::cout << isValid << std::endl;
}