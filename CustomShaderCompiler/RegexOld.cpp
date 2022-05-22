#include "RegexOld.h"
#include <iostream>

RegexOld::RegexOld()
{
	m_operator = Operator::One;
	m_terminals = "";
	m_left = nullptr;
	m_right = nullptr;
}

RegexOld::RegexOld(const std::string& expression)
{
	m_operator = Operator::One;
	m_terminals = expression;
	m_left = nullptr;
	m_right = nullptr;
}

RegexOld* RegexOld::plus()
{
	auto result = new RegexOld();
	result->m_operator = Operator::Plus;
	result->m_left = this;
	
	return result;
}

RegexOld* RegexOld::star()
{
	auto result = new RegexOld();
	result->m_operator = Operator::Star;
	result->m_left = this;
	return result;
}

RegexOld* RegexOld::orr(RegexOld* other)
{
	auto result = new RegexOld();
	result->m_operator = Operator::Or;
	result->m_left = this;
	result->m_right = other;
	return result;
}

RegexOld* RegexOld::dot(RegexOld* other)
{
	auto result = new RegexOld();
	result->m_operator = Operator::Dot;
	result->m_left = this;
	result->m_right = other;
	return result;
}

bool RegexOld::isValid(const std::string& sequence, int maxSteps)
{
	auto language = getLanguage(maxSteps);
	return std::find(language.begin(), language.end(), sequence) != language.end();
}

std::vector<std::string> RegexOld::getLanguage(int maxSteps)
{
	std::vector<std::string> emptyLanguage;
	std::vector<std::string> languageResult;
	
	std::vector<std::string> languageLeft;
	std::vector<std::string> languageRight;

	if (maxSteps < 1)
	{
		return emptyLanguage;
	}

	switch (m_operator)
	{
	case Operator::One: 
		languageResult.push_back(m_terminals);
		break;
		
	case Operator::Or:
		languageLeft = m_left == nullptr ? emptyLanguage : m_left->getLanguage(maxSteps - 1);
		languageRight = m_right == nullptr ? emptyLanguage : m_right->getLanguage(maxSteps - 1);
		languageResult.insert(languageResult.end(), languageLeft.begin(), languageLeft.end());
		languageResult.insert(languageResult.end(), languageRight.begin(), languageRight.end());
		
		break;
		
	case Operator::Dot:
		languageLeft = m_left == nullptr ? emptyLanguage : m_left->getLanguage(maxSteps - 1);
		languageRight = m_right == nullptr ? emptyLanguage : m_right->getLanguage(maxSteps - 1);
		
		for (const std::string& s1 : languageLeft)
		{
			for (const std::string& s2 : languageRight)
			{
				languageResult.push_back(s1 + s2);
			}
		}
		
		break;
		
	case Operator::Star:
	case Operator::Plus:
		languageLeft = m_left == nullptr ? emptyLanguage : m_left->getLanguage(maxSteps - 1);
		languageResult.insert(languageResult.end(), languageLeft.begin(), languageLeft.end());
		
		for (int i = 0; i < maxSteps; i++)
		{
			std::vector<std::string> languageTemp = std::vector<std::string>(languageResult);
			
			for (const std::string& s1 : languageLeft)
			{
				for (const std::string& s2 : languageTemp)
				{
					languageResult.push_back(s1 + s2);
				}
			}
		}

		if (m_operator == Operator::Star)
		{
			languageResult.push_back("");
		}
		
		break;
		
	default:
		std::cout << "getLanguage is not defined yet for this operator!" << std::endl;
		break;
	}

	return languageResult;
}
