#pragma once
#include <array>
#include <GL\glew.h>
#include <GLFW/glfw3.h>
#include <functional>
#include "RenderSpace.h"
 

static class WindowManager
{
public:
	WindowManager();
	WindowManager(GLuint xRes, GLuint yRes, std::string WindowName, GLuint ActiveMonitor, bool isFullscreen = false, bool isVsyncON = false);
	~WindowManager();

	GLFWwindow* Window;
	int InitializeWindow();
	void LinkRenderSpaces(std::initializer_list<RenderSpace*> RSpace);
	void LinkRenderSpace(RenderSpace* RSpace);
	int DrawLinkedRenderSpaces(std::function<void()> lambda);
	int DrawLinkedRenderSpaces();
	std::vector<RenderSpace*>RenderSpaceList;
	GLuint xRes = 1024;
	GLuint yRes = 768;
	bool bFullscreen = false;
	bool bVsyncON = false;
	GLuint ActiveMonitor = 0;
	std::string WindowName;
};

