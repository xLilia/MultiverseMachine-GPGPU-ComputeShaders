#include <GL\glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "WindowManager.h"
#include "ShaderWizard.h"
#include "EntityManager.h"
int main(void)
{

	WindowManager WinM(1920, 1080, "MultiverseA", 0, false, true);
	WinM.InitializeWindow();


	RenderSpace* SimulationDisplay = new RenderSpace("Space1");
	ShaderWizard Shwiz;
	Shwiz.installShaders("VertexShader.glsl", "DisplayShader.glsl");
	Shwiz.installShaders("VertexShader.glsl", "FragmentShader.glsl");
	Shwiz.installShaders("VertexShader.glsl", "FragmentShader2.glsl");
	Shwiz.installShaders(NULL,NULL,"ComputeShader.glsl");

	//---EntityManagers---
	float n = 15000;
	EntityManager ParticleGroup1;
	for (size_t x = 0; x < n; x++)
	{
		float col = sin(rand());
		ParticleGroup1.addEntity(EntityManager::MakeEntity(abs(sin(rand())*1920), abs(sin(rand()) * 1080), sin(rand()), sin(rand()), x/n, sin(rand()), sin(rand()), sin(rand())));
	}
	ParticleGroup1.MakeEntitySSBO();

	//----SimulationDisplay----
	//Setup RenderSpace1 Framebuffer
	SimulationDisplay->LinkShaderPrograms(Shwiz.InstlledProgramIDs[2], Shwiz.InstlledProgramIDs[0], Shwiz.InstlledProgramIDs[3]);
	SimulationDisplay->SetPixelSpaceProperties(0, 0, 1920, 1080, 1920);
	RenderAux::Texture* ParticlesTex = RenderAux::GenerateTexture("in_ParticlesTex", SimulationDisplay->GetSpaceProperties().Xpxu, SimulationDisplay->GetSpaceProperties().Ypxu);
	//RenderAux::Texture* ParticlesDataTex = SimulationDisplay->GenerateTexture("out_ParticlesDataTex");
	SimulationDisplay->LinkInputTexture(ParticlesTex);
	SimulationDisplay->LinkOutputToTexture("out_ParticlesTex", ParticlesTex);
	SimulationDisplay->SetDefaultBackFrontTextures();
	//Setup Compute Shader
	RenderAux::PrintComputeWorkGroupStats();
	SimulationDisplay->SetComputeGroups(n,1);
	SimulationDisplay->LinkComputeBlock(ParticleGroup1.EntitySSBO);
	//InitializeRenderSpace
	SimulationDisplay->InitializeRenderSpaceFramebuffer();
	
	WinM.LinkRenderSpaces({SimulationDisplay}); 
	WinM.DrawLinkedRenderSpaces([WinM,SimulationDisplay]()->void {

		//std::array<unsigned char, 4> pixel = { 255, 255, 255, 255 };
		unsigned char pixel[4] = { 255, 255, 255, 0 };
		//SimulationDisplay->SetDeltaPixelSpaceProperties(0, 0, 0, 0, 0, 0);

		//std::cout << RenderAux::FPS_counter(glfwGetTime()) << std::endl;
		//RenderAux::FPS_lock(120);
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.1, 0.2, 0.3, 1);

		double x, y;
		int LMB, RMB;

		//GET INPUT VALS
		RMB = glfwGetMouseButton(WinM.Window, GLFW_MOUSE_BUTTON_RIGHT);
		LMB = glfwGetMouseButton(WinM.Window, GLFW_MOUSE_BUTTON_LEFT);
		glfwGetCursorPos(WinM.Window, &x, &y);

		//INPUT LOGIC
		if (RMB == GLFW_PRESS) {
			SimulationDisplay->RS_Framebuffer->bSkipDrawCall = true;
			SimulationDisplay->RS_Framebuffer->bSkipcomputeCall = true;
		}
		if (RMB == GLFW_RELEASE ) {
			SimulationDisplay->RS_Framebuffer->bSkipDrawCall = false;
			SimulationDisplay->RS_Framebuffer->bSkipcomputeCall = false;
		}
	

		if (glfwGetKey(WinM.Window, GLFW_KEY_1) == GLFW_PRESS) {
			unsigned char Npixel[4] = { 0, 0, 0, 0 };
			memcpy(pixel, Npixel, sizeof(pixel));
		}

		if (glfwGetKey(WinM.Window, GLFW_KEY_2) == GLFW_PRESS) {
			unsigned char Npixel[4] = { 255, 255, 255, 0 };
			memcpy(pixel, Npixel, sizeof(pixel));
		}

		if (glfwGetKey(WinM.Window, GLFW_KEY_3) == GLFW_PRESS) {
			unsigned char Npixel[4] = { 255, 0, 0, 0 };
			memcpy(pixel, Npixel, sizeof(pixel));
		}

		if (glfwGetKey(WinM.Window, GLFW_KEY_4) == GLFW_PRESS) {
			unsigned char Npixel[4] = { 0 , 255, 0, 0 };
			memcpy(pixel, Npixel, sizeof(pixel));
		}

		if (glfwGetKey(WinM.Window, GLFW_KEY_5) == GLFW_PRESS) {
			unsigned char Npixel[4] = { 0 , 0, 255, 0 };
			memcpy(pixel, Npixel, sizeof(pixel));
		}

		if (LMB == GLFW_PRESS) {

			int wx, wy;
			glfwGetWindowSize(WinM.Window, &wx, &wy);
			if (SimulationDisplay->RS_Framebuffer->bHasFrontBackTex) SimulationDisplay->poke(x, y, 1, 1, wx, wy, pixel, GL_RGBA, GL_UNSIGNED_BYTE, SimulationDisplay->RS_Framebuffer->frontTexture->TexObj->ID);
			
		}
	});
	
	//vec4* vals2 = new vec4[10000];
	//RenderAux::FetchShaderStorageBufferObject(Cb2->Length, 1, Cb2->ID, &C2[0]);
	//C2;
	return 0;
}

//Physarum polycephalum