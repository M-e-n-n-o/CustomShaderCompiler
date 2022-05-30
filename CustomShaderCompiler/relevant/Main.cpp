#include <iostream>
#include "Automata.h"
#include "Regex.h"

int main()
{
	// Automata m = AutomataBuilder()
	// 	.addSymbol('a')
	// 	.addSymbol('b')
	// 	.addConcatenation()
	// 	.construct();

	// Automata m = AutomataBuilder()
	// 	.addSymbol('a')
	// 	.addClosure()
	// 	.construct();

	// Automata m = AutomataBuilder()
	// 	.addSymbol('a')
	// 	.addSymbol('b')
	// 	.addUnion()
	// 	.construct();
	
	// m.printTransitions();

	Regex regex("a.(a|b)*.b");
	// Regex regex("b.a*");
	// std::cout << regex.getRegex() << std::endl;
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