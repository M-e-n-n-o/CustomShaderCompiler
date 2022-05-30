#include <iostream>
#include "AutomataBuilder.h"
#include "Automata.h"
#include "Regex.h"

int main()
{
	Regex regex("a.(a|b)*.b");
	Automata m = AutomataBuilder(regex).construct();

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