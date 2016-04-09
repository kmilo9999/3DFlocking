

#include "Framework.h"
#include <stdlib.h>
#include <stdio.h>
#include "Shader.h"
#include "Loader.h"
#include "PhongShading.h"
#include "Simpletechnique.h"
#include "TerrainShader.h"
#include <windows.h>
#include "GoalEntity.h"
#include "Flock.h"



#define  TIME_STEP 1.0f/60.0f

static double       g_Time = 0.0f;
static bool         g_MousePressed[3] = { false, false, false };
static float        g_MouseWheel = 0.0f;
static GLuint       g_FontTexture = 0;
static int          g_ShaderHandle = 0, g_VertHandle = 0, g_FragHandle = 0;
static int          g_AttribLocationTex = 0, g_AttribLocationProjMtx = 0;
static int          g_AttribLocationPosition = 0, g_AttribLocationUV = 0, g_AttribLocationColor = 0;
static unsigned int g_VboHandle = 0, g_VaoHandle = 0, g_ElementsHandle = 0;


Framework* Framework::instance(0);

/*
	Singlenton Intance of framework class
*/
Framework* Framework::GetInstance()
{
	if (!instance)
	{
		instance = new Framework();
	}
	return instance;
}

/*
  Contructor of the class Framework
*/
Framework::Framework()
{

	masterRendered = new MasterRenderer(vec2(854, 480));
}

/*
	Contructor of the class Framework
*/
Framework::~Framework()
{
	if (masterRendered)
	{
		delete masterRendered;
	}

	for (std::map<TexturedModel*, vector<Entity*>, TexturedModelCompare>::iterator itr = entities.begin(); 
		itr != entities.end(); itr++)
	{
		vector<Entity*> entis = itr->second;
		for (size_t i = 0; i < entis.size(); ++i)
		{
			delete entis[i];
		}
		delete[] itr->first;
		entities.erase(itr);
	}

	if (instance)
	{
		delete instance;
	}
}

/*
	Initialize all the components of the scene:
	Curves
	Plane
	SkinnedMeshes
	Camera
	Shaders
	Gui

*/
void Framework::Init()
{
	
	masterRendered->Init();
	Light* globalLight = new Light(vec3(-5.0f, 10.0f, 5.0f), vec3(0.3f, 0.3f, 0.3f), vec3(1.0f, 1.0f, 1.0f), vec3(1.0f, 1.0f, 1.0f));
	ShadingTechnique* shader = new Simpletechnique();
	globalLight->GetModel()->SetShader(shader);
	masterRendered->SetGlobalLight(globalLight);

	InitModels();
	InitTerrain();
	InitDebugGui();

	
	
}

/*
	Run the Game Loop where updates all the componentes of the scene
*/
void Framework::Run()
{
	
	running = true;

	double timeaccumulator = 0;
	double frameCounter = 0.0f;
	double prevTime = glfwGetTime();
	ImVec4 clear_color = ImColor(114, 144, 154);
	bool show_test_window = true;
	bool show_another_window = false;
	/* Loop until the user closes the window */
	glfwSwapInterval(1);
	while (running )
	{
		
		//Handle window resize
		{
			GLsizei width, height;
			glfwGetWindowSize(masterRendered->GetWindow(), &width, &height);
			glViewport(0, 0, width, height);

		}
		
		{
			if (glfwWindowShouldClose(masterRendered->GetWindow()))
			{
				running = false;
			}
		}
		
		ImGui_NewFrame();

		DebugWindow();
		


		double currentTime = glfwGetTime();
		double dt = currentTime - prevTime;
		prevTime = currentTime;
		


		timeaccumulator += dt;
		masterRendered->Update(entities,(float)dt);

		masterRendered->PrepareRenderer();

		masterRendered->RenderEntities(entities);
		masterRendered->RenderTerrains(terrains);




		ImGui::Render();

		masterRendered->FinishRenderer();

		frameCounter += 1;
		
		if (timeaccumulator > 1.0f)
		{
			fps = (float)(frameCounter / timeaccumulator );
			timeaccumulator -= 1.0f;
			frameCounter = 0.0f;
		}		
		
		
		//TwDraw();
		

	}
	Shutdown();
	glfwDestroyWindow(masterRendered->GetWindow());
	
	//TwTerminate();
	
}




/*
	Initialize the Virtual Camera
*/
void Framework::InitCamera()
{
	/*Camera* camera = new Camera();
	
    camera->Transform.Position = Vector3(3.33333f, 67.8721f, 37.3296f);
		  camera->Transform.Orientation = Quaternion(-0.456822f, -0.00697113f, 0.00562094f, 0.889513f);
	camera->LookAt(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f));
	camera->FieldOfView = 45;

	render->SetCamera(camera);*/
}

void Framework::InitModels()
{

	
	//---  Sky Dome
	vector<Entity*> skydomes;
	Entity* skydome = new Entity();
	skydome->SetPosition(vec3(0.0f, 0.0f, 0.0f));
	skydome->SetScale(vec3(28.0f, 28.0f, 28.0f));
	skydomes.push_back(skydome);

	Texture* skyDometexture = new Texture(GL_TEXTURE_2D, "../Resources/Textures/SkyDome.png");
	skyDometexture->type = "diffuse";
	skyDometexture->Load();

	vector<TexturedModel*> domes = Loader::GetInstance()->LoadToFromFile("../Resources/Models/dome.obj");
	ShadingTechnique* simpleShader = new TerrainShader(masterRendered->GetGlobalLight(),1.0f);

	TexturedModel* domeModel = domes[0];
	domeModel->Addtexture(skyDometexture);
	domeModel->SetShader(simpleShader);
	domeModel->SetId("skyDome");
	//-- End of Sky Dome


	//-- Trees
	vector<TexturedModel*> treeMesh = Loader::GetInstance()->LoadToFromFile("../Resources/Models/mytree.obj");
	vector<Entity*> trees;
	
	float TreePositionZ = -10.0f;
	for (int i = 0; i < 5; ++i)
	{
		float TreePositionX = 3.0f;
		for (int j = 0; j < 10; ++j)
		{
			Entity* tree = new Entity();
			float positionX = (rand() % 40) + (-20);
			float positionZ = (rand() % 40) + (-20);
			tree->SetPosition(vec3(positionX, 0, positionZ));
			trees.push_back(tree);
		}
	}
	

	//-- End of Trees
	
	vector<TexturedModel*> cube = Loader::GetInstance()->LoadToFromFile("../Resources/Models/cube.obj");

	Texture* targetTtexture = new Texture(GL_TEXTURE_2D, "../Resources/Textures/target.png");
	targetTtexture->type = "diffuse";
	targetTtexture->Load();

	Texture* Boidtexture = new Texture(GL_TEXTURE_2D, "../Resources/Textures/pyramid.png");
	Boidtexture->type = "diffuse";
	Boidtexture->Load();

	vector<Entity*> targets;
	Entity* target = new GoalEntity();
	target->SetPosition(vec3(0.0f, 3.0f, 0.0f));
	target->SetScale(vec3(0.2f, 0.2f, 0.2f));
	targets.push_back(target);

	vector<TexturedModel*> piramid = Loader::GetInstance()->LoadToFromFile("../Resources/Models/pyramid2.obj");
	vector<Entity*> piramids;
	
	bool leader = true;
	

	float positionZ = -10.0f;
	for (int z = 0; z < 1; ++z)
	{
		float positionY = 3.0f;
		for (int i = 0; i < 10; ++i)
		{
			float positionX = -5.0f;
			for (int j = 0; j < 10; ++j)
			{

				Agent* entityPiramid = new Agent();
				entityPiramid->SetPosition(vec3(positionX, positionY, positionZ));
				//entityPiramid->SetPosition(vec3(positionX, 5, 0.0f));
				int randomAngle = (rand() % 360);
				int randomAxis = (rand() % 4) + 1;
				vec3 axis;
				switch (randomAxis)
				{
				case 1:
					axis = vec3(1, 0, 0);
					break;
				case 2:
					axis = vec3(0, 1, 0);
					break;
				case 3:
					axis = vec3(0, 0, 1);
					break;
				default:
					break;
				}
				entityPiramid->SetOrientation(randomAngle, axis);
				entityPiramid->SetScale(vec3(0.1f, 0.1f, 0.1f));
				entityPiramid->goal = target;
				entityPiramid->SetVelocity(vec3(0, 0, 1));
				entityPiramid->SetIsLeader(leader);
				leader = false;
				piramids.push_back(entityPiramid);

				positionX += 2.5f;
			}
			positionY += 1.5f;
		}
		positionZ += 2.5;
	}
	

	
	
	ShadingTechnique* phong = new PhongShading(masterRendered->GetGlobalLight());
	
	
	TexturedModel* piramidModel = piramid[0];
	piramidModel->Addtexture(Boidtexture);
	piramidModel->SetId("pyramid");
	piramidModel->SetShader(simpleShader);

	TexturedModel* cubeModel = cube[0];
	cubeModel->Addtexture(targetTtexture);
	cubeModel->SetId("cube");
	cubeModel->SetShader(simpleShader);
	

	TexturedModel* treeModel = treeMesh[0];
	treeModel->SetId("tree");
	treeModel->SetShader(phong);


	entities.insert(std::pair <TexturedModel*, vector<Entity*>>(piramidModel, piramids));
	entities.insert(std::pair <TexturedModel*, vector<Entity*>>(cubeModel, targets));
	entities.insert(std::pair <TexturedModel*, vector<Entity*>>(domeModel, skydomes));
	entities.insert(std::pair <TexturedModel*, vector<Entity*>>(treeModel, trees));

	
	
}

void Framework::InitTerrain()
{
	Terrain* terrain = new Terrain(0.0f, 0.0f,"../Resources/Textures/heightmap.png");
	
	ShadingTechnique* terrainshading = new TerrainShader(masterRendered->GetGlobalLight(),20.0f);
	
	terrain->GetModel()->SetShader(terrainshading);
	terrains.push_back(terrain);
	
	
}



/*
	Update teh state of the Keys 
*/
void Framework::KeyPressedEvent(int key, int action)
{
	
}

/*
	Update the position of the mouse in the engine
*/
void Framework::MouseMoveEvent(double  posx, double posy)
{
	//render->GetCamera()->posX = posx;
	//render->GetCamera()->posY = posy;
}

/*
	Return a Render Instance
*/
MasterRenderer* Framework::GetRendered()
{
	return masterRendered;
}



/*
	Callback function for antweekbar
*/
void Framework::TwEventMouseButtonGLFW3(GLFWwindow* window, int button, int action, int mods)
{ 
	
	g_MousePressed[button] = true;
	
	InputHandler::GetInstance()->HandleMouseButtonInput(button, action);

	
	
}

/*
Callback function for antweekbar
*/
void Framework::TwEventMouseMoveGLFW3(GLFWwindow* window, double xpos, double ypos)
{

	
	InputHandler::GetInstance()->HandleMouseMovement(xpos, ypos);

}

void Framework::TwEventMouseScrollGLFW3(GLFWwindow* window, double xoffset, double yoffset)
{
	g_MouseWheel += (float)yoffset;
	InputHandler::GetInstance()->HandleMouseScroll(xoffset, yoffset);
	
}

void Framework::TwEventKeyCallGLFW3(GLFWwindow*, int key, int, int action, int mods)
{
	ImGuiIO& io = ImGui::GetIO();
	if (action == GLFW_PRESS)
		io.KeysDown[key] = true;
	if (action == GLFW_RELEASE)
		io.KeysDown[key] = false;

	(void)mods; // Modifiers are not reliable across systems
	io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
	io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
	io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
	io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
}

/*
Initalize all the components in the GUI antweekbar
*/
void Framework::InitDebugGui()
{
	int width = (int)masterRendered->GetWindowSize().x;
	int height = (int)masterRendered->GetWindowSize().y;
	
	ImGui_Init(true);
	
	glfwSetMouseButtonCallback(masterRendered->GetWindow(), Framework::TwEventMouseButtonGLFW3);
	glfwSetCursorPosCallback(masterRendered->GetWindow(), Framework::TwEventMouseMoveGLFW3);
	glfwSetScrollCallback(masterRendered->GetWindow(), Framework::TwEventMouseScrollGLFW3);
	//glfwSetKeyCallback(masterRendered->GetWindow(), Framework::TwEventKeyCallGLFW3);
}


void ImGui_RenderDrawLists(ImDrawData* draw_data)
{
	// Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates != framebuffer coordinates)
	ImGuiIO& io = ImGui::GetIO();
	int fb_width = (int)(io.DisplaySize.x * io.DisplayFramebufferScale.x);
	int fb_height = (int)(io.DisplaySize.y * io.DisplayFramebufferScale.y);
	if (fb_width == 0 || fb_height == 0)
		return;
	draw_data->ScaleClipRects(io.DisplayFramebufferScale);

	// Backup GL state
	GLint last_program; glGetIntegerv(GL_CURRENT_PROGRAM, &last_program);
	GLint last_texture; glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
	GLint last_array_buffer; glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);
	GLint last_element_array_buffer; glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &last_element_array_buffer);
	GLint last_vertex_array; glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last_vertex_array);
	GLint last_blend_src; glGetIntegerv(GL_BLEND_SRC, &last_blend_src);
	GLint last_blend_dst; glGetIntegerv(GL_BLEND_DST, &last_blend_dst);
	GLint last_blend_equation_rgb; glGetIntegerv(GL_BLEND_EQUATION_RGB, &last_blend_equation_rgb);
	GLint last_blend_equation_alpha; glGetIntegerv(GL_BLEND_EQUATION_ALPHA, &last_blend_equation_alpha);
	GLint last_viewport[4]; glGetIntegerv(GL_VIEWPORT, last_viewport);
	GLboolean last_enable_blend = glIsEnabled(GL_BLEND);
	GLboolean last_enable_cull_face = glIsEnabled(GL_CULL_FACE);
	GLboolean last_enable_depth_test = glIsEnabled(GL_DEPTH_TEST);
	GLboolean last_enable_scissor_test = glIsEnabled(GL_SCISSOR_TEST);

	// Setup render state: alpha-blending enabled, no face culling, no depth testing, scissor enabled
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_SCISSOR_TEST);
	glActiveTexture(GL_TEXTURE0);

	// Setup viewport, orthographic projection matrix
	glViewport(0, 0, (GLsizei)fb_width, (GLsizei)fb_height);
	const float ortho_projection[4][4] =
	{
		{ 2.0f / io.DisplaySize.x, 0.0f, 0.0f, 0.0f },
		{ 0.0f, 2.0f / -io.DisplaySize.y, 0.0f, 0.0f },
		{ 0.0f, 0.0f, -1.0f, 0.0f },
		{ -1.0f, 1.0f, 0.0f, 1.0f },
	};
	glUseProgram(g_ShaderHandle);
	glUniform1i(g_AttribLocationTex, 0);
	glUniformMatrix4fv(g_AttribLocationProjMtx, 1, GL_FALSE, &ortho_projection[0][0]);
	glBindVertexArray(g_VaoHandle);

	for (int n = 0; n < draw_data->CmdListsCount; n++)
	{
		const ImDrawList* cmd_list = draw_data->CmdLists[n];
		const ImDrawIdx* idx_buffer_offset = 0;

		glBindBuffer(GL_ARRAY_BUFFER, g_VboHandle);
		glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)cmd_list->VtxBuffer.size() * sizeof(ImDrawVert), (GLvoid*)&cmd_list->VtxBuffer.front(), GL_STREAM_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_ElementsHandle);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)cmd_list->IdxBuffer.size() * sizeof(ImDrawIdx), (GLvoid*)&cmd_list->IdxBuffer.front(), GL_STREAM_DRAW);

		for (const ImDrawCmd* pcmd = cmd_list->CmdBuffer.begin(); pcmd != cmd_list->CmdBuffer.end(); pcmd++)
		{
			if (pcmd->UserCallback)
			{
				pcmd->UserCallback(cmd_list, pcmd);
			}
			else
			{
				glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->TextureId);
				glScissor((int)pcmd->ClipRect.x, (int)(fb_height - pcmd->ClipRect.w), (int)(pcmd->ClipRect.z - pcmd->ClipRect.x), (int)(pcmd->ClipRect.w - pcmd->ClipRect.y));
				glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount, sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, idx_buffer_offset);
			}
			idx_buffer_offset += pcmd->ElemCount;
		}
	}

	// Restore modified GL state
	glUseProgram(last_program);
	glBindTexture(GL_TEXTURE_2D, last_texture);
	glBindVertexArray(last_vertex_array);
	glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, last_element_array_buffer);
	glBlendEquationSeparate(last_blend_equation_rgb, last_blend_equation_alpha);
	glBlendFunc(last_blend_src, last_blend_dst);
	if (last_enable_blend) glEnable(GL_BLEND); else glDisable(GL_BLEND);
	if (last_enable_cull_face) glEnable(GL_CULL_FACE); else glDisable(GL_CULL_FACE);
	if (last_enable_depth_test) glEnable(GL_DEPTH_TEST); else glDisable(GL_DEPTH_TEST);
	if (last_enable_scissor_test) glEnable(GL_SCISSOR_TEST); else glDisable(GL_SCISSOR_TEST);
	glViewport(last_viewport[0], last_viewport[1], (GLsizei)last_viewport[2], (GLsizei)last_viewport[3]);
}

bool  Framework::ImGui_Init( bool install_callbacks)
{
	ImGuiIO& io = ImGui::GetIO();
	io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;                     // Keyboard mapping. ImGui will use those indices to peek into the io.KeyDown[] array.
	io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
	io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
	io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
	io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
	io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
	io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
	io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
	io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
	io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
	io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
	io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
	io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
	io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
	io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
	io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
	io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
	io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
	io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;
	io.RenderDrawListsFn = ImGui_RenderDrawLists;      // Alternatively you can set this to NULL and call ImGui::GetDrawData() after ImGui::Render() to get the same ImDrawData pointer.

#ifdef _WIN32
	io.ImeWindowHandle = glfwGetWin32Window(masterRendered->GetWindow());
#endif

	return true;
}

void  Framework::Shutdown()
{
	if (g_VaoHandle) glDeleteVertexArrays(1, &g_VaoHandle);
	if (g_VboHandle) glDeleteBuffers(1, &g_VboHandle);
	if (g_ElementsHandle) glDeleteBuffers(1, &g_ElementsHandle);
	g_VaoHandle = g_VboHandle = g_ElementsHandle = 0;

	glDetachShader(g_ShaderHandle, g_VertHandle);
	glDeleteShader(g_VertHandle);
	g_VertHandle = 0;

	glDetachShader(g_ShaderHandle, g_FragHandle);
	glDeleteShader(g_FragHandle);
	g_FragHandle = 0;

	glDeleteProgram(g_ShaderHandle);
	g_ShaderHandle = 0;

	if (g_FontTexture)
	{
		glDeleteTextures(1, &g_FontTexture);
		ImGui::GetIO().Fonts->TexID = 0;
		g_FontTexture = 0;
	}
	ImGui::Shutdown();
}

bool  Framework::CreateDeviceObjects()
{
	// Backup GL state
	GLint last_texture, last_array_buffer, last_vertex_array;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
	glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);
	glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last_vertex_array);

	const GLchar *vertex_shader =
		"#version 330\n"
		"uniform mat4 ProjMtx;\n"
		"in vec2 Position;\n"
		"in vec2 UV;\n"
		"in vec4 Color;\n"
		"out vec2 Frag_UV;\n"
		"out vec4 Frag_Color;\n"
		"void main()\n"
		"{\n"
		"	Frag_UV = UV;\n"
		"	Frag_Color = Color;\n"
		"	gl_Position = ProjMtx * vec4(Position.xy,0,1);\n"
		"}\n";

	const GLchar* fragment_shader =
		"#version 330\n"
		"uniform sampler2D Texture;\n"
		"in vec2 Frag_UV;\n"
		"in vec4 Frag_Color;\n"
		"out vec4 Out_Color;\n"
		"void main()\n"
		"{\n"
		"	Out_Color = Frag_Color * texture( Texture, Frag_UV.st);\n"
		"}\n";

	g_ShaderHandle = glCreateProgram();
	g_VertHandle = glCreateShader(GL_VERTEX_SHADER);
	g_FragHandle = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(g_VertHandle, 1, &vertex_shader, 0);
	glShaderSource(g_FragHandle, 1, &fragment_shader, 0);
	glCompileShader(g_VertHandle);
	glCompileShader(g_FragHandle);
	glAttachShader(g_ShaderHandle, g_VertHandle);
	glAttachShader(g_ShaderHandle, g_FragHandle);
	glLinkProgram(g_ShaderHandle);

	g_AttribLocationTex = glGetUniformLocation(g_ShaderHandle, "Texture");
	g_AttribLocationProjMtx = glGetUniformLocation(g_ShaderHandle, "ProjMtx");
	g_AttribLocationPosition = glGetAttribLocation(g_ShaderHandle, "Position");
	g_AttribLocationUV = glGetAttribLocation(g_ShaderHandle, "UV");
	g_AttribLocationColor = glGetAttribLocation(g_ShaderHandle, "Color");

	glGenBuffers(1, &g_VboHandle);
	glGenBuffers(1, &g_ElementsHandle);

	glGenVertexArrays(1, &g_VaoHandle);
	glBindVertexArray(g_VaoHandle);
	glBindBuffer(GL_ARRAY_BUFFER, g_VboHandle);
	glEnableVertexAttribArray(g_AttribLocationPosition);
	glEnableVertexAttribArray(g_AttribLocationUV);
	glEnableVertexAttribArray(g_AttribLocationColor);

#define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))
	glVertexAttribPointer(g_AttribLocationPosition, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, pos));
	glVertexAttribPointer(g_AttribLocationUV, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, uv));
	glVertexAttribPointer(g_AttribLocationColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, col));
#undef OFFSETOF

	CreateFontsTexture();

	// Restore modified GL state
	glBindTexture(GL_TEXTURE_2D, last_texture);
	glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);
	glBindVertexArray(last_vertex_array);

	return true;
}

void Framework::CreateFontsTexture()
{
	// Build texture atlas
	ImGuiIO& io = ImGui::GetIO();
	unsigned char* pixels;
	int width, height;
	io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);   // Load as RGBA 32-bits for OpenGL3 demo because it is more likely to be compatible with user's existing shader.

	// Upload texture to graphics system
	GLint last_texture;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
	glGenTextures(1, &g_FontTexture);
	glBindTexture(GL_TEXTURE_2D, g_FontTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

	// Store our identifier
	io.Fonts->TexID = (void *)(intptr_t)g_FontTexture;

	// Restore state
	glBindTexture(GL_TEXTURE_2D, last_texture);
}

void  Framework::ImGui_NewFrame()
{
	if (!g_FontTexture)
		CreateDeviceObjects();

	ImGuiIO& io = ImGui::GetIO();

	// Setup display size (every frame to accommodate for window resizing)
	int w, h;
	int display_w, display_h;
	glfwGetWindowSize(masterRendered->GetWindow(), &w, &h);
	glfwGetFramebufferSize(masterRendered->GetWindow(), &display_w, &display_h);
	io.DisplaySize = ImVec2((float)w, (float)h);
	io.DisplayFramebufferScale = ImVec2(w > 0 ? ((float)display_w / w) : 0, h > 0 ? ((float)display_h / h) : 0);

	// Setup time step
	double current_time = glfwGetTime();
	io.DeltaTime = g_Time > 0.0 ? (float)(current_time - g_Time) : (float)(1.0f / 60.0f);
	g_Time = current_time;

	// Setup inputs
	// (we already got mouse wheel, keyboard keys & characters from glfw callbacks polled in glfwPollEvents())
	if (glfwGetWindowAttrib(masterRendered->GetWindow(), GLFW_FOCUSED))
	{
		double mouse_x, mouse_y;
		glfwGetCursorPos(masterRendered->GetWindow(), &mouse_x, &mouse_y);
		io.MousePos = ImVec2((float)mouse_x, (float)mouse_y);   // Mouse position in screen coordinates (set to -1,-1 if no mouse / on another screen, etc.)
	}
	else
	{
		io.MousePos = ImVec2(-1, -1);
	}

	for (int i = 0; i < 3; i++)
	{
		io.MouseDown[i] = g_MousePressed[i] || glfwGetMouseButton(masterRendered->GetWindow(), i) != 0;    // If a mouse press event came, always pass it as "mouse held this frame", so we don't miss click-release events that are shorter than 1 frame.
		g_MousePressed[i] = false;
	}

	io.MouseWheel = g_MouseWheel;
	g_MouseWheel = 0.0f;

	// Hide OS mouse cursor if ImGui is drawing it
	glfwSetInputMode(masterRendered->GetWindow(), GLFW_CURSOR, io.MouseDrawCursor ? GLFW_CURSOR_HIDDEN : GLFW_CURSOR_NORMAL);

	// Start the frame
	ImGui::NewFrame();
}

void Framework::DebugWindow()
{
	// 1. Show a simple window
	// Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears in a window automatically called "Debug"
	{
		static float aligmentWeight = 1.0f;
		static float cohesionWeight = 1.0f;
		static float separationWeight = 1.0f;
		ImGui::Text("Flocking Weights");
		ImGui::SliderFloat("Aligment", &aligmentWeight, 0.0f, 2.0f);
		ImGui::SliderFloat("Cohesion", &cohesionWeight, 0.0f, 2.0f);
		ImGui::SliderFloat("Separation", &separationWeight, 0.0f, 2.0f);
		masterRendered->SetAlignmentWeight(aligmentWeight);
		masterRendered->SetCohesionWeight(cohesionWeight);
		masterRendered->SetSeparationWeight(separationWeight);
		
		static int e = 0;
		ImGui::RadioButton("Flock", &e, 0); ImGui::SameLine();
		ImGui::RadioButton("Wander", &e, 1); ImGui::SameLine();
		ImGui::RadioButton("Follow Leader", &e, 2); ImGui::SameLine();
		ImGui::RadioButton("Seek", &e, 3); 
		
		if (e == 0)
		{
			masterRendered->SetWander(false);
			masterRendered->SetFollowLeader(false);
			masterRendered->SetSeek(false);
		}
		else if (e == 1 )
		{
			masterRendered->SetWander(true);
			masterRendered->SetFollowLeader(false);
			masterRendered->SetSeek(false);
		}
		else if (e == 2)
		{
			masterRendered->SetWander(false);
			masterRendered->SetFollowLeader(true);
			masterRendered->SetSeek(false);
		}
		else if (e == 3)
		{
			masterRendered->SetWander(false);
			masterRendered->SetFollowLeader(false);
			masterRendered->SetSeek(true);
		}
	
		static bool moveTarget = true;
		ImGui::Checkbox("Move Target", &moveTarget);
		masterRendered->SetMoveTarget(moveTarget);



	}
}