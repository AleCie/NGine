#include "Application.h"

#include <iostream>

#include "Shader.h"
#include "glsl.h"
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

float noiseTreshold = 0;
float noiseScale = 1;

void Application::RenderUI()
{
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Terrain Gen");
	ImGui::SliderFloat ("treshold", &noiseTreshold, -1.0f, 1.0f);
	ImGui::SliderFloat("scale", &noiseScale, 0.0f, 100.0f);
	if (ImGui::Button("generate"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
	{
		chunk.NoiseTreshold = noiseTreshold;
		chunk2.NoiseTreshold = noiseTreshold;
		chunk3.NoiseTreshold = noiseTreshold;
		chunk4.NoiseTreshold = noiseTreshold;

		chunk.NoiseScale = noiseScale;
		chunk2.NoiseScale = noiseScale;
		chunk3.NoiseScale = noiseScale;
		chunk4.NoiseScale = noiseScale;

		chunk.RebuildMesh();
		chunk2.RebuildMesh();
		chunk3.RebuildMesh();
		chunk4.RebuildMesh();
	}
	ImGui::End();

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
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);
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

	auto ogldebugfunc = [](GLenum source,
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
		true);
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

	Sh = std::unique_ptr<Shader>(new Shader("test.v", "test.f"));

	TextureArrayShader = std::unique_ptr<Shader>(new Shader("Data//Shaders//texturearray.v", "Data//Shaders//texturearray.f"));
	TexArrLightShader = std::shared_ptr<Shader>(new Shader("Data//Shaders//texarrlight.v", "Data//Shaders//texarrlight.f"));

	TestTexture = std::unique_ptr<Texture>(new Texture("Data//Textures//dirt.jpg", GL_TEXTURE_2D));
	TestTexture2 = std::unique_ptr<Texture>(new Texture("Data//Textures//stone.png", GL_TEXTURE_2D));
	
	chunk.Create(glm::vec3(0), TexArrLightShader);
	chunk2.Create(glm::vec3(16, 0, 0), TexArrLightShader);
	chunk3.Create(glm::vec3(16, 0, 16), TexArrLightShader);
	chunk4.Create(glm::vec3(0, 0, 16), TexArrLightShader);


	ColorShader = std::unique_ptr<Shader>(new Shader("Data//Shaders//color.v", "Data//Shaders//color.f"));
	CoordsObj.Create(ColorShader.get());

	MainCamera.SetSpeed(0.1f);
	MainCamera.SetPosition(glm::vec3(0, 0, 5));


	glGenTextures(1, &TextureArray);
	glBindTexture(GL_TEXTURE_2D_ARRAY, TextureArray);
	glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, 512, 512, 2);//last three numbers are size of images and array

	int dw, dh, sw, sh;
	unsigned char* image1 = SOIL_load_image("Data//Textures//dirt.jpg", &dw, &dh, NULL, SOIL_LOAD_RGBA);
	unsigned char* image2 = SOIL_load_image("Data//Textures//stone.png", &sw, &sh, NULL, SOIL_LOAD_RGBA);

	glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, dw, dh, 1, GL_RGBA, GL_UNSIGNED_BYTE, image1);
	glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 1, sw, sh, 1, GL_RGBA, GL_UNSIGNED_BYTE, image2);


	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Application::RenderTestCode()
{
	
	CoordsObj.Render(ColorShader.get(), &MainCamera);

	//TestTexture->bind(0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D_ARRAY, TextureArray);

	TexArrLightShader->bind(); // this should be done once and somewhere else

	GLint modelLoc = glGetUniformLocation(TexArrLightShader->id(), "M"); //dont do that in main loop
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(chunk.GetWorldMatrix()));

	GLint lightPosLoc = glGetUniformLocation(TexArrLightShader->id(), "lightPos"); //dont do that in main loop

	GLint objectColorLoc = glGetUniformLocation(TexArrLightShader->id(), "objectColor"); //dont do that in main loop
	GLint lightColorLoc = glGetUniformLocation(TexArrLightShader->id(), "lightColor"); //dont do that in main loop

	GLint ambientStrenghtLoc = glGetUniformLocation(TexArrLightShader->id(), "ambientStrenght"); //dont do that in main loop
	GLint camPosLoc = glGetUniformLocation(TexArrLightShader->id(), "camPos"); //dont do that in main loop

	GLint cusDirLightLoc = glGetUniformLocation(TexArrLightShader->id(), "cusDirLight"); //dont do that in main loop


	glUniform3f(lightPosLoc, -5.0f, 5.0f, 5.0f);

	glUniform3f(objectColorLoc, 0.0f, 1.0f, 0.0f); //since it doesnt change could be done outside main loop
	glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f); //since it doesnt change could be done outside main loop

	glUniform1f(ambientStrenghtLoc, 0.5f); //since it doesnt change could be done outside main loop

	glUniform3f(camPosLoc, MainCamera.GetPosition().x, MainCamera.GetPosition().y, MainCamera.GetPosition().z);

	glUniform3f(cusDirLightLoc, 0.5f, -1, 0.5f);

	chunk.Render(&MainCamera);
	chunk2.Render(&MainCamera);
	chunk3.Render(&MainCamera);
	chunk4.Render(&MainCamera);
}
