#include "WindowManager.h"

WindowManager::WindowManager(GLuint xRes, GLuint yRes, std::string WindowName, bool isFullscreen, bool isVsyncON)
{
	this->xRes = xRes;
	this->yRes = yRes;
	this->WindowName = WindowName;
	this->bFullscreen = isFullscreen;
	this->bVsyncON = isVsyncON;
}

int WindowManager::InitializeWindow() {
	/* Initialize the library */
	if (!glfwInit()) {
		return -1;
	}

	///* Create a windowed mode window and its OpenGL context */
	if (bFullscreen) {
		Window = glfwCreateWindow(xRes, yRes, WindowName.c_str(), glfwGetPrimaryMonitor(), NULL);
	}
	else {
		Window = glfwCreateWindow(xRes, yRes, WindowName.c_str(), NULL, NULL);
	}

	if (!Window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(Window);
	glfwSwapInterval(bVsyncON);
	
	check_gl_error_on();
	//check_gl_error_off();
	/*Initialize GLEW Library*/
	glewInit();

	return 0;
}


//std::array<unsigned char, 4> pixel = { 255, 255, 255, 255 };
unsigned char pixel[4] = { 255, 255, 255, 0 };
bool stepOK = false;
int WindowManager::DrawLinkedRenderSpaces()
{

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(Window))
	{
		
		//std::cout << RenderAux::FPS_counter(glfwGetTime()) << std::endl;
		//RenderAux::FPS_lock(120);
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.1, 0.2, 0.3, 1);

		double x, y;
		int LMB, RMB;

		RMB = glfwGetMouseButton(Window, GLFW_MOUSE_BUTTON_RIGHT);
		LMB = glfwGetMouseButton(Window, GLFW_MOUSE_BUTTON_LEFT);
		glfwGetCursorPos(Window, &x, &y);

		if (RMB == GLFW_PRESS) {
			stepOK = true;
		}
		if (RMB == GLFW_RELEASE && stepOK) {
			for (RenderSpace* F : RenderSpaceList) {
				F->ExecuteComputeStage();
				F->ExecuteRenderStage();
				check_gl_error();
			}
			stepOK = false;
		}
		for (RenderSpace* F : RenderSpaceList) { 
			F->ExecuteDisplayStage();
			check_gl_error();
		}

		if (glfwGetKey(Window, GLFW_KEY_1) == GLFW_PRESS) {
			unsigned char Npixel[4] = { 0, 0, 0, 0 };
			memcpy(pixel, Npixel, sizeof(pixel));
		}

		if (glfwGetKey(Window, GLFW_KEY_2) == GLFW_PRESS) {
			unsigned char Npixel[4] = { 255, 255, 255, 0 };
			memcpy(pixel, Npixel, sizeof(pixel));
		}

		if (glfwGetKey(Window, GLFW_KEY_3) == GLFW_PRESS) {
			unsigned char Npixel[4] = { 255, 0, 0, 0 };
			memcpy(pixel, Npixel, sizeof(pixel));
		}

		if (glfwGetKey(Window, GLFW_KEY_4) == GLFW_PRESS) {
			unsigned char Npixel[4] = {0 , 255, 0, 0 };
			memcpy(pixel, Npixel, sizeof(pixel));
		}

		if (glfwGetKey(Window, GLFW_KEY_5) == GLFW_PRESS) {
			unsigned char Npixel[4] = { 0 , 0, 255, 0 };
			memcpy(pixel, Npixel, sizeof(pixel));
		}

		if (LMB == GLFW_PRESS) {
			
			int wx, wy;
			glfwGetWindowSize(Window, &wx, &wy);
			for (RenderSpace* F : RenderSpaceList) { 
				F->poke(x, y, 1, 1, wx, wy, pixel, GL_RGBA, GL_UNSIGNED_BYTE, F->RS_Framebuffer->frontTexture->TexObj->ID);
			}
		}

		/* Swap front and back buffers */
		glfwSwapBuffers(Window);

		/* Poll for and process events */
		glfwPollEvents();
	}


	/* Terminate the library */
	glfwTerminate();

	return 0;
}

void WindowManager::LinkRenderSpace(RenderSpace * RSpace)
{
	RenderSpaceList.push_back(RSpace);
}

WindowManager::~WindowManager()
{

}
