#pragma once

// third party
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

// system
#include <memory>

// custom
#include "Color.h"
#include "Camera.h"
#include "Mesh.h"

struct DestroyGLFWwnd {

	void operator()(GLFWwindow* ptr) {
		glfwDestroyWindow(ptr);
	}

};

typedef std::unique_ptr<GLFWwindow, DestroyGLFWwnd> sptr_GLFWwindow;

class Shader;
class glShaderManager;

class Application
{
public:
	Application(int windowWidth, int windowHeight);
	~Application();

	void Run();

private:

	void Init();
	void MainLoop();
	void Release();

	void ProcessInput();
	void UpdateLogic();
	void Render();
	void RenderUI();

	void InitGLFW();
	void InitGLEW();
	void InitOGL();
	void InitIMGUI();

	void GLFWFramebufferSizeCallback(GLFWwindow* window, int width, int height);
	void GLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

	void CalculateDeltaTime();
	void SaveLastDeltaTime();

	void ClearColor();
	void SwapBuffersPollEvents();

	void CloseOnEsc();

	void EnableCursor();
	void DisableCursor();
	

	sptr_GLFWwindow Window;

	Color BackgroundColor;

	int WindowWidth;
	int WindowHeight;

	float DeltaTime;
	float LastTime;

	//test

	void InitTestCode();
	void RenderTestCode();

	unsigned int VAO;

	std::unique_ptr<Shader> Sh;

	Camera MainCamera;

	Mesh m;
	//std::unique_ptr<glShaderManager> ShaderManager;
	//std::unique_ptr<glShader> TestShader;

	bool IsMouseLookEnabled = true;
	bool IsWireframeEnabled = false;

	std::unique_ptr<Shader> ColorShader;
	Mesh coordsMesh;
};