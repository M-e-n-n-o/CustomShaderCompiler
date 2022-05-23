#include "../RegexToNFA.h"
#include "Automata.h"
#include "Regex.h"

int main()
{
	// regexToNFATest();

	Regex regex("a.(a|b)*.b");

	Automata m(regex);
	m.printTransitions();
	
	// Automata<std::string> m;
	// m.setAlphabet({'a', 'b'});
	//
	// m.addTransition({"q0", 'a', "q1"});	
	// m.addTransition({"q0", 'b', "q4"});
	// m.addTransition({"q1", 'a', "q4"});
	// m.addTransition({"q1", 'b', "q2"});
	// m.addTransition({"q2", 'a', "q3"});
	// m.addTransition({"q2", 'b', "q4"});
	// m.addTransition({"q3", 'a', "q1"});
	// m.addTransition({"q3", 'b', "q2"});
	//
	// m.addTransition({ "q4", 'a' });
	// m.addTransition({ "q4", 'b' });
	//
	// m.addStartState("q0");
	//
	// m.addFinalState("q2");
	// m.addFinalState("q3");
	//
	// // m.printTransitions();
	//
	// bool isValid = m.validate("q0", "a");
	// std::cout << isValid << std::endl;
}