#pragma once
#include <vector>
#include <string>

// ------------------------------------------------
//					ShaderSource
// ------------------------------------------------
enum class ShaderSourceType
{
	Vertex,
	Fragment,

	Output
};

struct ShaderSource
{
	std::string code;
	ShaderSourceType type;
};


// ------------------------------------------------
//				GlobalShaderVariable
// ------------------------------------------------
enum class VariableType
{
	VertexInput,
	VertexOutput,
	
	Uniform,

	FragmentInput,
	FragmentOutput
};

enum class VariableDataType
{
	Float,
	Texture2D
};

struct GlobalShaderVariable
{
	VariableType type;
	VariableDataType dataType;
};


// ------------------------------------------------
//				ShaderOutputLanguage
// ------------------------------------------------

class ShaderOutputLanguage
{
public:
	virtual ~ShaderOutputLanguage() = default;

	virtual void addGlobalVariable() = 0;
	virtual void addShaderSource() = 0;

	virtual std::vector<GlobalShaderVariable> getVariables() = 0;
	virtual std::vector<ShaderSource> getShaderSources() = 0;

	virtual std::vector<ShaderSource> constructOutput() = 0;
};