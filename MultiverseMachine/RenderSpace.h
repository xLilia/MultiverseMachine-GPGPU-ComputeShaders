#pragma once
#include "RenderAux.h"
#include <vector>
#include <string>
class RenderSpace
{
public:

	struct ComputeBlock{
		GLuint ID;
		GLuint Length;
		GLuint UnitSize;
	};

	struct Texture {
		int w = 1;
		int h = 1;
		std::string Name;
		GLuint ID;
	};

	struct TextureOutput {
		Texture* TexObj;
		std::string Name;
	};

	struct Framebuffer {
		GLuint ID;
		TextureOutput* backTexture = new TextureOutput;
		TextureOutput* frontTexture = new TextureOutput;
		std::vector<Texture*>Textures;
		std::vector<TextureOutput*>TextureOutputs;
		std::vector<ComputeBlock*>SSBOs;
		GLuint defaultComputeProgram;
		GLuint defaultRenderProgram;
		GLuint defaultDisplayProgram;
		//GLuint depthStencilTex;
		bool bFramebufferInitialized = false;
		bool bHasFrontBackTex = false;
	};
	std::string IDname;
	RenderSpace(std::string IDname);
	void InitializeRenderSpaceFramebuffer();
	std::vector<GLuint> getTextureOutputs();
	Texture* GenerateTexture(std::string OutputName);
	void SetBackFrontTextures(Texture * Back_TextureOut, Texture * Front_TextureIn);
	void LinkInputTexture(Texture* TexObj);
	void LinkOutputToTexture(std::string OutputName, Texture* TexObj);
	void LinkComputeBlock(ComputeBlock* ComputeBlockObj);
	void LinkShaderPrograms(GLuint RenderProgram = 0, GLuint DisplayProgram = 0, GLuint ComputeProgram = 0);
	void ExecuteComputeStage();
	void ExecuteRenderStage();
	void ExecuteDisplayStage();
	void SetTexturePixelDimention(GLuint xPos, GLuint yPos, GLuint xWidth, GLuint yHeight, GLuint pxpu = NULL);
	void SetTexturePixelDimention(GLuint xPos, GLuint yPos, GLuint xWidth, GLuint yHeight, GLuint Xpxs, GLuint Ypxs);
	void SetComputeGroups(GLuint Num_Groups_x = 1, GLuint Work_Group_Size_x = 1, GLuint Num_Groups_y = 1, GLuint Work_Group_Size_y = 1, GLuint Num_Groups_z = 1, GLuint Work_Group_Size_z = 1);
	
	
	Framebuffer* RS_Framebuffer = new Framebuffer;
	GLuint time = 0;
	~RenderSpace();
	template<typename T>
	ComputeBlock* createStorageBufferObject(GLuint Length, T* Data) {
		ComputeBlock* CB = new ComputeBlock;
		CB->ID = RenderAux::CreateShaderStorageBufferObject<T>(Length, Data);
		CB->Length = Length;
		CB->UnitSize = sizeof(T);
		return CB;
	}

	//DEBUG
	template<typename type>
	void poke(float xPos, float yPos, float xIWidth, float yIHeight, float winWidth, float winHeight, type pixels[], GLenum GLFormat, GLenum GLType, GLuint TextureTarget) {
		GLdouble Mx = RenderAux::MapValueOfAB(Xpxu, xWidth);
		GLdouble My = RenderAux::MapValueOfAB(Ypxu, yHeight);
		xPos = RenderAux::MapAToMapValue(xPos - this->xPos, Mx);
		yPos = RenderAux::MapAToMapValue((winHeight - this->yPos) - yPos, My);
		RenderAux::UseRenderProgram(RS_Framebuffer->defaultRenderProgram);
		RenderAux::BindTextureUnitToUniformName(TextureTarget, 0, RS_Framebuffer->defaultRenderProgram, RS_Framebuffer->frontTexture->Name);
		glTexSubImage2D(GL_TEXTURE_2D, 0, xPos, yPos, xIWidth, yIHeight, GLFormat, GLType, pixels);
		check_gl_error();
	}

private:
	GLuint pxpu = 64; //pixel unit
	GLuint xPos = 0;
	GLuint yPos = 0;
	GLuint xWidth = 512;
	GLuint yHeight = 256;
	GLuint Xpxu = 64; //pixels per Xunit
	GLuint Ypxu = 32; //pixels per Yunit

	GLuint Num_Groups_x = 1;
	GLuint Work_Group_Size_x = 1;
	GLuint Num_Groups_y = 1;
	GLuint Work_Group_Size_y = 1;
	GLuint Num_Groups_z = 1;
	GLuint Work_Group_Size_z = 1;
};

