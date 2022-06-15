#pragma once
#include "ShaderConverter.h"

class ShaderClass : public ShaderConverter
{
private:
	bool m_insideClass = false;
	
public:
	ShaderClass() = default;
	
	void convertLanguage(const std::string& input, std::shared_ptr<ShaderOutputLanguage>& output) override
	{
		output->addGlobalVariable({ "test1", VariableType::VertexInput, VariableDataType::Float });
		output->addGlobalVariable({ "test2", VariableType::VertexOutput, VariableDataType::Float });

		output->addShaderSource({ ShaderSourceType::Vertex, "uv = texCoord;\ngl_Position = _projectionMatrix * _viewMatrix * _modelMatrix * vec4(vertexPos, 1.0);" });
		
		output->addGlobalVariable({ "test3", VariableType::FragmentInput, VariableDataType::Float });
		output->addGlobalVariable({ "test4", VariableType::FragmentInput, VariableDataType::Float });

		output->addGlobalVariable({ "test5", VariableType::Uniform, VariableDataType::Texture2D });
	}
};