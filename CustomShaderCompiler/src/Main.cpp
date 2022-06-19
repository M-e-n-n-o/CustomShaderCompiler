#include <iostream>
#include "shaderCompiler/automata/Automata.h"
#include "shaderCompiler/automata/Regex.h"
#include "shaderCompiler/ShaderLanguage.h"
#include "shaderCompiler/GLSLShader.h"
#include "shaderCompiler/ShaderClass.h"

int main()
{
	// This is for testing purposes of the Automata and Regex systems
	Regex regex("a.(a|b)*.b");
	// Regex regex("'appel'.(a|b)*.b");
	
	Automata m = regex.constructAutomata();
	m.makeDeterministic();
	m.printTransitions();
	
	std::vector<std::string> input = { "appel", "a", "b" };
	bool isValid = m.validate(input);
	std::cout << isValid << std::endl;
	
	isValid = m.validate("ab");
	std::cout << isValid << std::endl;
	
	isValid = m.validate("aa");
	std::cout << isValid << std::endl;
	
	isValid = m.validate("aaab");
	std::cout << isValid << std::endl;
	
	isValid = m.validate("abbaab");
	std::cout << isValid << std::endl;

	
	// This is for testing purposes of the shaderCompiler
// 	std::string t = R"(
// 		#shader "Example.bcsl"
// 		#include "ByteCatStandard.bcsl"
//
// 		iostream {
// 		    vec3 vertexPos
// 		    vec2 texCoord
// 		    vec2 uv
// 		    vec4 fragColor
// 		}
//
// 		uniform sampler2D tex
//
// 		vertex {
// 		    void main()
// 		    {
// 		        uv = texCoord;
// 		        gl_Position = objectToClipSpace(vec4(vertexPos, 1.0));
// 		    }
// 		}
//
// 		fragment {
// 		    void main()
// 		    {
// 		        fragColor = texture(tex, uv);
// 		    }
// 		}
// 	)";
// 	
// 	auto c = std::make_shared<GLSLShader>();
// 	ShaderLanguage l = ShaderLanguage(c);
// 	l.addConverter(std::make_shared<ShaderClass>());
// 	
// 	auto output = l.convert(t);
// 	auto sources = output->constructOutput();
//
// 	std::cout << "Vertex shader: " << std::endl;
// 	std::cout << sources[0].code << std::endl << std::endl;
//
// 	std::cout << "Fragment shader: " << std::endl;
// 	std::cout << sources[1].code << std::endl << std::endl;
}