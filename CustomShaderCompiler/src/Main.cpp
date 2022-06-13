#include <iostream>
#include "shaderCompiler/automata/Automata.h"
#include "shaderCompiler/automata/Regex.h"

int main()
{	
	Regex regex("'appel'.(a|b)*.b");
	
	Automata m = regex.constructAutomata();
	m.makeDeterministic();
	m.printTransitions();

	std::vector<std::string> input = { "appel", "b", "b" };
	bool isValid = m.validate(input);
	std::cout << isValid << std::endl;
	
	// bool isValid = m.validate("ab");
	// std::cout << isValid << std::endl;
	//
	// isValid = m.validate("aa");
	// std::cout << isValid << std::endl;
	//
	// isValid = m.validate("aaab");
	// std::cout << isValid << std::endl;
	//
	// isValid = m.validate("abbaab");
	// std::cout << isValid << std::endl;
}


/*
 *
 * Split the input with enters
 * 
 */