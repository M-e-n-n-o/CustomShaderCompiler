#pragma once
#include <string>

class Symbol
{
public:
	virtual ~Symbol() = default;

	bool validate(char s) const { return validate(std::string(1, s)); }
	virtual bool validate(const std::string& s) const = 0;

	virtual bool operator==(const Symbol& other) const = 0;
	friend bool operator==(const std::shared_ptr<Symbol>& l, const std::shared_ptr<Symbol>& r)
	{
		return (*l) == (*r);
	}

	virtual bool operator<(const Symbol& other) const = 0;
	friend bool operator<(const std::shared_ptr<Symbol>& l, const std::shared_ptr<Symbol>& r)
	{
		return (*l) < (*r);
	}

	virtual std::string toString() const = 0;
};

// -----------------------------------------------------------------------------------------------------------
//												DERIVED CLASSES
// -----------------------------------------------------------------------------------------------------------

class Word : public Symbol
{
private:
	std::string m_word;
	
public:
	Word(const std::string& word): m_word(word) {}
	Word(char letter) : m_word(std::string(1, letter)) {}

	bool validate(const std::string& s) const override
	{
		return m_word == s;
	}
	
	bool operator==(const Symbol& other) const override
	{		
		if (auto o = dynamic_cast<const Word*>(&other); o != nullptr)
		{
			return m_word == o->m_word;
		}
		
		return false;
	}

	bool operator<(const Symbol& other) const override
	{
		if (auto o = dynamic_cast<const Word*>(&other); o != nullptr)
		{
			return m_word < o->m_word;
		}

		return false;
	}

	std::string toString() const override
	{
		return m_word;
	}
};

class Epsilon : public Symbol
{
public:
#define EPSILON "$"
	
	Epsilon() = default;

	bool validate(const std::string& s) const override
	{
		return EPSILON == s;
	}

	bool operator==(const Symbol& other) const override
	{
		if (auto o = dynamic_cast<const Epsilon*>(&other); o != nullptr)
		{
			return true;
		}

		return false;
	}

	bool operator<(const Symbol& other) const override
	{
		if (auto o = dynamic_cast<const Epsilon*>(&other); o != nullptr)
		{
			return true;
		}

		return false;
	}

	std::string toString() const override
	{
		return EPSILON;
	}
};