#include "ShaderWizard.h"

std::string ShaderWizard::readShaderCode(const char * filename) {
	std::ifstream Input(filename);
	if (!Input.good()) {
		std::cout << " FailedToLoad: " << filename << std::endl;
		return NULL;
		Input.close();
	}
	return std::string(
		std::istreambuf_iterator<char>(Input),
		std::istreambuf_iterator<char>());
	Input.close();
}

bool ShaderWizard::checkStatus(GLuint objectID,
	PFNGLGETSHADERIVPROC objectPropertyGetterFunc,
	PFNGLGETSHADERINFOLOGPROC getInfoLogFunc,
	GLenum statusType)
{
	GLint Status;
	objectPropertyGetterFunc(objectID, statusType, &Status);
	if (Status != GL_TRUE) {

		GLint infoLogLength;
		objectPropertyGetterFunc(objectID, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* buffer = new GLchar[infoLogLength];

		GLsizei bufferSize;
		getInfoLogFunc(objectID, infoLogLength, &bufferSize, buffer);
		std::cout << buffer << std::endl;
		delete[] buffer;
		return false;
	}
	return true;
}

bool ShaderWizard::checkShaderStatus(GLuint ShaderID) {
	return checkStatus(ShaderID, glGetShaderiv, glGetShaderInfoLog, GL_COMPILE_STATUS);
}

bool ShaderWizard::checkProgramStatus(GLuint ProgramID) {
	return checkStatus(ProgramID, glGetProgramiv, glGetProgramInfoLog, GL_LINK_STATUS);
}

void ShaderWizard::installShaders(const char * VertexShader, const char * FragmentShader, const char * ComputeShader)
{
	GLuint programID;
	GLuint VertexShaderID;
	GLuint FragmentShaderID;
	GLuint ComputeShaderID;
	const GLchar* adapter[1];
	std::string temp;

	if (VertexShader) {
		VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		temp = readShaderCode(VertexShader);
		adapter[0] = temp.c_str();
		glShaderSource(VertexShaderID, 1, adapter, 0);
		glCompileShader(VertexShaderID);
		if (!checkShaderStatus(VertexShaderID)) 
			return;
	}

	if (FragmentShader) {
		FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		temp = readShaderCode(FragmentShader);
		adapter[0] = temp.c_str();
		glShaderSource(FragmentShaderID, 1, adapter, 0);
		glCompileShader(FragmentShaderID);
		if (!checkShaderStatus(FragmentShaderID)) 
 			return;
	}

	if (ComputeShader) {
		ComputeShaderID = glCreateShader(GL_COMPUTE_SHADER);
		temp = readShaderCode(ComputeShader);
		adapter[0] = temp.c_str();
		glShaderSource(ComputeShaderID, 1, adapter, 0);
		glCompileShader(ComputeShaderID);
		if (!checkShaderStatus(ComputeShaderID))
			return;
	}

	programID = glCreateProgram();

	if(VertexShader)
		glAttachShader(programID, VertexShaderID);

	if (FragmentShader)
		glAttachShader(programID, FragmentShaderID);

	if (ComputeShader)
		glAttachShader(programID, ComputeShaderID);

	glLinkProgram(programID);

	if (!checkProgramStatus(programID))
		return;

	if (VertexShader)
		glDeleteShader(VertexShaderID);
	if (FragmentShader)
		glDeleteShader(FragmentShaderID);
	if (ComputeShader)
		glDeleteShader(ComputeShaderID);

	InstlledProgramIDs.push_back(programID);
}

void ShaderWizard::clearPrograms()
{
	InstlledProgramIDs.clear();
	InstlledProgramIDs.shrink_to_fit();
}

ShaderWizard::~ShaderWizard()
{

}
