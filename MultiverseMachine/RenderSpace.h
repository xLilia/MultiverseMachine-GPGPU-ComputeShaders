#pragma once
#include "RenderAux.h"
#include <vector>
#include <string>

class RenderSpace
{
public:

	std::string IDname;
	RenderSpace(std::string IDname);
	void InitializeRenderSpaceFramebuffer();
	std::vector<GLuint> getTextureOutputs();
	void LinkInputTexture(RenderAux::Texture* TexObj);
	void LinkOutputToTexture(std::string OutputName, RenderAux::Texture* TexObj);
	void LinkComputeBlock(RenderAux::ComputeBlock* ComputeBlockObj);
	void LinkShaderPrograms(GLuint RenderProgram = 0, GLuint DisplayProgram = 0, GLuint ComputeProgram = 0);
	void ExecuteComputeStage();
	void ExecuteRenderStage();
	void ExecuteDisplayStage();
	void SetPixelSpaceProperties(GLuint xPos, GLuint yPos, GLuint xWidth, GLuint yHeight, GLuint pxpu = NULL);
	void SetPixelSpaceProperties(GLuint xPos, GLuint yPos, GLuint xWidth, GLuint yHeight, GLuint Xpxs, GLuint Ypxs);
	void SetDeltaPixelSpaceProperties(GLuint xPos, GLuint yPos, GLuint xWidth, GLuint yHeight, GLuint Xpxs, GLuint Ypxs);
	void SetComputeGroups(GLuint Num_Groups_x = 1, GLuint Work_Group_Size_x = 1, GLuint Num_Groups_y = 1, GLuint Work_Group_Size_y = 1, GLuint Num_Groups_z = 1, GLuint Work_Group_Size_z = 1);
	void SetDefaultBackFrontTextures();
	void SetBackFrontTextures(RenderAux::Texture* Back_TextureOut, RenderAux::Texture* Front_TextureIn);
	RenderAux::RenderQuadProperties GetSpaceProperties();
	RenderAux::Framebuffer* RS_Framebuffer = new RenderAux::Framebuffer;
	GLuint time = 0;
	~RenderSpace();

	//DEBUG
	template<typename type>
	void poke(float xPos, float yPos, float xIWidth, float yIHeight, float winWidth, float winHeight, type pixels[], GLenum GLFormat, GLenum GLType, GLuint TextureTarget) {
		GLdouble Mx = RenderAux::MapValueOfAB(SpaceProperties.Xpxu, SpaceProperties.xWidth);
		GLdouble My = RenderAux::MapValueOfAB(SpaceProperties.Ypxu, SpaceProperties.yHeight);
		xPos = RenderAux::MapAToMapValue(xPos - this->SpaceProperties.xPos, Mx);
		yPos = RenderAux::MapAToMapValue((winHeight - this->SpaceProperties.yPos) - yPos, My);
		RenderAux::UseRenderProgram(RS_Framebuffer->defaultRenderProgram);
		RenderAux::BindTextureUnitToUniformName(TextureTarget, 0, RS_Framebuffer->defaultRenderProgram, RS_Framebuffer->frontTexture->Name);
		glTexSubImage2D(GL_TEXTURE_2D, 0, xPos, yPos, xIWidth, yIHeight, GLFormat, GLType, pixels);
		check_gl_error();
	}

private:
	
	RenderAux::RenderQuadProperties SpaceProperties;

	GLuint Num_Groups_x = 1;
	GLuint Work_Group_Size_x = 1;
	GLuint Num_Groups_y = 1;
	GLuint Work_Group_Size_y = 1;
	GLuint Num_Groups_z = 1;
	GLuint Work_Group_Size_z = 1;
};

