#include "Application.h"

#include <iostream>

#include "Shader.h"
#include "glsl.h"

Application::Application(int windowWidth, int windowHeight)
	: MainCamera(90.0f, 4.0f / 3.0f, 0.1f, 1000.0f)
{
	WindowWidth = windowWidth;
	WindowHeight = windowHeight;

	BackgroundColor = Color(0, 0, 1, 1);

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

	if (glfwGetKey(Window.get(), GLFW_KEY_P))
	{
		IsMouseLookEnabled = false;
		EnableCursor();
	}

	if (glfwGetKey(Window.get(), GLFW_KEY_O))
	{
		IsMouseLookEnabled = true;
		DisableCursor();
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

	Sh = std::unique_ptr<Shader>(new Shader("test.v", "test.f"));

	//m.Vertices.push_back()
	m.Vertices.insert(m.Vertices.end(), vertices, vertices + 24);
	m.Indices.insert(m.Indices.end(), indices, indices + 12);
	m.Create(Sh.get());

	/*glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);*/

	
	//Sh.get()->init(vertexShaderSource, fragmentShaderSource);


	//ShaderManager = std::unique_ptr<glShaderManager>(new glShaderManager());
	
	MainCamera.SetSpeed(0.001f);
	MainCamera.SetPosition(glm::vec3(0, 0, 5));
}

void Application::RenderTestCode()
{
	if (glfwGetKey(Window.get(), GLFW_KEY_Q))
	{
		MainCamera.SetAngles(0, 3.14f);
	}
	//glUseProgram(Sh->id());
	/*Sh->bind();

	glm::mat4 ProjectionMatrix = MainCamera.GetProjectionMatrix();
	glm::mat4 ViewMatrix = MainCamera.GetViewMatrix();
	glm::mat4 ModelMatrix = m.WorldMatrix;
	glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

	// Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(Sh->id(), "MVP");
	// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);*/

	m.Render(Sh.get(), &MainCamera);
	/*glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		//glDrawArrays(GL_TRIANGLES, 0, 6);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);*/
}
