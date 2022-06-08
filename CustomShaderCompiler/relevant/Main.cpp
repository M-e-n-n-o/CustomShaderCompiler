#include <iostream>
#include "Automata.h"
#include "Regex.h"
#include "Symbol.h"

int main()
{
	// Letter s1('a');
	// Letter s2('a');
	//
	// std::cout << (s1 == s2);
	
	Regex regex("a.(a|b)*.b");
	
	Automata m = regex.constructAutomata();
	m.makeDeterministic();
	m.printTransitions();
	
	bool isValid = m.validate("ab");
	std::cout << isValid << std::endl;
	
	isValid = m.validate("aa");
	std::cout << isValid << std::endl;
	
	isValid = m.validate("aaab");
	std::cout << isValid << std::endl;
	
	isValid = m.validate("abbaab");
	std::cout << isValid << std::endl;
}