#pragma once
#include <memory>
#include <vector>
#include <string>
#include "ShaderOutputLanguage.h"
#include "ShaderConverter.h"

class ShaderLanguage
{
private:
	std::shared_ptr<ShaderOutputLanguage> m_output;
	std::vector<std::shared_ptr<ShaderConverter>> m_converters;

public:
	ShaderLanguage(const std::shared_ptr<ShaderOutputLanguage>& output): m_output(output) {}

	void addConverter(const std::shared_ptr<ShaderConverter>& converter);

	std::shared_ptr<ShaderOutputLanguage> convert(const std::string& input);
};