#include <GL\glew.h>
#include <GLFW/glfw3.h>
#include"WindowManager.h"
#include "ShaderWizard.h"
int main(void)
{

	WindowManager M;
	M.InitializeWindow();


	RenderSpace* S1 = new RenderSpace;
	ShaderWizard Shwiz;
	Shwiz.installShaders("VertexShader.glsl", "FragmentShader.glsl");
	Shwiz.installShaders("VertexShader.glsl", "CopyFShader.glsl");
	Shwiz.installShaders(NULL,NULL,"ComputeShader.glsl");
	//Shwiz.installShaders("VertexShader.glsl", "FragmentShader2.glsl");

	struct vec4 {
		float x, y, z, w;
	};

	vec4* pos = new vec4[3]{
		vec4{1,0,0,0},
		vec4{0,1,0,0},
		vec4{0,0,1,0}
	};

	S1->defaultDisplayProgram = Shwiz.InstlledProgramIDs[0];
	S1->defaultSwapProgram = Shwiz.InstlledProgramIDs[1];
	S1->defaultComputeProgram = Shwiz.InstlledProgramIDs[2];
	S1->SetPixelDimentions(0, 0, M.xRes/2, M.xRes / 2, 32);
	S1->SetComputeGroups(1,3);
	S1->LinkOutputToTexture("out1", S1->createInputTexture("in1"));
	S1->createStorageBufferObject(3, pos);
	S1->InitializeFramebuffer();
	
	M.LinkRenderSpace(S1);
	M.DrawLinkedRenderSpaces();

	return 0;
}