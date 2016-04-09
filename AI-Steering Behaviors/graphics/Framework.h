/* Start Header *****************************************************************/
/*!
\file Framework.h
\author Camilo Diaz
\e-mail c.diaz@digipen.edu
\date 9/24/2015
\Brief Copyright (C) 2015 DigiPen Institute of Technology. Reproduction or
disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
*/
/* End Header *******************************************************************/
#pragma once
#include "OpenglGL.h"

#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#include <GLFW/glfw3native.h>

#include "MasterRenderer.h"
#include "Common.hpp"
#include "Shader.h"
#include "RawModel.h"
#include "TexturedModel.h"
#include <map>
#include "imgui.h"

class Framework
{
public:
	static Framework* GetInstance();
	
	~Framework();
	void Init();
	void Run();
	IMGUI_API void			Shutdown();
	IMGUI_API bool        ImGui_Init(bool install_callbacks);
	IMGUI_API bool			CreateDeviceObjects();

	void KeyPressedEvent(int key, int action);
	void MouseMoveEvent(double  posx, double posy);
	bool running;


	MasterRenderer* GetRendered();
	

private:
	Framework();

	IMGUI_API void   ImGui_NewFrame();

	void InitDebugGui();	

	void InitCamera();
	void InitModels();
	void InitTerrain();
	static void TwEventMouseButtonGLFW3(GLFWwindow* window, int button, int action, int mods);
	static void TwEventMouseMoveGLFW3(GLFWwindow* window, double xpos, double ypos);
	static void TwEventMouseScrollGLFW3(GLFWwindow* window, double xoffset, double yoffset);
	static void TwEventKeyCallGLFW3(GLFWwindow*, int key, int, int action, int mods);
	void CreateFontsTexture();
	static Framework* instance;
	
	MasterRenderer* masterRendered;
	
	float fps;

	void DebugWindow();
	map<TexturedModel*, vector<Entity*>, TexturedModelCompare> entities;
	vector<Terrain*> terrains;
	int itemCombo = 0;
};

