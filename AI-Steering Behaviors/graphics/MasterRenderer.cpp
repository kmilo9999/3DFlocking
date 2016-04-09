#include "MasterRenderer.h"
#include "PhongShading.h"
#include "Simpletechnique.h"
#include "gtc/matrix_transform.hpp"
#include "Flock.h"

MasterRenderer::MasterRenderer(vec2 windowSize) :window(), WindowSize(windowSize), FOV(45)
{
	EntityRender = new EntityRenderer();
	TerrainRender = new TerrainRenderer();
	camera = new Camera(vec3(0.0f, 6.0f, 25.0f));
	start = time(0);
	startFlock = false;
	AlignmentWeight = 1.0f ;
	CohesionWeight = 1.0f;
	SeparationWeight = 1.0f;
}


MasterRenderer::~MasterRenderer()
{
	if (EntityRender)
	{
		delete EntityRender;
	}
	if (camera)
	{
		delete camera;
	}
	if (TerrainRender)
	{
		delete TerrainRender;
	}

}


void MasterRenderer::Init()
{
	InitOpengl(); 
}

/*
Initialize Opengl
*/
void MasterRenderer::InitOpengl()
{
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));

	}


	glEnable(GL_DEPTH_TEST);
}

void MasterRenderer::CreateProjectionMatrix()
{
	ProjectionMatrix = glm::perspective(glm::radians(FOV), WindowSize.x / WindowSize.y, 0.1f, 100.0f);
}

/*
Render the element in the screen
*/
void MasterRenderer::RenderEntities(map<TexturedModel*, vector<Entity*>, TexturedModelCompare> entities)
{

	//EntityRender->RenderLight(ProjectionMatrix,camera->GetViewMatrix());

	EntityRender->Render(entities, ProjectionMatrix, camera->GetViewMatrix());

	
}


/*
Render the terrains
*/
void MasterRenderer::RenderTerrains(vector<Terrain*> terrains)
{
	TerrainRender->Render(terrains, ProjectionMatrix, camera->GetViewMatrix());
}

/*
Update positions and animations of the models
*/
void MasterRenderer::Update(map<TexturedModel*, vector<Entity*>, TexturedModelCompare> entities, float dt)
{
	UpdateEntites(entities, dt);
	UpdateCamera(dt);
}

/*
returns the instance of a GLFW window
*/
GLFWwindow* MasterRenderer::GetWindow()
{
	return window;
}

/*
return the size of the window
*/
void MasterRenderer::SetWindowSize(vec2 v)
{
	WindowSize = v;
}

/*
return the size of the window
*/
vec2 MasterRenderer::GetWindowSize()
{
	return WindowSize;
}

/*
Set a new dimension of the window
*/
void MasterRenderer::SetWindow(GLFWwindow* w)
{
	window = w;
}

void MasterRenderer::HandleWindowResize()
{
	//Handle window resize	
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		WindowSize.x = (float)width;
		WindowSize.y = (float)height;
		glViewport(0, 0, (GLsizei)WindowSize.x, (GLsizei)WindowSize.y);
}


void MasterRenderer::PrepareRenderer()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.5f, 0.0f, 0.0f, 1.0f);

}

void MasterRenderer::FinishRenderer()
{
	glfwSwapBuffers(window);
	/* Poll for and process events */
	glfwPollEvents();
}

void MasterRenderer::SetGlobalLight(Light* light)
{
	GlobalLight = light;
	EntityRender->SetGlobalLight(GlobalLight);
	TerrainRender->SetGlobalLight(GlobalLight);
}

Light* MasterRenderer::GetGlobalLight()
{
	return GlobalLight;
}

Camera* MasterRenderer::GetCamera()
{
	return camera;
}

void MasterRenderer::UpdateCamera(float dt)
{

	if (InputHandler::GetInstance()->GetKeyState(GLFW_KEY_W))
		camera->ProcessKeyboard(FORWARD, dt);
	if (InputHandler::GetInstance()->GetKeyState(GLFW_KEY_S))
		camera->ProcessKeyboard(BACKWARD, dt);
	if (InputHandler::GetInstance()->GetKeyState(GLFW_KEY_A))
		camera->ProcessKeyboard(LEFT, dt);
	if (InputHandler::GetInstance()->GetKeyState(GLFW_KEY_D))
		camera->ProcessKeyboard(RIGHT, dt);

	bool moveCamera = InputHandler::GetInstance()->GetMouseButtonState(GLFW_MOUSE_BUTTON_RIGHT);

	vec2 mouseCursorPosition = InputHandler::GetInstance()->GetPosCursorState();
	camera->ProcessMouseMovement(mouseCursorPosition.x, mouseCursorPosition.y, moveCamera);

	vec2 mouseScroll = InputHandler::GetInstance()->GetScrollOffsetState();
	ProcessMouseScroll(mouseScroll.y);
	CreateProjectionMatrix();
	InputHandler::GetInstance()->SetScrollOffsetState(vec2(0.0f, 0.0f));
}

void MasterRenderer::ProcessMouseScroll(float yoffset)
{

	if (FOV >= 1.0f && FOV <= 45.0f)
		FOV -= yoffset;
	if (FOV <= 1.0f)
		FOV = 1.0f;
	if (FOV > 45.0f)
		FOV = 45.0f;
}


void MasterRenderer::UpdateEntites(map<TexturedModel*, vector<Entity*>, TexturedModelCompare> entities, float dt)
{
	map<TexturedModel*, vector<Entity*>>::iterator it = entities.begin();
	for (it = entities.begin(); it != entities.end(); ++it)
	{
		TexturedModel* model = it->first;
		vector<Entity*> batch = it->second;

		if (model->GetId().compare("skyDome")==0)
		{
			Entity* skydome = batch[0];
			skydome->SetOrientation(glfwGetTime()* 0.01f,vec3(0,1,0));
			
		}


		if (!startFlock)
		{
			double seconds_since_start = difftime(time(0), start);
			if (seconds_since_start > 5)
			{
				startFlock = true;
			}
		}

		if (model->GetId().compare("pyramid") == 0 && startFlock && !Wander && !FollowL && !Seek)
		{
			for (Entity* entity : batch)
			{

				Agent* agent = dynamic_cast<Agent*>(entity);
			
				vec3 aligment = Steering::Aligment(batch, agent);
				vec3 cohesion = Steering::Cohesion(batch, agent);
				vec3 separation = Steering::Separation(batch, agent);
				vec3  desiredVelocity = agent->GetVelocity() + (cohesion* CohesionWeight) + 
										(separation * SeparationWeight) + (aligment * AlignmentWeight);
				desiredVelocity = normalize(desiredVelocity);


				Steering::Seek(agent);
				vec3 velocity = agent->GetVelocity();
				velocity += desiredVelocity;
				velocity += agent->GetOrientation() * vec3(0, 0, 1);
				velocity = normalize(velocity);

				agent->SetVelocity(velocity);
				 
				 

			}
		}

		if (model->GetId().compare("cube") == 0 && startFlock && !Wander && !FollowL && !Seek)
		{
			Entity* goal = batch[0];
			map<TexturedModel*, vector<Entity*>>::iterator it2 = entities.begin();
			for (it2 = entities.begin(); it2 != entities.end(); ++it2){
				TexturedModel* boidModel = it2->first;
				vector<Entity*> boids = it2->second;
					if (boidModel->GetId().compare("pyramid") == 0 )
					{
						for (Entity* entity : boids)
						{
							float distance = Math::Distance(goal->GetPosition(), entity->GetPosition());
							if (distance < 0.8 && Movetarget)
							{
								vec3 newPosition;
								newPosition.x = (rand() % 40) + (-20);
								newPosition.y = (rand() % 5) + 3;
								newPosition.z = (rand() % 40) + (-20);
								goal->SetPosition(newPosition);
							}
							
						}
					}
			}
			
			
		}

		if (Seek)
		{
			for (Entity* entity : batch)
			{
				Agent* agent = dynamic_cast<Agent*>(entity);
				Steering::Seek(agent);
				vec3 velocity = agent->GetVelocity();
				velocity += agent->GetOrientation() * vec3(0, 0, 1);
				velocity = normalize(velocity);
				agent->SetVelocity(velocity);
			}


			if (model->GetId().compare("cube") == 0)
			{
				Entity* goal = batch[0];
				map<TexturedModel*, vector<Entity*>>::iterator it2 = entities.begin();
				for (it2 = entities.begin(); it2 != entities.end(); ++it2){
					TexturedModel* boidModel = it2->first;
					vector<Entity*> boids = it2->second;
					if (boidModel->GetId().compare("pyramid") == 0)
					{
						for (Entity* entity : boids)
						{
							float distance = Math::Distance(goal->GetPosition(), entity->GetPosition());
							if (distance < 0.8 && Movetarget)
							{
								vec3 newPosition;
								newPosition.x = (rand() % 40) + (-20);
								newPosition.y = (rand() % 5) + 3;
								newPosition.z = (rand() % 40) + (-20);
								goal->SetPosition(newPosition);
							}

						}
					}
				}
			}
		}

		if (Wander)
		{
			if (model->GetId().compare("cube") == 0 )
			{
				Entity* goal = batch[0];
				goal->SetPosition(vec3(0, 7, 0));

			}

			for (Entity* entity : batch)
			{
				
				Agent* agent = dynamic_cast<Agent*>(entity);
				if (agent)
				{
					Steering::Wander(agent);
					vec3 velocity = entity->GetVelocity();
					velocity += entity->GetOrientation() * vec3(0, 0, 1);
					entity->SetVelocity(velocity);
				}


			}
		}

		if (FollowL)
		{
			if (model->GetId().compare("cube") == 0)
			{
				Entity* goal = batch[0];
				goal->SetPosition(vec3(0, 7, 0));

			}

			for (Entity* entity : batch)
			{

				Agent* agent = dynamic_cast<Agent*>(entity);
				if (agent)
				{
					Steering::FollowLeader(batch, agent);
				}


			}
		}

		for (Entity* entity : batch)
		{
			entity->Update(dt);
		}
	}
}