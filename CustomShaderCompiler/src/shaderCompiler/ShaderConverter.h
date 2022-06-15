#pragma once
#include <string>
#include <memory>
#include <vector>
#include "ShaderOutputLanguage.h"

/*
 * TODO This code segment has to be rethinked to make it easier to convert the input to the output (maybe add a linker somewhere?)
 */

class ShaderConverter
{
public:
	ShaderConverter() = default;
	virtual ~ShaderConverter() = default;

	virtual void convertLanguage(const std::string& input, std::shared_ptr<ShaderOutputLanguage>& output) = 0;

protected:
	static std::vector<std::string> stringDelimiter(const std::string& input, const std::string& delimiter)
	{
		std::string in = input;
		int pos = 0;
		std::string token;
		std::vector<std::string> lines;
		while ((pos = in.find(delimiter)) != std::string::npos)
		{
			token = in.substr(0, pos);
			in.erase(0, pos + delimiter.length());

			lines.push_back(token);
		}
		lines.push_back(in);

		return lines;
	}
	
	static std::string& leftTrim(std::string& str, const std::string& chars)
	{
		str.erase(0, str.find_first_not_of(chars));
		return str;
	}

	static std::string& rightTrim(std::string& str, const std::string& chars)
	{
		str.erase(str.find_last_not_of(chars) + 1);
		return str;
	}

	static std::string& trimString(std::string& str, const std::string& chars)
	{
		return leftTrim(rightTrim(str, chars), chars);
	}

	static std::string& removeFromString(std::string& str, const std::string& chars)
	{
		for (char c : chars)
		{
			str.erase(std::remove(str.begin(), str.end(), c), str.end());
		}
		
		return str;
	}
};