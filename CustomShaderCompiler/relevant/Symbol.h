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

class Letter : public Symbol
{
private:
	std::string m_letter;
	
public:
	Letter(char letter): m_letter(std::string(1, letter)) {}

	bool validate(const std::string& s) const override
	{
		return m_letter == s;
	}
	
	bool operator==(const Symbol& other) const override
	{		
		if (auto o = dynamic_cast<const Letter*>(&other); o != nullptr)
		{
			return m_letter == o->m_letter;
		}
		
		return false;
	}

	bool operator<(const Symbol& other) const override
	{
		if (auto o = dynamic_cast<const Letter*>(&other); o != nullptr)
		{
			return m_letter < o->m_letter;
		}

		return false;
	}

	std::string toString() const override
	{
		return m_letter;
	}
};