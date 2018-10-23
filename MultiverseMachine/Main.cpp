#include <GL\glew.h>
#include <GLFW/glfw3.h>
#include"WindowManager.h"
#include "ShaderWizard.h"
int main(void)
{

	WindowManager M(1920,1080, "AAA", true, false);
	M.InitializeWindow();


	RenderSpace* S1 = new RenderSpace("Space1");
	ShaderWizard Shwiz;
	Shwiz.installShaders("VertexShader.glsl", "FragmentShader.glsl");
	Shwiz.installShaders("VertexShader.glsl", "DisplayShader.glsl");
	Shwiz.installShaders(NULL,NULL,"ComputeShader.glsl");

	//Shwiz.installShaders("VertexShader.glsl", "FragmentShader2.glsl");
	
	struct vec4 {
		float x, y, z, w;
	};

	std::vector<vec4>C1;
	for (int i = 0; i < 100 * 100; i++) {
		//float r = std::sin(std::rand());
		//float g = std::sin(std::rand());
		//float b = std::sin(std::rand()); 
		C1.push_back(vec4{ 0.5,0.5,0.5 });
	}

	std::vector<vec4>C2;
	for (int i = 0; i < 100 * 100; i++) {
		//float r = std::sin(std::rand());
		//float g = std::sin(std::rand());
		//float b = std::sin(std::rand()); 
		C2.push_back(vec4{ 0.0,0.0,0.0 });
	}

	

	//vec4* P = new vec4[3];
	//P[0] = vec4{ 1,0,0,0 };
	//P[1] = vec4{ 0,1,0,0 };
	//P[2] = vec4{ 0,0,1,0 };

	
	S1->LinkShaderPrograms(Shwiz.InstlledProgramIDs[0], Shwiz.InstlledProgramIDs[1], Shwiz.InstlledProgramIDs[2]);
	//S1->LinkShaderPrograms(Shwiz.InstlledProgramIDs[0], Shwiz.InstlledProgramIDs[1]);
	RenderAux::PrintComputeWorkGroupStats();
	S1->SetComputeGroups(100, 1, 100, 1);
	S1->LinkComputeBlock(S1->createStorageBufferObject(10000, &C1[0]));
	RenderSpace::ComputeBlock* Cb2 = S1->createStorageBufferObject(10000, &C2[0]);
	S1->LinkComputeBlock(Cb2);

	S1->SetTexturePixelDimention(0, 0, 1080, 1080, 100);
	S1->SetBackFrontTextures(S1->GenerateTexture("_defaultOutput"), S1->GenerateTexture("_LastFrame"));

	RenderSpace::Texture* t1 = S1->GenerateTexture("in1");
	S1->LinkInputTexture(t1);
	S1->LinkOutputToTexture("out1", t1);

	S1->InitializeRenderSpaceFramebuffer();

	M.LinkRenderSpace(S1);
	M.DrawLinkedRenderSpaces();
	
	//vec4* vals2 = new vec4[10000];
	RenderAux::FetchShaderStorageBufferObject(Cb2->Length, 1, Cb2->ID, &C2[0]);
	C2;
	return 0;
}

//Physarum polycephalum