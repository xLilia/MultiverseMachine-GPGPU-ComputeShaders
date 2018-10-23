#include "RenderSpace.h"

RenderSpace::RenderSpace(std::string IDname)
{
	this->IDname = IDname;
}

void RenderSpace::InitializeRenderSpaceFramebuffer()
{

	if (!RS_Framebuffer->bHasFrontBackTex) {
		std::cout << "Back and Front Texture not set! Bypassing Framebuffer initialization of \""<< this->IDname << "\"." << std::endl;
		return;
	}

	RS_Framebuffer->ID = RenderAux::CreateFramebuffer();
	RS_Framebuffer->TextureOutputs.push_back(RS_Framebuffer->backTexture);
	RenderAux::BindMultipleRenderTargetsToFramebuffer(RS_Framebuffer->ID, &getTextureOutputs()[0], RS_Framebuffer->TextureOutputs.size());
	RS_Framebuffer->bFramebufferInitialized = RenderAux::CheckFramebufferStatus(RS_Framebuffer->ID);
	RS_Framebuffer->TextureOutputs.pop_back();
}

std::vector<GLuint> RenderSpace::getTextureOutputs()
{
	std::vector<GLuint> texData;
	for (TextureOutput* I : RS_Framebuffer->TextureOutputs) {
		texData.push_back(I->TexObj->ID);
	}
	return texData;
}

void RenderSpace::SetTexturePixelDimention(GLuint xPos, GLuint yPos, GLuint xWidth, GLuint yHeight, GLuint pxpu)
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

void RenderSpace::SetTexturePixelDimention(GLuint xPos, GLuint yPos, GLuint xWidth, GLuint yHeight, GLuint Xpxs, GLuint Ypxs)
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

RenderSpace::Texture* RenderSpace::GenerateTexture(std::string TextureName) {
	Texture* TexObj = new Texture;
	TexObj->ID = RenderAux::CreateTexture2D(Xpxu, Ypxu);
	TexObj->w = Xpxu;
	TexObj->h = Ypxu;
	TexObj->Name = TextureName;
	return TexObj;
}

void RenderSpace::SetBackFrontTextures(Texture* Back_TextureOut, Texture* Front_TextureIn)
{
	RS_Framebuffer->backTexture->TexObj = Back_TextureOut;
	RS_Framebuffer->backTexture->Name = Back_TextureOut->Name;
	RS_Framebuffer->frontTexture->TexObj = Front_TextureIn;
	RS_Framebuffer->frontTexture->Name = Front_TextureIn->Name;
	RS_Framebuffer->bHasFrontBackTex = true;
}

void RenderSpace::LinkOutputToTexture(std::string OutputName, Texture* TexObj) {
	TextureOutput* Tout = new TextureOutput;
	Tout->TexObj = TexObj;
	Tout->Name = OutputName;
	RS_Framebuffer->TextureOutputs.push_back(Tout);
}

void RenderSpace::LinkComputeBlock(ComputeBlock* ComputeBlockObj)
{
	RS_Framebuffer->SSBOs.push_back(ComputeBlockObj);
}

void RenderSpace::LinkShaderPrograms(GLuint RenderProgram, GLuint DisplayProgram, GLuint ComputeProgram)
{
	RS_Framebuffer->defaultComputeProgram = ComputeProgram;
	RS_Framebuffer->defaultRenderProgram = RenderProgram;
	RS_Framebuffer->defaultDisplayProgram = DisplayProgram;
}

void RenderSpace::LinkInputTexture(Texture* TexObj) {
	RS_Framebuffer->Textures.push_back(TexObj);
}

void RenderSpace::ExecuteComputeStage()
{
	if (RS_Framebuffer->bFramebufferInitialized) {
		//FRAMEBUFFER
		RenderAux::BindFramebuffer(RS_Framebuffer->ID);

		if (RS_Framebuffer->defaultComputeProgram) {
			//SSBOs
			for (GLuint i = 0; i < RS_Framebuffer->SSBOs.size(); i++)
				RenderAux::BindShaderStorageBufferObject(RS_Framebuffer->SSBOs[i]->ID, RS_Framebuffer->SSBOs[i]->Length, RS_Framebuffer->SSBOs[i]->UnitSize, i);
			
			//TEX
			RS_Framebuffer->TextureOutputs.push_back(RS_Framebuffer->backTexture);
			for (GLuint i = 0; i < RS_Framebuffer->TextureOutputs.size(); i++)
				RenderAux::BindImageFromTextureToUnit(RS_Framebuffer->TextureOutputs[i]->TexObj->ID, i, GL_READ_WRITE, GL_RGBA32F);
			RS_Framebuffer->TextureOutputs.pop_back();

			RenderAux::UseComputeProgram(RS_Framebuffer->defaultComputeProgram, Num_Groups_x, Work_Group_Size_x, Num_Groups_y, Work_Group_Size_y, Num_Groups_z, Work_Group_Size_z);
		}
	}
}

void RenderSpace::ExecuteRenderStage()
{
	if (RS_Framebuffer->bFramebufferInitialized) {
		//FRAMEBUFFER	
		RenderAux::BindFramebuffer(RS_Framebuffer->ID);
		RenderAux::UseRenderProgram(RS_Framebuffer->defaultRenderProgram);

		//RENDER TARGETS - OUTPUT
		RS_Framebuffer->TextureOutputs.push_back(RS_Framebuffer->backTexture);
		RenderAux::BindMultipleRenderTargetsToFramebuffer(RS_Framebuffer->ID, &getTextureOutputs()[0], RS_Framebuffer->TextureOutputs.size());
		for (GLuint i = 0; i < RS_Framebuffer->TextureOutputs.size(); i++)
			RenderAux::BindAtributeName(i, RS_Framebuffer->defaultRenderProgram, RS_Framebuffer->TextureOutputs[i]->Name);
		RS_Framebuffer->TextureOutputs.pop_back();

		//(!)
		//UNIFORMS - INPUT
		RenderAux::BindTextureUnitToUniformName(RS_Framebuffer->frontTexture->TexObj->ID, 0, RS_Framebuffer->defaultRenderProgram, RS_Framebuffer->frontTexture->Name);
		for (GLuint TexObj_ID = 1; TexObj_ID < RS_Framebuffer->Textures.size() + 1; TexObj_ID++)
			RenderAux::BindTextureUnitToUniformName(RS_Framebuffer->Textures[TexObj_ID - 1]->ID, TexObj_ID, RS_Framebuffer->defaultRenderProgram, RS_Framebuffer->Textures[TexObj_ID - 1]->Name);

		//float* pxpu = new float[2]{ 1 / float(Xpxu), 1 / float(Ypxu) };
		//glUniform1fv(glGetUniformLocation(defaultRenderProgram, "_oneDivPxpu"), 2, &pxpu[0]);
		//glUniform1ui(glGetUniformLocation(defaultRenderProgram, "_t"), time);

		//RENDER
		RenderAux::RenderQuad(0, 0, Xpxu, Ypxu, true);
		time++;
		RenderAux::SwapTexIDs(RS_Framebuffer->backTexture->TexObj->ID, RS_Framebuffer->frontTexture->TexObj->ID);
		//std::cout << "t: " << time << std::endl;
	}
}

void RenderSpace::ExecuteDisplayStage()
{
	if (RS_Framebuffer->bFramebufferInitialized) {
		RenderAux::BindFramebuffer(0);
		RenderAux::UseRenderProgram(RS_Framebuffer->defaultDisplayProgram);
		RenderAux::BindTextureUnitToUniformName(RS_Framebuffer->frontTexture->TexObj->ID, 0, RS_Framebuffer->defaultDisplayProgram, RS_Framebuffer->frontTexture->TexObj->Name);
		RenderAux::RenderQuad(xPos, yPos, xWidth, yHeight, true);
	}
}


RenderSpace::~RenderSpace()
{
	delete RS_Framebuffer->frontTexture;
	delete RS_Framebuffer->backTexture;
	delete RS_Framebuffer;
}

