// Taken from https://learnopengl.com/code_viewer_gh.php?code=src/1.getting_started/1.2.hello_window_clear/hello_window_clear.cpp and modified to work with glfw
// All credit go to Joey De Vries: https://joeydevries.com/#home author of Learn OpenGL.
// Modified by: Jakob Törmä Ruhl

//#include <GL/glew.h>
/*#include "glsl.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <array>

const std::array<GLfloat, 12> frontFace
{
	0, 0, 1,
	1, 0, 1,
	1, 1, 1,
	0, 1, 1,
};

const std::array<GLfloat, 12> backFace
{
	1, 0, 0,
	0, 0, 0,
	0, 1, 0,
	1, 1, 0,
};

const std::array<GLfloat, 12> leftFace
{
	0, 0, 0,
	0, 0, 1,
	0, 1, 1,
	0, 1, 0,
};

const std::array<GLfloat, 12> rightFace
{
	1, 0, 1,
	1, 0, 0,
	1, 1, 0,
	1, 1, 1,
};

const std::array<GLfloat, 12> topFace
{
	0, 1, 1,
	1, 1, 1,
	1, 1, 0,
	0, 1, 0,
};

const std::array<GLfloat, 12> bottomFace
{
	0, 0, 0,
	1, 0, 0,
	1, 0, 1,
	0, 0, 1
};

const std::array<GLfloat, 12> xFace1
{
	0, 0, 0,
	1, 0, 1,
	1, 1, 1,
	0, 1, 0,
};

const std::array<GLfloat, 12> xFace2
{
	0, 0, 1,
	1, 0, 0,
	1, 1, 0,
	0, 1, 1,
};


#define _CRT_SECURE_NO_WARNINGS // undo that later on in project settings

GLuint loadBMP_custom(const char* imagepath)
{
	// Data read from the header of the BMP file
	unsigned char header[54]; // Each BMP file begins by a 54-bytes header
	unsigned int dataPos;     // Position in the file where the actual data begins
	unsigned int width, height;
	unsigned int imageSize;   // = width*height*3
	// Actual RGB data
	unsigned char* data;

	// Open the file
	FILE* file = fopen(imagepath, "rb");
	if (!file) { printf("Image could not be opened\n"); return 0; }

	if (fread(header, 1, 54, file) != 54) { // If not 54 bytes read : problem
		printf("Not a correct BMP file\n");
		return false;
	}

	if (header[0] != 'B' || header[1] != 'M') {
		printf("Not a correct BMP file\n");
		return 0;
	}

	// Read ints from the byte array
	dataPos = *(int*) & (header[0x0A]);
	imageSize = *(int*) & (header[0x22]);
	width = *(int*) & (header[0x12]);
	height = *(int*) & (header[0x16]);

	// Some BMP files are misformatted, guess missing information
	if (imageSize == 0)    imageSize = width * height * 3; // 3 : one byte for each Red, Green and Blue component
	if (dataPos == 0)      dataPos = 54; // The BMP header is done that way

	// Create a buffer
	data = new unsigned char[imageSize];

	// Read the actual data from the file into the buffer
	fread(data, 1, imageSize, file);

	//Everything is in memory now, the file can be closed
	fclose(file);

	// Create one OpenGL texture
	GLuint textureID;
	glGenTextures(1, &textureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera

// position
glm::vec3 position = glm::vec3(0, 0, 15);
// horizontal angle : toward -Z
float horizontalAngle = 3.14f;
// vertical angle : 0, look at the horizon
float verticalAngle = 0.0f;
// Initial Field of View
float initialFoV = 45.0f;

float speed = 0.0002f; // 3 units / second
float mouseSpeed = 0.0005f;

float lastTime = 0;

// -
*/

#include "Application.h"

int main()
{
	Application app(1600, 1200);
	app.Run();

	/*// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	//init glew after the context have been made
	glewInit();


	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);



	glShaderManager sm;
	glShader *shader = sm.loadfromFile("vertex.v", "fragment.f");



	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

	// Or, for an ortho camera :
	//glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates

	// Camera matrix
	glm::mat4 View = glm::lookAt(
		glm::vec3(4, 3, 3), // Camera is at (4,3,3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);

	// Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 Model = glm::mat4(1.0f);
	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 mvp = Projection * View * Model; // Remember, matrix multiplication is the other way around


	// Get a handle for our "MVP" uniform
// Only during the initialisation
	GLuint MatrixID = glGetUniformLocation(shader->GetProgramObject(), "MVP");

	
	GLuint Texture = loadBMP_custom("uvtemplate.bmp");

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);



	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		processInput(window);

		//double currentTime = glfwGetTime();
		float deltaTime = float((float)glfwGetTime() - lastTime);

		// Get mouse position
		double xpos, ypos;
		//glfwGetMousePos(&xpos, &ypos);
		glfwGetCursorPos(window, &xpos, &ypos);

		// Compute new orientation
		horizontalAngle += mouseSpeed * deltaTime * float(SCR_WIDTH / 2 - xpos);
		verticalAngle += mouseSpeed * deltaTime * float(SCR_HEIGHT / 2 - ypos);

		// Direction : Spherical coordinates to Cartesian coordinates conversion
		glm::vec3 direction(
			cos(verticalAngle)* sin(horizontalAngle),
			sin(verticalAngle),
			cos(verticalAngle)* cos(horizontalAngle)
		);

		// Right vector
		glm::vec3 right = glm::vec3(
			sin(horizontalAngle - 3.14f / 2.0f),
			0,
			cos(horizontalAngle - 3.14f / 2.0f)
		);

		// Up vector : perpendicular to both direction and right
		glm::vec3 up = glm::cross(right, direction);

		// Move forward
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
			position += direction * deltaTime * speed;
		}
		// Move backward
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
			position -= direction * deltaTime * speed;
		}
		// Strafe right
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
			position += right * deltaTime * speed;
		}
		// Strafe left
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
			position -= right * deltaTime * speed;
		}

		//float FoV = initialFoV - 5 * glfwGetMouseWheel();
		// Projection matrix : 45&deg; Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
		glm::mat4 ProjectionMatrix = glm::perspective((float)glm::radians(60.0f), 4.0f / 3.0f, 0.1f, 1000.0f);
		// Camera matrix
		glm::mat4 ViewMatrix = glm::lookAt(
			position,           // Camera is here
			position + direction, // and looks here : at the same position, plus "direction"
			up                  // Head is up (set to 0,-1,0 to look upside-down)
		);


		glfwSetCursorPos(window, SCR_WIDTH / 2, SCR_HEIGHT / 2);
		lastTime = deltaTime;

		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		//glUseProgram(programID);
		shader->begin();

		

		// Send our transformation to the currently bound shader, in the "MVP" uniform
	// This is done in the main loop since each model will have a different MVP matrix (At least for the M part)
		glm::mat4 mvpn = ProjectionMatrix * ViewMatrix;
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvpn[0][0]);
		

		// Draw the triangle !
		//glDrawArrays(GL_TRIANGLES, 0, 12 * 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
		//glDisableVertexAttribArray(0);

		shader->end();

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;*/


}
/*
// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}*/