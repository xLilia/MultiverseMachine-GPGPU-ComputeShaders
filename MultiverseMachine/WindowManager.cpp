#include "WindowManager.h"

WindowManager::WindowManager()
{

}

WindowManager::WindowManager(GLuint xRes, GLuint yRes, std::string WindowName, GLuint ActiveMonitor, bool isFullscreen, bool isVsyncON)
{
	this->xRes = xRes;
	this->yRes = yRes;
	this->WindowName = WindowName;
	this->bFullscreen = isFullscreen;
	this->bVsyncON = isVsyncON;
	this->ActiveMonitor = ActiveMonitor;
}

int WindowManager::InitializeWindow() {
	/* Initialize the library */
	if (!glfwInit()) {
		return -1;
	}

	if (bVsyncON) {
		glfwWindowHint(GLFW_DOUBLEBUFFER, GL_FALSE);
	}
	else {
		glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);
	}

	///* Create a windowed mode window and its OpenGL context */
	if (bFullscreen) {
		int* ActiveMonitorsNum = new int;
		Window = glfwCreateWindow(xRes, yRes, WindowName.c_str(), glfwGetMonitors(ActiveMonitorsNum)[ActiveMonitor%*ActiveMonitorsNum], NULL);
		delete ActiveMonitorsNum;
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
	
	//check_gl_error_on();
	check_gl_error_off();
	/*Initialize GLEW Library*/
	glewInit();

	return 0;
}


//std::array<unsigned char, 4> pixel = { 255, 255, 255, 255 };
unsigned char pixel[4] = { 255, 255, 255, 0 };
bool stepOK = false;
int WindowManager::DrawLinkedRenderSpaces()
{
	for (RenderSpace* F : RenderSpaceList) {
		F->ExecuteComputeStage();
		F->ExecuteRenderStage();
		check_gl_error();
	}
	for (RenderSpace* F : RenderSpaceList) {
		F->ExecuteDisplayStage();
		check_gl_error();
	}
	/* Swap front and back buffers */
	if (bVsyncON) {
		glfwSwapBuffers(Window);
	}
	else {
		glFlush();
	}
	
	/* Poll for and process events */
	glfwPollEvents();

	return 0;
}

void WindowManager::LinkRenderSpaces(std::initializer_list<RenderSpace*> RSpace)
{
	for(auto rs : RSpace)
	RenderSpaceList.push_back(rs);
}

void WindowManager::LinkRenderSpace(RenderSpace* RSpace)
{
	RenderSpaceList.push_back(RSpace);
}

int WindowManager::DrawLinkedRenderSpaces(std::function<void()> lambda)
{
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(Window))
	{
		lambda();
		DrawLinkedRenderSpaces();
	}
	/* Terminate the library */
	glfwTerminate();
	return 0;
}

WindowManager::~WindowManager()
{

}
