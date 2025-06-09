#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <openglErrorReporting.h>
#include <iostream>
#include <shader.h>


float triangleData[]{
	//position     colors
	//x y z        rgb
	0,1,0,		1,0,0,  //vertex1
	-1,-1,0,	0,1,0,	//vertex2
	1,-1,0,		0,0,1	//vertex3
};

int main() {

	GLFWwindow* window;

	if (!glfwInit()) {
		std::cout << "GLFW INIT ERROR!\n";
	}

	window = glfwCreateWindow(640, 480, "Triangle", NULL, NULL);
	if (!window) {
		std::cout << "WINDOW ERROR!\n";
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	enableReportGlErrors();

#pragma region buffer
	//createBuffer
	GLuint buffer = 0;
	glGenBuffers(1, &buffer);

	//send data to the buffer
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleData), triangleData, GL_STATIC_DRAW);

	//tell opengl the attributes in my triangle data
	glEnableVertexAttribArray(0); //position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);

	glEnableVertexAttribArray(1); //color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(sizeof(float)*3));

#pragma endregion

#pragma region shader
	Shader shader;

	shader.loadShaderProgramFromFile("resources/myshader.vert","resources/myshader.frag");
	shader.bind();
#pragma endregion

	while (!glfwWindowShouldClose(window)) {
		//resize
		int w = 0, h = 0;
		glfwGetWindowSize(window, &w, &h);
		glViewport(0, 0, w, h);

		//draw
		glClear(GL_COLOR_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLES, 0, 3);


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}


