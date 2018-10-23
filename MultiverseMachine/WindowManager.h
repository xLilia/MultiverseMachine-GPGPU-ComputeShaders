#pragma once
#include <array>
#include <GL\glew.h>
#include <GLFW/glfw3.h>
#include "RenderSpace.h"
 

static class WindowManager
{
public:
	WindowManager(){};
	WindowManager(GLuint xRes, GLuint yRes, std::string WindowName, bool isFullscreen = false, bool isVsyncON = false);
	~WindowManager();

	GLFWwindow* Window;
	int InitializeWindow();
	void LinkRenderSpace(RenderSpace* RSpace);
	int DrawLinkedRenderSpaces();
	std::vector<RenderSpace*>RenderSpaceList;
	GLuint xRes = 1024;
	GLuint yRes = 768;
	bool bFullscreen = false;
	bool bVsyncON = false;
	std::string WindowName;
};

