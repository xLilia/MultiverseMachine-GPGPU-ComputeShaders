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
		void* dataRef;
	};

	struct TextureInput {
		int w = 1;
		int h = 1;
		std::string Name = "";
		GLuint ID;
	};

	struct TextureOutput {
		TextureInput* TexObj;
		std::string Name = "";
	};

	RenderSpace();

	TextureInput* createInputTexture(std::string InteractionName, bool LinkTex = true);
	void LinkInputTexture(TextureInput* TexObj);
	TextureOutput LinkOutputToTexture(std::string InteractionName, TextureInput* TexObj, bool LinkTex = true);
	std::vector<GLuint> getOutputTextures();
	void ExecuteComputeStage();
	void ExecuteStepStage();
	void ExecuteDisplayStage();
	template<typename T>
	void createStorageBufferObject(GLuint Length, T* Data) {
		ComputeBlock CB;
		CB.ID = RenderAux::CreateShaderStorageBufferObject(Length, Data);
		CB.Length = Length;
		CB.UnitSize = sizeof(T);
		CB.dataRef = Data;
		ComputeData.push_back(CB);
	}
	template<typename type>
	void poke(float xPos, float yPos, float xIWidth, float yIHeight, float winWidth, float winHeight, type pixels[], GLenum GLFormat, GLenum GLType, GLuint TextureTarget) {
		GLdouble Mx = RenderAux::GetMapValueOfAB(Xpxu, xWidth);
		GLdouble My = RenderAux::GetMapValueOfAB(Ypxu, yHeight);
		xPos = RenderAux::MapAToMapValue(xPos - this->xPos, Mx);
		yPos = RenderAux::MapAToMapValue((winHeight - this->yPos) - yPos, My);
		RenderAux::UseRenderProgram(defaultDisplayProgram);
		RenderAux::BindTextureUnitToUniformName(TextureTarget, 0, defaultDisplayProgram, "_lastStep");
		glTexSubImage2D(GL_TEXTURE_2D, 0, xPos, yPos, xIWidth, yIHeight, GLFormat, GLType, pixels);
		check_gl_error();
	}
	void InitializeFramebuffer();
	void SetPixelDimentions(GLuint xPos, GLuint yPos, GLuint xWidth, GLuint yHeight, GLuint pxpu = NULL);
	void SetPixelDimentions(GLuint xPos, GLuint yPos, GLuint xWidth, GLuint yHeight, GLuint Xpxs, GLuint Ypxs);
	void SetComputeGroups(GLuint Num_Groups_x = 1, GLuint Work_Group_Size_x = 1, GLuint Num_Groups_y = 1, GLuint Work_Group_Size_y = 1, GLuint Num_Groups_z = 1, GLuint Work_Group_Size_z = 1);
	TextureOutput backTex;
	TextureOutput frontTex;
	GLuint defaultComputeProgram;
	GLuint defaultDisplayProgram;
	GLuint defaultSwapProgram;
	//GLuint depthStencilTex;
	GLuint framebuffer;
	std::vector<TextureInput*>InputTextures;
	std::vector<TextureOutput>OutputTextures;
	std::vector<ComputeBlock>ComputeData;
	bool initiated = false;
	GLuint time = 0;
	~RenderSpace();
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

