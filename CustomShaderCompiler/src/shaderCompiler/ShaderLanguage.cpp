#include "ShaderLanguage.h"
#include <iostream>

void ShaderLanguage::addConverter(const std::shared_ptr<ShaderConverter>& converter)
{
	m_converters.push_back(converter);
}

std::shared_ptr<ShaderOutputLanguage> ShaderLanguage::convert(const std::string& input)
{
	for (auto& converter : m_converters)
	{
		converter->convertLanguage(input, m_output);
	}

	return m_output;
}
