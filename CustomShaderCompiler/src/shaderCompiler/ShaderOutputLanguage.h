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
};

struct ShaderSource
{
	ShaderSourceType type;
	std::string code;
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
	std::string name;
	VariableType type;
	VariableDataType dataType;
};


// ------------------------------------------------
//				ShaderOutputLanguage
// ------------------------------------------------

class ShaderOutputLanguage
{
public:
	ShaderOutputLanguage() = default;
	virtual ~ShaderOutputLanguage() = default;
	
	virtual void addGlobalVariable(const GlobalShaderVariable& variable) = 0;
	virtual void addShaderSource(const ShaderSource& source) = 0;

	virtual std::vector<ShaderSource> constructOutput() = 0;
};