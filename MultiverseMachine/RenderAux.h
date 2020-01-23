#pragma once
#include <GL\glew.h>
#include <GLFW/glfw3.h>
#include <chrono>
#include <thread>
#include <string>
#include "GLError.h" 
#include <vector>

static class RenderAux
{
public:
	RenderAux();
	~RenderAux();

	static struct ScreenQuad {
		GLfloat fullquad_v[8] =
		{
			-1,-1,
			+1,-1,
			+1,+1,
			-1,+1
		};

		GLuint fullquad_i[4] =
		{
			0,1,2,3
		};
	};

	static struct RenderQuadProperties {
		GLuint pxpu = 64; //pixel unit
		GLuint xPos = 0;
		GLuint yPos = 0;
		GLuint xWidth = 512;
		GLuint yHeight = 256;
		GLuint Xpxu = 64; //pixels per Xunit
		GLuint Ypxu = 32; //pixels per Yunit
	};

	static struct ComputeBlock {
		GLuint ID;
		GLuint Length;
		GLuint UnitSize;
	};

	static struct Texture {
		int w = 1;
		int h = 1;
		std::string Name = "";
		GLuint ID = 0;
	};

	static struct TextureOutput {
		Texture* TexObj;
		std::string Name;
	};

	static struct Framebuffer {
		GLuint ID;
		TextureOutput* backTexture = new TextureOutput;
		TextureOutput* frontTexture = new TextureOutput;
		std::vector<RenderAux::Texture*>Textures;
		std::vector<RenderAux::TextureOutput*>TextureOutputs;
		std::vector<RenderAux::ComputeBlock*>SSBOs;
		GLuint defaultComputeProgram;
		GLuint defaultRenderProgram;
		GLuint defaultDisplayProgram;
		//GLuint depthStencilTex;
		bool bFramebufferInitialized = false;
		bool bHasFrontBackTex = false;
		bool bSkipDrawCall = false;
		bool bSkipcomputeCall = false;
	};

	static struct Create {

	};

	static void RenderQuad(GLfloat x, GLfloat y, GLfloat w, GLfloat h, bool removeProgramAferUse = true, GLuint Shader = 0);
	static GLuint CreateTexture2D(GLfloat w, GLfloat h);
	static GLuint CreateFramebuffer();
	static RenderAux::Texture* GenerateTexture(std::string TextureOutputName, GLfloat Xpxu, GLfloat Ypxu);
	static void BindRenderTargetToFramebuffer(GLuint Framebuffer, GLuint TextureAtachment, GLuint DepthStencilAtachment = NULL);
	static void BindMultipleRenderTargetsToFramebuffer(GLuint Framebuffer, GLuint * ColorAtachments, GLuint nRenderTargets, GLuint DepthStencilAtachment = NULL);
	static void PrepareFramebufferForMultipleRenderTargets(GLuint Framebuffer, GLuint nRenderTargets);
	static void BindImageFromTextureToUnit(GLuint TexID, GLuint UnitIndex, GLenum Access = GL_WRITE_ONLY, GLenum Format = GL_RGBA, GLuint LODlvl = 0, bool layered = false, GLuint Layer = 0);
	static void BindTextureUnitToUniformID(GLuint TexID, GLuint TexUnit);
	static void BindTextureUnitToUniformName(GLuint TexID, GLuint TexUnit, GLuint ProgramID, std::string UniformName);
	static void BindAtributeName(GLuint AtribID, GLuint ProgramID, std::string AtributeName);
	static void CleanTextureUnit(GLuint TexUnit);
	static void BindFramebuffer(GLuint Framebuffer);
	static bool CheckFramebufferStatus(GLuint Framebuffer);
	static void UseRenderProgram(GLuint ShaderID);
	static void PrintComputeWorkGroupStats();
	static void UseComputeProgram(GLuint ComputeShaderID, GLuint num_groups_x = 1, GLuint work_group_size_x = 1, GLuint num_groups_y = 1, GLuint work_group_size_y = 1, GLuint num_groups_z = 1, GLuint work_group_size_z = 1);
	static void SwapTexIDs(GLuint &Tex_A, GLuint &Tex_B);
	static void FPS_lock(int FPSlock);
	static double FPS_counter(double Time);
	static GLdouble MapValueOfAB(GLdouble A, GLdouble B);
	
	template<typename T>
	static T MapAToMapValue(T A, GLdouble Map) {
		return A * Map;
	}

	template<typename T>
	static GLuint CreateShaderStorageBufferObject(GLuint N_Units, T* package)
	{
		GLuint SSboID;
		glCreateBuffers(1, &SSboID);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSboID);
		glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(T) * N_Units, /*&package[0]*/ NULL, GL_STATIC_DRAW);

		GLint bufMask = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT;
		T* mappedSSBO = (T*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, sizeof(T) * N_Units, bufMask);

		for (int i = 0; i < N_Units; i++) {
			mappedSSBO[i] = package[i];
		}

		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
		//glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
		check_gl_error();
		
		return SSboID;
	}

	template<typename T>
	static ComputeBlock* CreateComputeSSBO(GLuint Length, T* Data) {
		ComputeBlock* CB = new ComputeBlock;
		CB->ID = RenderAux::CreateShaderStorageBufferObject<T>(Length, Data);
		CB->Length = Length;
		CB->UnitSize = sizeof(T);
		return CB;
	}

	template<typename T>
	static void FetchShaderStorageBufferObject(GLuint N_Units, GLuint Uindex, GLuint ssboID, T* mappedSSBO)
	{
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboID);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, Uindex, ssboID);
		GLint bufMask = GL_MAP_READ_BIT;
		std::memcpy(mappedSSBO, (T*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, sizeof(T) * N_Units, bufMask), sizeof(T) * N_Units);
		check_gl_error();
		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);
		check_gl_error();
	}

	static void BindShaderStorageBufferObject(GLuint SSboID, GLuint N_Units, GLuint UnitSize, GLuint index);

	//template<typename Tcall, typename Tdata>
	//static void BindUniformValues(Tcall glUniformCall, GLuint ShaderProgram, std::string Name, GLuint Count, Tdata* data) {
	//	glUniformCall(glGetUniformLocation(ShaderProgram, Name.c_str()), Count, &data[0]);
	//	return;
	//}
	//template<typename Tcall, typename Tdata>
	//static void BindUniformValue(Tcall glUniformCall, GLuint ShaderProgram, std::string Name, Tdata v0) {
	//	glUniformCall(glGetUniformLocation(ShaderProgram, Name.c_str()), v0);
	//	return;
	//}
	//template<typename Tcall, typename Tdata>
	//static void BindUniformValue(Tcall glUniformCall, GLuint ShaderProgram, std::string Name, Tdata v0, Tdata v1) {
	//	glUniformCall(glGetUniformLocation(ShaderProgram, Name.c_str()), v0, v1);
	//	return;
	//}
	//template<typename Tcall, typename Tdata>
	//static void BindUniformValue(Tcall glUniformCall, GLuint ShaderProgram, std::string Name, Tdata v0, Tdata v1, Tdata v2) {
	//	glUniformCall(glGetUniformLocation(ShaderProgram, Name.c_str()), v0, v1, v2);
	//	return;
	//}
	//template<typename Tcall, typename Tdata>
	//static void BindUniformValue(Tcall glUniformCall, GLuint ShaderProgram, std::string Name, Tdata v0, Tdata v1, Tdata v2, Tdata v3) {
	//	glUniformCall(glGetUniformLocation(ShaderProgram, Name.c_str()), v0, v1, v2, v3);
	//	return;
	//}

	//template<typename Tcall, typename Tdata>
	//static void BindUniform(Tcall glUniformCall, GLuint ShaderProgram, std::string Name, GLuint Count = NULL, Tdata v0 = NULL, Tdata v1 = NULL, Tdata v2 = NULL, Tdata v3 = NULL) {
	//	if (Count != 0) {
	//		BindUniformValues(glUniformCall, ShaderProgram, Name, Count, &v0);
	//		return;
	//	}
	//	else {
	//		if (v0 != NULL) {
	//			BindUniformValue(glUniformCall, ShaderProgram, Name, v0);
	//			return;
	//		}
	//		if (v1 != NULL) {
	//			BindUniformValue(glUniformCall, ShaderProgram, Name, v0, v1);
	//			return;
	//		}
	//		if (v2 != NULL) {
	//			BindUniformValue(glUniformCall, ShaderProgram, Name, v0, v1, v2);
	//			return;
	//		}
	//		if (v3 != NULL) {
	//			BindUniformValue(glUniformCall, ShaderProgram, Name, v0, v1, v2, v3);
	//			return;
	//		}
	//	}
	//
	//}
};
