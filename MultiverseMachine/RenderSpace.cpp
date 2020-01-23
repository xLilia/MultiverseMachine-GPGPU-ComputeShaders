#include "RenderSpace.h"

RenderSpace::RenderSpace(std::string IDname)
{
	this->IDname = IDname;
}

//void RenderSpace::InitializeRenderSpaceFramebuffer(
//	GLuint RenderProgram, GLuint DisplayProgram, GLuint ComputeProgram,
//	GLuint PosX, GLuint PosY, GLuint xWidth, GLuint yHeight, GLuint Xpxs, GLuint Ypxs)
//{
//	LinkShaderPrograms(RenderProgram, DisplayProgram, ComputeProgram);
//	SetTexturePixelDimention(PosX, PosY, xWidth, yHeight, Xpxs, Ypxs);
//	SetDefaultBackFrontTextures();
//	InitializeRenderSpaceFramebuffer();
//}
//
//void RenderSpace::InitializeRenderSpaceFramebuffer(
//	GLuint RenderProgram, GLuint DisplayProgram, GLuint ComputeProgram,
//	GLuint PosX, GLuint PosY, GLuint xWidth, GLuint yHeight, GLuint Xpxs, GLuint Ypxs,
//	RenderSpace::Texture* Back_TextureOut, RenderSpace::Texture* Front_TextureIn)
//{
//	LinkShaderPrograms(RenderProgram,DisplayProgram,ComputeProgram);
//	SetTexturePixelDimention(PosX, PosY, xWidth, yHeight, Xpxs, Ypxs);
//	SetBackFrontTextures(Back_TextureOut, Front_TextureIn);
//	InitializeRenderSpaceFramebuffer();
//}

void RenderSpace::InitializeRenderSpaceFramebuffer()
{

	if (!RS_Framebuffer->bHasFrontBackTex) {
		std::cout << "\"" << this->IDname << "\"" << " Framebuffer wont be displayable. Bypassing Default Framebuffer initialization. (Back and Front Texture not set)" << std::endl;
		RS_Framebuffer->ID = RenderAux::CreateFramebuffer();
		RenderAux::BindMultipleRenderTargetsToFramebuffer(RS_Framebuffer->ID, &getTextureOutputs()[0], RS_Framebuffer->TextureOutputs.size());
		RS_Framebuffer->bFramebufferInitialized = RenderAux::CheckFramebufferStatus(RS_Framebuffer->ID);
		return;
	}
	else {
		RS_Framebuffer->ID = RenderAux::CreateFramebuffer();
		RS_Framebuffer->TextureOutputs.push_back(RS_Framebuffer->backTexture);
		RenderAux::BindMultipleRenderTargetsToFramebuffer(RS_Framebuffer->ID, &getTextureOutputs()[0], RS_Framebuffer->TextureOutputs.size());
		RS_Framebuffer->bFramebufferInitialized = RenderAux::CheckFramebufferStatus(RS_Framebuffer->ID);
		RS_Framebuffer->TextureOutputs.pop_back();
	}
}

std::vector<GLuint> RenderSpace::getTextureOutputs()
{
	std::vector<GLuint> texData;
	for (RenderAux::TextureOutput* I : RS_Framebuffer->TextureOutputs) {
		texData.push_back(I->TexObj->ID);
	}
	return texData;
}

void RenderSpace::SetPixelSpaceProperties(GLuint xPos, GLuint yPos, GLuint xWidth, GLuint yHeight, GLuint pxpu)
{
	this->SpaceProperties.xPos = xPos;
	this->SpaceProperties.yPos = yPos;
	this->SpaceProperties.xWidth = xWidth;
	this->SpaceProperties.yHeight = yHeight;
	if (pxpu == NULL) {
		this->SpaceProperties.Xpxu = xWidth;
		this->SpaceProperties.Ypxu = yHeight;
	}
	else {
		this->SpaceProperties.Xpxu = pxpu;
		this->SpaceProperties.Ypxu = pxpu / (float(xWidth) / float(yHeight));
		
	}
}	

void RenderSpace::SetPixelSpaceProperties(GLuint xPos, GLuint yPos, GLuint xWidth, GLuint yHeight, GLuint Xpxs, GLuint Ypxs)
{
	this->SpaceProperties.xPos = xPos;
	this->SpaceProperties.yPos = yPos;
	this->SpaceProperties.xWidth = xWidth;
	this->SpaceProperties.yHeight = yHeight;
	this->SpaceProperties.Xpxu = Xpxs;
	this->SpaceProperties.Ypxu = Ypxs;
}

void RenderSpace::SetDeltaPixelSpaceProperties(GLuint DxPos, GLuint DyPos, GLuint DxWidth, GLuint DyHeight, GLuint DXpxs, GLuint DYpxs)
{
	SetPixelSpaceProperties(SpaceProperties.xPos + DxPos, SpaceProperties.yPos + DyPos, SpaceProperties.xWidth + DxWidth, SpaceProperties.yHeight + DyHeight, SpaceProperties.Xpxu + DXpxs, SpaceProperties.Ypxu + DYpxs);
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

void RenderSpace::SetDefaultBackFrontTextures()
{
	RS_Framebuffer->backTexture->TexObj = RenderAux::GenerateTexture("_defaultOutput", SpaceProperties.Xpxu, SpaceProperties.Ypxu);
	RS_Framebuffer->backTexture->Name = "_defaultOutput";
	RS_Framebuffer->frontTexture->TexObj = RenderAux::GenerateTexture("_LastFrame", SpaceProperties.Xpxu, SpaceProperties.Ypxu);
	RS_Framebuffer->frontTexture->Name = "_LastFrame";
	RS_Framebuffer->bHasFrontBackTex = true;
}

void RenderSpace::SetBackFrontTextures(RenderAux::Texture* Back_TextureOut, RenderAux::Texture* Front_TextureIn)
{
	RS_Framebuffer->bHasFrontBackTex = true;

	if (Back_TextureOut != nullptr) {
		RS_Framebuffer->backTexture->TexObj = Back_TextureOut;
		RS_Framebuffer->backTexture->Name = Back_TextureOut->Name;
	}
	else {
		RS_Framebuffer->bHasFrontBackTex = false;
	}

	if (Front_TextureIn != nullptr) {
		RS_Framebuffer->frontTexture->TexObj = Front_TextureIn;
		RS_Framebuffer->frontTexture->Name = Front_TextureIn->Name;
	}
	else {
		RS_Framebuffer->bHasFrontBackTex = false;
	}

}

RenderAux::RenderQuadProperties RenderSpace::GetSpaceProperties()
{
	return SpaceProperties;
}

void RenderSpace::LinkShaderPrograms(GLuint RenderProgram, GLuint DisplayProgram, GLuint ComputeProgram)
{
	RS_Framebuffer->defaultComputeProgram = ComputeProgram;
	RS_Framebuffer->defaultRenderProgram = RenderProgram;
	RS_Framebuffer->defaultDisplayProgram = DisplayProgram;
}

void RenderSpace::LinkOutputToTexture(std::string OutputName, RenderAux::Texture* TexObj) {
	RenderAux::TextureOutput* Tout = new RenderAux::TextureOutput;
	Tout->TexObj = TexObj;
	Tout->Name = OutputName;
	RS_Framebuffer->TextureOutputs.push_back(Tout);
}

void RenderSpace::LinkInputTexture(RenderAux::Texture* TexObj) {
	RS_Framebuffer->Textures.push_back(TexObj);
}

void RenderSpace::LinkComputeBlock(RenderAux::ComputeBlock* ComputeBlockObj)
{
	RS_Framebuffer->SSBOs.push_back(ComputeBlockObj);
}

void RenderSpace::ExecuteComputeStage()
{
	if (RS_Framebuffer->bFramebufferInitialized && !RS_Framebuffer->bSkipcomputeCall) {
		//FRAMEBUFFER
		RenderAux::BindFramebuffer(RS_Framebuffer->ID);

		if (RS_Framebuffer->defaultComputeProgram) {
			//SSBOs
			for (GLuint i = 0; i < RS_Framebuffer->SSBOs.size(); i++)
				RenderAux::BindShaderStorageBufferObject(RS_Framebuffer->SSBOs[i]->ID, RS_Framebuffer->SSBOs[i]->Length, RS_Framebuffer->SSBOs[i]->UnitSize, i);
			
			//TEX
			if(RS_Framebuffer->bHasFrontBackTex)RS_Framebuffer->TextureOutputs.push_back(RS_Framebuffer->backTexture);
			for (GLuint i = 0; i < RS_Framebuffer->TextureOutputs.size(); i++)
				RenderAux::BindImageFromTextureToUnit(RS_Framebuffer->TextureOutputs[i]->TexObj->ID, i, GL_READ_WRITE, GL_RGBA32F);
			if (RS_Framebuffer->bHasFrontBackTex)RS_Framebuffer->TextureOutputs.pop_back();

			RenderAux::UseComputeProgram(RS_Framebuffer->defaultComputeProgram, Num_Groups_x, Work_Group_Size_x, Num_Groups_y, Work_Group_Size_y, Num_Groups_z, Work_Group_Size_z);
		}
	}
}

void RenderSpace::ExecuteRenderStage()
{
	if (RS_Framebuffer->bFramebufferInitialized && RS_Framebuffer->bHasFrontBackTex && !RS_Framebuffer->bSkipDrawCall) {
		//FRAMEBUFFER	
		RenderAux::BindFramebuffer(RS_Framebuffer->ID);
		RenderAux::UseRenderProgram(RS_Framebuffer->defaultRenderProgram);
			
		//RENDER TARGETS - OUTPUT
		RS_Framebuffer->TextureOutputs.push_back(RS_Framebuffer->backTexture);
		RenderAux::BindMultipleRenderTargetsToFramebuffer(RS_Framebuffer->ID, &getTextureOutputs()[0], RS_Framebuffer->TextureOutputs.size());
		for (GLuint Atrib_ID = 0; Atrib_ID < RS_Framebuffer->TextureOutputs.size(); Atrib_ID++)
			RenderAux::BindAtributeName(Atrib_ID, RS_Framebuffer->defaultRenderProgram, RS_Framebuffer->TextureOutputs[Atrib_ID]->Name);
		RS_Framebuffer->TextureOutputs.pop_back();

		//(!)
		//UNIFORMS - INPUT
		RenderAux::BindTextureUnitToUniformName(RS_Framebuffer->frontTexture->TexObj->ID, 0, RS_Framebuffer->defaultRenderProgram, RS_Framebuffer->frontTexture->Name);
		for (GLuint TexObj_ID = 1; TexObj_ID < RS_Framebuffer->Textures.size() + 1; TexObj_ID++)
			RenderAux::BindTextureUnitToUniformName(RS_Framebuffer->Textures[TexObj_ID - 1]->ID, TexObj_ID, RS_Framebuffer->defaultRenderProgram, RS_Framebuffer->Textures[TexObj_ID - 1]->Name);

		//UNIFORMS - DEFAULT
		GLfloat* pxpu = new GLfloat[2]{ 1.0f / GLfloat(SpaceProperties.Xpxu), 1.0f / GLfloat(SpaceProperties.Ypxu) };
		glUniform2fv(glGetUniformLocation(RS_Framebuffer->defaultRenderProgram, "_oneDivPxpu"),1, &pxpu[0]);
		glUniform1ui(glGetUniformLocation(RS_Framebuffer->defaultRenderProgram, "_t"), time);

		//RENDER
		RenderAux::RenderQuad(0, 0, SpaceProperties.Xpxu, SpaceProperties.Ypxu, true);
		time++;
		RenderAux::SwapTexIDs(RS_Framebuffer->backTexture->TexObj->ID, RS_Framebuffer->frontTexture->TexObj->ID);
		//std::cout << "t: " << time << std::endl;
		delete[] pxpu;
	}
}

void RenderSpace::ExecuteDisplayStage()
{
	if (RS_Framebuffer->bFramebufferInitialized) {
		if (RS_Framebuffer->bHasFrontBackTex) {
			RenderAux::BindFramebuffer(0);
			RenderAux::UseRenderProgram(RS_Framebuffer->defaultDisplayProgram);
			RenderAux::BindTextureUnitToUniformName(RS_Framebuffer->frontTexture->TexObj->ID, 0, RS_Framebuffer->defaultDisplayProgram, RS_Framebuffer->frontTexture->TexObj->Name);
			RenderAux::RenderQuad(SpaceProperties.xPos, SpaceProperties.yPos, SpaceProperties.xWidth, SpaceProperties.yHeight, true);
		}
	}
}


RenderSpace::~RenderSpace()
{
	delete RS_Framebuffer->frontTexture->TexObj;
	delete RS_Framebuffer->frontTexture;
	delete RS_Framebuffer->backTexture->TexObj;
	delete RS_Framebuffer->backTexture;
	
	for (auto T : RS_Framebuffer->Textures) delete T;
	for (auto T : RS_Framebuffer->TextureOutputs) delete T;
	for (auto SO : RS_Framebuffer->SSBOs) delete SO;
	delete RS_Framebuffer;
}

