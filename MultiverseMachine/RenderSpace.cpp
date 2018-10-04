#include "RenderSpace.h"

RenderSpace::RenderSpace()
{
	
}

void RenderSpace::SetPixelDimentions(GLuint xPos, GLuint yPos, GLuint xWidth, GLuint yHeight, GLuint pxpu)
{
	this->xPos = xPos;
	this->yPos = yPos;
	this->xWidth = xWidth;
	this->yHeight = yHeight;
	if (pxpu == NULL) {
		this->Xpxu = xWidth;
		this->Ypxu = yHeight;
	}
	else {
		this->Xpxu = pxpu;
		this->Ypxu = pxpu / (float(xWidth) / float(yHeight));
		
	}
}
	


void RenderSpace::SetPixelDimentions(GLuint xPos, GLuint yPos, GLuint xWidth, GLuint yHeight, GLuint Xpxs, GLuint Ypxs)
{
	this->xPos = xPos;
	this->yPos = yPos;
	this->xWidth = xWidth;
	this->yHeight = yHeight;
	this->Xpxu = Xpxs;
	this->Ypxu = Ypxs;
}

void RenderSpace::SetComputeGroups(GLuint Num_Groups_x, GLuint Work_Group_Size_x, GLuint Num_Groups_y, GLuint Work_Group_Size_y, GLuint Num_Groups_z, GLuint Work_Group_Size_z)
{
	this->Num_Groups_x = Num_Groups_x;
	this->Num_Groups_y = Num_Groups_y;
	this->Num_Groups_z = Num_Groups_z;
	this->Work_Group_Size_x = Work_Group_Size_x;
	this->Work_Group_Size_y = Work_Group_Size_y;
	this->Work_Group_Size_z = Work_Group_Size_z;
}

RenderSpace::TextureInput* RenderSpace::createInputTexture(std::string InteractionName, bool LinkTex) {
	TextureInput* TexObj = new TextureInput;
	TexObj->ID = RenderAux::CreateTexture2D(Xpxu, Ypxu);
	TexObj->w = Xpxu;
	TexObj->h = Ypxu;
	TexObj->Name = InteractionName;
	//CustomShadersPerTex
	//if (!DisplayProgram) {
	//	TexObj->DisplayProgram = defaultDisplayProgram;
	//}
	//else {
	//	TexObj->DisplayProgram = DisplayProgram;
	//}
	//
	//if (!StepProgram) {
	//	TexObj->StepProgram = defaultSwapProgram;
	//}
	//else {
	//	TexObj->StepProgram = StepProgram;
	//}
	if(LinkTex) InputTextures.push_back(TexObj);
	return TexObj;
}

RenderSpace::TextureOutput RenderSpace::LinkOutputToTexture(std::string InteractionName, TextureInput* TexObj, bool LinkTex) {
	TextureOutput FI;
	FI.TexObj = TexObj;
	FI.Name = InteractionName;
	if(LinkTex) OutputTextures.push_back(FI);
	return FI;
}

void RenderSpace::LinkInputTexture(TextureInput* TexObj) {
	InputTextures.push_back(TexObj);
}

void RenderSpace::InitializeFramebuffer(){
	if (!initiated) {
		backTex = LinkOutputToTexture("_outColor", createInputTexture("backTex", 0),0);
		frontTex = LinkOutputToTexture("_outColor", createInputTexture("FrontTex", 0),0);
		//depthStencilTex = 
		framebuffer = RenderAux::CreateFramebuffer();

		if (OutputTextures.size() == 0) {
			RenderAux::BindRenderTargetToFramebuffer(framebuffer, backTex.TexObj->ID);
		}
		else{
			OutputTextures.push_back(backTex);
			RenderAux::BindMultipleRenderTargetsToFramebuffer(framebuffer, &getOutputTextures()[0], OutputTextures.size());
		}
		RenderAux::CheckFramebufferStatus(framebuffer);
		initiated = true;
	}
}

void RenderSpace::ExecuteComputeStage()
{
	//FRAMEBUFFER
	RenderAux::BindFramebuffer(framebuffer);

	if (defaultComputeProgram) {
		for (GLuint i = 0; i < ComputeData.size(); i++)
			RenderAux::BindShaderStorageBufferObject(ComputeData[i].ID, ComputeData[i].Length, ComputeData[i].UnitSize, i);
		for (GLuint i = 0; i < OutputTextures.size(); i++)
			RenderAux::BindImageFromTextureToUnit(OutputTextures[i].TexObj->ID, i, GL_READ_WRITE, GL_RGBA32F);

		RenderAux::UseComputeProgram(defaultComputeProgram, Num_Groups_x, Work_Group_Size_x, Num_Groups_y, Work_Group_Size_y, Num_Groups_z, Work_Group_Size_z);
	}

	//void* a = this->ComputeData[0].dataRef;
	//a;
}

void RenderSpace::ExecuteStepStage()
{
	//FRAMEBUFFER
	RenderAux::BindFramebuffer(framebuffer);

	RenderAux::UseRenderProgram(defaultDisplayProgram);

	if (OutputTextures.size() == 0) {
		RenderAux::BindRenderTargetToFramebuffer(framebuffer, backTex.TexObj->ID);
	}
	else {
		OutputTextures.pop_back();
		OutputTextures.push_back(backTex);

		RenderAux::BindMultipleRenderTargetsToFramebuffer(framebuffer, &getOutputTextures()[0], OutputTextures.size());
		for (GLuint i = 0; i < OutputTextures.size(); i++)
			RenderAux::BindAtributeName(i, defaultDisplayProgram, OutputTextures[i].Name);
	}

	//UNIFORMS
	RenderAux::BindTextureUnitToUniformName(frontTex.TexObj->ID, 0, defaultDisplayProgram, "_lastStep");
	float* pxpu = new float[2]{ 1 / float(Xpxu), 1 / float(Ypxu) };
	glUniform1fv(glGetUniformLocation(defaultDisplayProgram, "_oneDivPxpu"), 2, &pxpu[0]);
	glUniform1ui(glGetUniformLocation(defaultDisplayProgram, "_t"), time);
	for (GLuint TexObj_ID = 1; TexObj_ID < InputTextures.size() + 1; TexObj_ID++)
		RenderAux::BindTextureUnitToUniformName(InputTextures[TexObj_ID - 1]->ID, TexObj_ID, defaultDisplayProgram, InputTextures[TexObj_ID - 1]->Name);

	//RENDER
	RenderAux::RenderQuad(0, 0, Xpxu, Ypxu, true);
	RenderAux::SwapTexIDs(backTex.TexObj->ID, frontTex.TexObj->ID);
	time++;
	//std::cout << "t: " << time << std::endl;
}

void RenderSpace::ExecuteDisplayStage()
{
	RenderAux::BindFramebuffer(0);
	RenderAux::UseRenderProgram(defaultSwapProgram);
	RenderAux::BindTextureUnitToUniformName(frontTex.TexObj->ID, 0, defaultSwapProgram, "_copyStep");
	RenderAux::RenderQuad(xPos, yPos, xWidth, yHeight, true);
}

std::vector<GLuint> RenderSpace::getOutputTextures()
{
	std::vector<GLuint> texData;
	for (TextureOutput I : OutputTextures) {
		texData.push_back(I.TexObj->ID);
	}
	return texData;
}

RenderSpace::~RenderSpace()
{

}

