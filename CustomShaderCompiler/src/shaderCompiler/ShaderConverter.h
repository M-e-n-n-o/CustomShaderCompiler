#pragma once
#include <string>
#include <memory>
#include "ShaderOutputLanguage.h"

class ShaderConverter
{
public:
	virtual ~ShaderConverter() = default;

	virtual std::shared_ptr<ShaderOutputLanguage> convertLanguage(const std::string& input) = 0;
};