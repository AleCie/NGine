#include "Application.h"

#include <iostream>

#include "Shader.h"
#include "glsl.h"

Application::Application(int windowWidth, int windowHeight)
	: MainCamera(60.0f, 4.0f / 3.0f, 0.1f, 1000.0f)
{
	WindowWidth = windowWidth;
	WindowHeight = windowHeight;

	BackgroundColor = Color(0.25f, 0.5f, 1, 1);

	DeltaTime = 0;
	LastTime = 0;
}

Application::~Application()
{
	glfwTerminate();
}

void Application::Run()
{
	Init();

	MainLoop();

	Release();
}

void Application::Init()
{
	InitGLFW();
	InitGLEW();
	InitOGL();
	InitIMGUI();

	InitTestCode();
}

void Application::MainLoop()
{
	while (!glfwWindowShouldClose(Window.get()))
	{
		CalculateDeltaTime();

		ProcessInput();
		UpdateLogic();

		Render();
		

		SaveLastDeltaTime();
	}
}

void Application::Release()
{
}

void Application::ProcessInput()
{
	CloseOnEsc();

	MainCamera.HandleInput(Window.get(), DeltaTime, WindowWidth, WindowHeight);
	if (IsMouseLookEnabled)
	{
		MainCamera.MouseLook(Window.get(), DeltaTime, WindowWidth, WindowHeight);
	}

	

}

void Application::UpdateLogic()
{
	MainCamera.Update(DeltaTime);
}

void Application::Render()
{
	RenderUI();

	ClearColor();

	RenderTestCode();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	SwapBuffersPollEvents();

	
}

float cpos[3], tpos[3];

void Application::RenderUI()
{
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	{
		static float f = 0.0f;
		static int counter = 0;
		static bool hasTarget = false;

		ImGui::Begin("Camera");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("position");

		ImGui::InputFloat3("pos", cpos);


		ImGui::Checkbox("target", &hasTarget);
		if (hasTarget)
		{
			ImGui::InputFloat3("dir", tpos);
		}

		if (ImGui::Button("Move"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
		{
			MainCamera.SetPosition(glm::vec3(cpos[0], cpos[1], cpos[2]));
			if (hasTarget)
			{
				glm::vec3 target = glm::vec3(cpos[0] - tpos[0], cpos[1] - tpos[1], cpos[2] - tpos[2]);
				
				MainCamera.SetDirection(glm::normalize(target));
			}
		}

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

	// Rendering
	ImGui::Render();
	
}

void Application::InitGLFW()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X

	Window = sptr_GLFWwindow(glfwCreateWindow(WindowWidth, WindowHeight, "LearnOpenGL", NULL, NULL));
	if (Window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		//return -1;
	}
	glfwMakeContextCurrent(Window.get());


	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(Window.get(), GLFW_STICKY_KEYS, GL_TRUE);
	// Hide the mouse and enable unlimited mouvement
	glfwSetInputMode(Window.get(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	//callbacks

	glfwSetWindowUserPointer(Window.get(), this);

	auto framebufferFunc = [](GLFWwindow* w, int width, int height)
	{
		static_cast<Application*>(glfwGetWindowUserPointer(w))->GLFWFramebufferSizeCallback(w, width, height);
	};

	glfwSetFramebufferSizeCallback(Window.get(), framebufferFunc);

	auto keyFunc = [](GLFWwindow* w, int key, int scancode, int action, int mods)
	{
		static_cast<Application*>(glfwGetWindowUserPointer(w))->GLFWKeyCallback(w, key, scancode, action, mods);
	};

	glfwSetKeyCallback(Window.get(), keyFunc);
}

void Application::InitGLEW()
{
	glewExperimental = GL_TRUE;
	//init glew after the context have been made
	glewInit();
	// enable experimental?

	GLint GlewInitResult = glewInit();
	if (GLEW_OK != GlewInitResult)
	{
		std::cout << "GLEW INIT FAILED";
	}
}

void Application::InitOGL()
{
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
	// Cull triangles which normal is not towards the camera
	//glEnable(GL_CULL_FACE);

	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_LINE);
}

void Application::InitIMGUI()
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(Window.get(), true);
	ImGui_ImplOpenGL3_Init("#version 330");
}


void Application::GLFWFramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

void Application::GLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_I && action == GLFW_PRESS)
	{
		MainCamera.SetAngles(0, 3.14f);
	}
	if (key == GLFW_KEY_O && action == GLFW_PRESS)
	{
		IsMouseLookEnabled = !IsMouseLookEnabled;

		if (IsMouseLookEnabled)
		{
			DisableCursor();
		}
		else
		{
			EnableCursor();
		}
	}
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		IsWireframeEnabled = !IsWireframeEnabled;

		if (IsWireframeEnabled)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			//glPolygonMode(GL_BACK, GL_LINE);
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
	}

	/*if (glfwGetKey(Window.get(), GLFW_KEY_Q))
	{
		MainCamera.SetAngles(0, 3.14f);
	}

	if (glfwGetKey(Window.get(), GLFW_KEY_P))
	{
		IsMouseLookEnabled = false;
		EnableCursor();
	}

	if (glfwGetKey(Window.get(), GLFW_KEY_O))
	{
		IsMouseLookEnabled = true;
		DisableCursor();
	}*/
}

void Application::CalculateDeltaTime()
{
	DeltaTime = float((float)glfwGetTime() - LastTime);
}

void Application::SaveLastDeltaTime()
{
	LastTime = DeltaTime;
}

void Application::ClearColor()
{
	glClearColor(BackgroundColor.GetR(), BackgroundColor.GetG(), BackgroundColor.GetB(), BackgroundColor.GetA());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Application::SwapBuffersPollEvents()
{
	// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
	glfwSwapBuffers(Window.get());
	glfwPollEvents();
}

void Application::CloseOnEsc()
{
	if (glfwGetKey(Window.get(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(Window.get(), true);

}

void Application::EnableCursor()
{
	glfwSetInputMode(Window.get(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void Application::DisableCursor()
{
	glfwSetInputMode(Window.get(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Application::InitTestCode()
{
	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	int size = 16;
	int index = 0;
	for (int x = 0; x < size; x++)
	{
		for (int y = 0; y < size; y++)
		{
			for (int z = 0; z < size; z++)
			{
				// top face
				m.Vertices.push_back(1 + x); m.Vertices.push_back(1 + y); m.Vertices.push_back(1 + z);
				m.Vertices.push_back(1 + x); m.Vertices.push_back(1 + y); m.Vertices.push_back(0 + z);
				m.Vertices.push_back(0 + x); m.Vertices.push_back(1 + y); m.Vertices.push_back(0 + z);
				m.Vertices.push_back(0 + x); m.Vertices.push_back(1 + y); m.Vertices.push_back(1 + z);

				// top

				m.Indices.push_back(index + 0); m.Indices.push_back(index + 1); m.Indices.push_back(index + 3);
				m.Indices.push_back(index + 1); m.Indices.push_back(index + 2); m.Indices.push_back(index + 3);
				index += 4;

				/*m.Colors.push_back(1); m.Colors.push_back(0); m.Colors.push_back(0);
				m.Colors.push_back(1); m.Colors.push_back(0); m.Colors.push_back(0);
				m.Colors.push_back(1); m.Colors.push_back(0); m.Colors.push_back(0);
				m.Colors.push_back(1); m.Colors.push_back(0); m.Colors.push_back(0);*/
			}
		}
	}
	//m.ColorsEnabled = true;

	coordsMesh.SetMeshDrawMode(EMeshDrawMode::Lines);
	coordsMesh.SetMeshLayout(EMeshLayout::VertexOnly);
	coordsMesh.ColorsEnabled = true;

	coordsMesh.Vertices.push_back(4); coordsMesh.Vertices.push_back(0); coordsMesh.Vertices.push_back(0);
	coordsMesh.Vertices.push_back(-4); coordsMesh.Vertices.push_back(0); coordsMesh.Vertices.push_back(0);

	coordsMesh.Vertices.push_back(4); coordsMesh.Vertices.push_back(0); coordsMesh.Vertices.push_back(0);
	coordsMesh.Vertices.push_back(-3); coordsMesh.Vertices.push_back(1); coordsMesh.Vertices.push_back(0);

	coordsMesh.Vertices.push_back(4); coordsMesh.Vertices.push_back(0); coordsMesh.Vertices.push_back(0);
	coordsMesh.Vertices.push_back(-3); coordsMesh.Vertices.push_back(-1); coordsMesh.Vertices.push_back(0);



	coordsMesh.Vertices.push_back(0); coordsMesh.Vertices.push_back(4); coordsMesh.Vertices.push_back(0);
	coordsMesh.Vertices.push_back(0); coordsMesh.Vertices.push_back(-4); coordsMesh.Vertices.push_back(0);

	coordsMesh.Vertices.push_back(0); coordsMesh.Vertices.push_back(4); coordsMesh.Vertices.push_back(0);
	coordsMesh.Vertices.push_back(1); coordsMesh.Vertices.push_back(-3); coordsMesh.Vertices.push_back(0);

	coordsMesh.Vertices.push_back(0); coordsMesh.Vertices.push_back(4); coordsMesh.Vertices.push_back(0);
	coordsMesh.Vertices.push_back(-1); coordsMesh.Vertices.push_back(-3); coordsMesh.Vertices.push_back(0);



	coordsMesh.Vertices.push_back(0); coordsMesh.Vertices.push_back(0); coordsMesh.Vertices.push_back(4);
	coordsMesh.Vertices.push_back(0); coordsMesh.Vertices.push_back(0); coordsMesh.Vertices.push_back(-4);

	coordsMesh.Vertices.push_back(0); coordsMesh.Vertices.push_back(0); coordsMesh.Vertices.push_back(4);
	coordsMesh.Vertices.push_back(0); coordsMesh.Vertices.push_back(1); coordsMesh.Vertices.push_back(-3);

	coordsMesh.Vertices.push_back(0); coordsMesh.Vertices.push_back(0); coordsMesh.Vertices.push_back(4);
	coordsMesh.Vertices.push_back(0); coordsMesh.Vertices.push_back(-1); coordsMesh.Vertices.push_back(-3);

	// colors
	coordsMesh.Colors.push_back(1); coordsMesh.Colors.push_back(0); coordsMesh.Colors.push_back(0);
	coordsMesh.Colors.push_back(1); coordsMesh.Colors.push_back(0); coordsMesh.Colors.push_back(0);
	coordsMesh.Colors.push_back(1); coordsMesh.Colors.push_back(0); coordsMesh.Colors.push_back(0);
	coordsMesh.Colors.push_back(1); coordsMesh.Colors.push_back(0); coordsMesh.Colors.push_back(0);
	coordsMesh.Colors.push_back(1); coordsMesh.Colors.push_back(0); coordsMesh.Colors.push_back(0);
	coordsMesh.Colors.push_back(1); coordsMesh.Colors.push_back(0); coordsMesh.Colors.push_back(0);

	coordsMesh.Colors.push_back(0); coordsMesh.Colors.push_back(1); coordsMesh.Colors.push_back(0);
	coordsMesh.Colors.push_back(0); coordsMesh.Colors.push_back(1); coordsMesh.Colors.push_back(0);
	coordsMesh.Colors.push_back(0); coordsMesh.Colors.push_back(1); coordsMesh.Colors.push_back(0);
	coordsMesh.Colors.push_back(0); coordsMesh.Colors.push_back(1); coordsMesh.Colors.push_back(0);
	coordsMesh.Colors.push_back(0); coordsMesh.Colors.push_back(1); coordsMesh.Colors.push_back(0);
	coordsMesh.Colors.push_back(0); coordsMesh.Colors.push_back(1); coordsMesh.Colors.push_back(0);

	coordsMesh.Colors.push_back(0); coordsMesh.Colors.push_back(0); coordsMesh.Colors.push_back(1);
	coordsMesh.Colors.push_back(0); coordsMesh.Colors.push_back(0); coordsMesh.Colors.push_back(1);
	coordsMesh.Colors.push_back(0); coordsMesh.Colors.push_back(0); coordsMesh.Colors.push_back(1);
	coordsMesh.Colors.push_back(0); coordsMesh.Colors.push_back(0); coordsMesh.Colors.push_back(1);
	coordsMesh.Colors.push_back(0); coordsMesh.Colors.push_back(0); coordsMesh.Colors.push_back(1);
	coordsMesh.Colors.push_back(0); coordsMesh.Colors.push_back(0); coordsMesh.Colors.push_back(1);
	/*// top face
	m.Vertices.push_back(1); m.Vertices.push_back(1); m.Vertices.push_back(1);
	m.Vertices.push_back(1); m.Vertices.push_back(1); m.Vertices.push_back(0);
	m.Vertices.push_back(0); m.Vertices.push_back(1); m.Vertices.push_back(0);
	m.Vertices.push_back(0); m.Vertices.push_back(1); m.Vertices.push_back(1);

	// top

	m.Indices.push_back(0); m.Indices.push_back(1); m.Indices.push_back(3);
	m.Indices.push_back(1); m.Indices.push_back(2); m.Indices.push_back(3);*/

	/*
	float vertices[] = {
		 0.5f,  0.5f, 0.0f,  // top right
		 0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f,   // top left 

		0.25f,  0.25f, 1.0f,  // top right
		 0.25f, -0.25f, 1.0f,  // bottom right
		-0.25f, -0.25f, 1.0f,  // bottom left
		-0.25f,  0.25f, 1.0f   // top left 
	};
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,  // first Triangle
		1, 2, 3,   // second Triangle

		4, 5, 7,
		5, 6, 7
	};
	unsigned int VBO, EBO;

	

	m.Vertices.insert(m.Vertices.end(), vertices, vertices + 24);
	m.Indices.insert(m.Indices.end(), indices, indices + 12);*/

	Sh = std::unique_ptr<Shader>(new Shader("test.v", "test.f"));
	//std::unique_ptr<Shader>(new Shader("test.v", "test.f"));
	m.Create(Sh.get());

	ColorShader = std::unique_ptr<Shader>(new Shader("Data//Shaders//color.v", "Data//Shaders//color.f"));
	coordsMesh.Create(ColorShader.get());

	MainCamera.SetSpeed(0.001f);
	MainCamera.SetPosition(glm::vec3(0, 0, 5));

	
}

void Application::RenderTestCode()
{
	
	coordsMesh.Render(ColorShader.get(), &MainCamera);
	m.Render(Sh.get(), &MainCamera);
}
