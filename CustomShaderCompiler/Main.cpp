#include "Regex.h"
#include <iostream>

void printLanguage(Regex* regex)
{
	std::cout << "Taal: " << std::endl;
	auto l = regex->getLanguage(5);
	for (std::string& s : l)
	{
		std::cout << s << ", ";
	}
}

void checkValidation(Regex* regex, const std::string& sequence)
{
	bool valid = regex->isValid(sequence, 5);
	std::cout << valid << std::endl;
}

int main()
{
	auto a = new Regex("a");
	auto b = new Regex("b");

	// "baa"
	auto expr1 = new Regex("baa");
	// "bb"
	auto expr2 = new Regex("bb");
	// "baa | bb"
	auto expr3 = expr1->orr(expr2);	

	// "(a | b)*"
	auto all = (a->orr(b))->star();

	// "(baa | bb)+"
	auto expr4 = expr3->plus();
	// "(baa | bb)+ (a | b)*"
	auto expr5 = expr4->dot(all);

	checkValidation(expr4, "bbbaa");
	
	delete a;
	delete b;
	delete expr1;
	delete expr2;
	delete expr3;
	delete all;
	delete expr4;
	delete expr5;
}