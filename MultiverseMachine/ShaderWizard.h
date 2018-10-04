#pragma once
#include <GL\glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <vector>

class ShaderWizard
{
public:
	void installShaders(const char * VertexShader = NULL, const char * FragmentShader = NULL, const char * ComputeShader = NULL);
	void clearPrograms();
	~ShaderWizard();
	std::vector<GLuint> InstlledProgramIDs;

private:
	std::string readShaderCode(const char * filename);
	bool checkStatus(GLuint objectID, PFNGLGETSHADERIVPROC objectPropertyGetterFunc, PFNGLGETSHADERINFOLOGPROC getInfoLogFunc, GLenum statusType);
	bool checkShaderStatus(GLuint ShaderID);
	bool checkProgramStatus(GLuint ProgramID);
};

