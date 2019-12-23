#include "Application.h"

#include <iostream>

#include "Shader.h"
#include "glsl.h"
#include <glm/gtx/transform.hpp>

using namespace std;
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
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // consider switching to 2.1 with extensions for release
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


	glfwSwapInterval(1);

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
	glEnable(GL_CULL_FACE);

	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_LINE);

	/*auto ogldebugfunc = [](GLenum source,
		GLenum type,
		GLuint id,
		GLenum severity,
		GLsizei length,
		const GLchar* message,
		const void* userParam) {

			cout << "---------------------opengl-callback-start------------" << endl;
			cout << "message: " << message << endl;
			cout << "type: ";
			switch (type) {
			case GL_DEBUG_TYPE_ERROR:
				cout << "ERROR";
				break;
			case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
				cout << "DEPRECATED_BEHAVIOR";
				break;
			case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
				cout << "UNDEFINED_BEHAVIOR";
				break;
			case GL_DEBUG_TYPE_PORTABILITY:
				cout << "PORTABILITY";
				break;
			case GL_DEBUG_TYPE_PERFORMANCE:
				cout << "PERFORMANCE";
				break;
			case GL_DEBUG_TYPE_OTHER:
				cout << "OTHER";
				break;
			}
			cout << endl;

			cout << "id: " << id << endl;
			cout << "severity: ";
			switch (severity) {
			case GL_DEBUG_SEVERITY_LOW:
				cout << "LOW";
				break;
			case GL_DEBUG_SEVERITY_MEDIUM:
				cout << "MEDIUM";
				break;
			case GL_DEBUG_SEVERITY_HIGH:
				cout << "HIGH";
				break;
			}
			cout << endl;
			cout << "---------------------opengl-callback-end--------------" << endl;
	};

	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(ogldebugfunc, nullptr);
	GLuint unusedIds = 0;
	glDebugMessageControl(GL_DONT_CARE,
		GL_DONT_CARE,
		GL_DONT_CARE,
		0,
		&unusedIds,
		true);*/
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

				// bottom face
				m.Vertices.push_back(1 + x); m.Vertices.push_back(y); m.Vertices.push_back(1 + z);
				m.Vertices.push_back(1 + x); m.Vertices.push_back(y); m.Vertices.push_back(0 + z);
				m.Vertices.push_back(0 + x); m.Vertices.push_back(y); m.Vertices.push_back(0 + z);
				m.Vertices.push_back(0 + x); m.Vertices.push_back(y); m.Vertices.push_back(1 + z);

				// bottom

				m.Indices.push_back(index + 3); m.Indices.push_back(index + 1); m.Indices.push_back(index + 0);
				m.Indices.push_back(index + 3); m.Indices.push_back(index + 2); m.Indices.push_back(index + 1);
				index += 4;

				// back face
				m.Vertices.push_back(1 + x); m.Vertices.push_back(1 + y); m.Vertices.push_back(z);
				m.Vertices.push_back(1 + x); m.Vertices.push_back(y); m.Vertices.push_back(z);
				m.Vertices.push_back(0 + x); m.Vertices.push_back(y); m.Vertices.push_back(z);
				m.Vertices.push_back(0 + x); m.Vertices.push_back(1 + y); m.Vertices.push_back(z);

				// back

				m.Indices.push_back(index + 0); m.Indices.push_back(index + 1); m.Indices.push_back(index + 3);
				m.Indices.push_back(index + 1); m.Indices.push_back(index + 2); m.Indices.push_back(index + 3);
				index += 4;

				// front face
				m.Vertices.push_back(1 + x); m.Vertices.push_back(1 + y); m.Vertices.push_back(z + 1);
				m.Vertices.push_back(1 + x); m.Vertices.push_back(y); m.Vertices.push_back(z + 1);
				m.Vertices.push_back(0 + x); m.Vertices.push_back(y); m.Vertices.push_back(z + 1);
				m.Vertices.push_back(0 + x); m.Vertices.push_back(1 + y); m.Vertices.push_back(z + 1);

				// front

				m.Indices.push_back(index + 3); m.Indices.push_back(index + 1); m.Indices.push_back(index + 0);
				m.Indices.push_back(index + 3); m.Indices.push_back(index + 2); m.Indices.push_back(index + 1);
				index += 4;

				// left face
				m.Vertices.push_back(x); m.Vertices.push_back(1 + y); m.Vertices.push_back(z + 1);
				m.Vertices.push_back(x); m.Vertices.push_back(y); m.Vertices.push_back(z + 1);
				m.Vertices.push_back(x); m.Vertices.push_back(y); m.Vertices.push_back(z);
				m.Vertices.push_back(x); m.Vertices.push_back(1 + y); m.Vertices.push_back(z);

				// left

				m.Indices.push_back(index + 3); m.Indices.push_back(index + 1); m.Indices.push_back(index + 0);
				m.Indices.push_back(index + 3); m.Indices.push_back(index + 2); m.Indices.push_back(index + 1);
				index += 4;

				// right face
				m.Vertices.push_back(x + 1); m.Vertices.push_back(1 + y); m.Vertices.push_back(z + 1);
				m.Vertices.push_back(x + 1); m.Vertices.push_back(y); m.Vertices.push_back(z + 1);
				m.Vertices.push_back(x + 1); m.Vertices.push_back(y); m.Vertices.push_back(z);
				m.Vertices.push_back(x + 1); m.Vertices.push_back(1 + y); m.Vertices.push_back(z);

				// right

				m.Indices.push_back(index + 0); m.Indices.push_back(index + 1); m.Indices.push_back(index + 3);
				m.Indices.push_back(index + 1); m.Indices.push_back(index + 2); m.Indices.push_back(index + 3);
				index += 4;

				//uvs
				for (int i = 0; i < 6; i++)
				{
					m.UVs.push_back(0); m.UVs.push_back(0);
					m.UVs.push_back(1); m.UVs.push_back(0);
					m.UVs.push_back(1); m.UVs.push_back(1);
					m.UVs.push_back(0); m.UVs.push_back(1);
				}
			}
		}
	}

	Sh = std::unique_ptr<Shader>(new Shader("test.v", "test.f"));
	TextureShader = std::unique_ptr<Shader>(new Shader("Data//Shaders//texture.v", "Data//Shaders//texture.f"));
	TextureArrayShader = std::unique_ptr<Shader>(new Shader("Data//Shaders//texturearray.v", "Data//Shaders//texturearray.f"));
	//glUniform1i(0, 0);
	TestTexture = std::unique_ptr<Texture>(new Texture("Data//Textures//dirt.jpg", GL_TEXTURE_2D));
	TestTexture2 = std::unique_ptr<Texture>(new Texture("Data//Textures//stone.png", GL_TEXTURE_2D));

	//std::unique_ptr<Shader>(new Shader("test.v", "test.f"));
	m.UVsEnabled = true;
	m.Create(TextureShader.get());

	m2.UVsEnabled = true;
	m2.Vertices = m.Vertices;
	m2.Indices = m.Indices;
	m2.UVs = m.UVs;
	m2.Create(TextureShader.get());

	m2.WorldMatrix = glm::translate(m2.WorldMatrix, glm::vec3(0, 16, 0));
		
	ColorShader = std::unique_ptr<Shader>(new Shader("Data//Shaders//color.v", "Data//Shaders//color.f"));
	//coordsMesh.Create(ColorShader.get());
	CoordsObj.Create(ColorShader.get());

	MainCamera.SetSpeed(0.1f);
	MainCamera.SetPosition(glm::vec3(0, 0, 5));
}

void Application::RenderTestCode()
{
	
	CoordsObj.Render(ColorShader.get(), &MainCamera);

	TestTexture->bind(0);

	m.Render(TextureShader.get(), &MainCamera);

	TestTexture2->bind(0);

	m2.Render(TextureShader.get(), &MainCamera);
}
