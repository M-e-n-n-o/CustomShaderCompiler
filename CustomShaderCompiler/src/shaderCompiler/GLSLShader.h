#pragma once
#include <string>
#include "ShaderLanguage.h"

class GLSLShader : public ShaderOutputLanguage
{
private:
	int m_vertexInputCount = 0;
	int m_vertexVariableIndex = 1;
	int m_vertexSourceIndex = 2;
	std::vector<std::string> m_vertexOutput = { "#version 450 core", "void main() {", "}" };
	
	int m_fragmentVariableIndex = 1;
	int m_fragmentSourceIndex = 2;
	std::vector<std::string> m_fragmentOutput = { "#version 450 core", "void main() {", "}" };

public:
	GLSLShader() = default;

	void addGlobalVariable(const GlobalShaderVariable& variable) override
	{
		std::string typeName;
		switch (variable.dataType)
		{
			case VariableDataType::Float: typeName = ("float " + variable.name + ";"); break;
			case VariableDataType::Texture2D: typeName = ("sampler2D " + variable.name + ";"); break;

			default: std::cout << "VariableDataType is not supported yet!" << std::endl;
		}
		
		switch (variable.type)
		{
			case VariableType::VertexInput:
			{
				if (variable.dataType == VariableDataType::Texture2D)
				{
					std::cout << "A texture2D is not a valid input variable for a vertex shader! Ignoring..." << std::endl;
					break;
				}
					
				m_vertexOutput.insert(m_vertexOutput.begin() + m_vertexVariableIndex++, "layout (location = " + std::to_string(m_vertexInputCount++) + ") in " + typeName);
				m_vertexSourceIndex++;
				break;
			}
				
			case VariableType::VertexOutput:
			{
				if (variable.dataType == VariableDataType::Texture2D)
				{
					std::cout << "A texture2D is not a valid output variable for a vertex shader! Ignoring..." << std::endl;
					break;
				}

				m_vertexOutput.insert(m_vertexOutput.begin() + m_vertexVariableIndex++, "out " + typeName);
				m_vertexSourceIndex++;
				break;	
			}
			
			case VariableType::Uniform:
			{
				m_vertexOutput.insert(m_vertexOutput.begin() + m_vertexVariableIndex++, "uniform " + typeName);
				m_vertexSourceIndex++;
				m_fragmentOutput.insert(m_fragmentOutput.begin() + m_fragmentVariableIndex++, "uniform " + typeName);
				m_fragmentSourceIndex++;
				break;
			}
				
			case VariableType::FragmentInput:
			{
				if (variable.dataType == VariableDataType::Texture2D)
				{
					std::cout << "A texture2D is not a valid input variable for a fragment shader! Ignoring..." << std::endl;
					break;
				}
					
				m_fragmentOutput.insert(m_fragmentOutput.begin() + m_fragmentVariableIndex++, "in " + typeName);
				m_fragmentSourceIndex++;
				break;
			}
				
			case VariableType::FragmentOutput:
			{
				m_fragmentOutput.insert(m_fragmentOutput.begin() + m_fragmentVariableIndex++, "out " + typeName);
				m_fragmentSourceIndex++;
				break;
			}

			default:
			{
				std::cout << "VariableType is not supported yet!" << std::endl;
			}
		}
	}

	void addShaderSource(const ShaderSource& source) override
	{
		switch (source.type)
		{
			case ShaderSourceType::Vertex:
			{
				m_vertexOutput.insert(m_vertexOutput.begin() + m_vertexSourceIndex++, source.code);
				break;
			}
			
			case ShaderSourceType::Fragment:
			{
				m_fragmentOutput.insert(m_fragmentOutput.begin() + m_fragmentSourceIndex++, source.code);
				break;
			}

			default:
			{
				std::cout << "ShaderSourceType is not supported yet!" << std::endl;
			}
		}
	}
	
	std::vector<ShaderSource> constructOutput() override
	{
		std::vector<ShaderSource> sources;

		ShaderSource vertex;
		vertex.type = ShaderSourceType::Vertex;
		for (auto& s : m_vertexOutput)
		{
			vertex.code += s + "\n";
		}
		sources.push_back(vertex);

		ShaderSource fragment;
		fragment.type = ShaderSourceType::Fragment;
		for (auto& s : m_fragmentOutput)
		{
			fragment.code += s + "\n";
		}
		sources.push_back(fragment);
		
		return sources;
	}
};