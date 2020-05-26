#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
using namespace std;
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char *fragmentShader1Source = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

const char *fragmentShader2Source = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
"}\n\0";

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "GLTest", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	//typedef void* (*GLADloadproc)(const char *name);
	//GLFWAPI GLFWglproc glfwGetProcAddress(const char* procname);
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// build and compile our shader program
		// ------------------------------------
		// vertex shader
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// fragment shader
	int fragment1Shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment1Shader, 1, &fragmentShader1Source, NULL);
	glCompileShader(fragment1Shader);

	int fragment2Shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment2Shader, 1, &fragmentShader2Source, NULL);
	glCompileShader(fragment2Shader);

	// link shaders
	int shader1Program = glCreateProgram();
	glAttachShader(shader1Program, vertexShader);
	glAttachShader(shader1Program, fragment1Shader);
	glLinkProgram(shader1Program);


	int shader2Program = glCreateProgram();
	glAttachShader(shader2Program, vertexShader);
	glAttachShader(shader2Program, fragment2Shader);
	glLinkProgram(shader2Program);



	glDeleteShader(vertexShader);
	glDeleteShader(fragment1Shader);
	glDeleteShader(fragment2Shader);


	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	//顶点数据

	float firstTriangle[] = {
		-0.9f, -0.5f, 0.0f,  // left 
		-0.0f, -0.5f, 0.0f,  // right
		-0.45f, 0.5f, 0.0f,  // top 
	};
	float secondTriangle[] = {
		0.0f, -0.5f, 0.0f,  // left
		0.9f, -0.5f, 0.0f,  // right
		0.45f, 0.5f, 0.0f   // top 
	};

	//索引数据
	//unsigned int indices[] = { // 注意索引从0开始! 
	//	0, 1, 2, // 第一个三角形
	//	2, 3, 4  // 第二个三角形
	//};

	unsigned int VBOs[2], VAOs[2];//顶点缓冲对象   顶点数组对象
	//unsigned int EBO;//索引缓冲对象
	//glGenBuffers(1, &EBO);//生成一个缓冲对象
	glGenVertexArrays(2, VAOs);;//生成一个顶点数组对象
	glGenBuffers(2, VBOs);//生成一个索引缓冲对象

	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle), secondTriangle, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);



	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	//glBindVertexArray(0);

	// uncomment this call to draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	float r = 0.2f;
	float g = 0.3f;
	float b = 0.3f;
	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(r, g, b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// draw our first triangle
		glUseProgram(shader1Program);
		glBindVertexArray(VAOs[0]); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glUseProgram(shader2Program);
		glBindVertexArray(VAOs[1]); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		glDrawArrays(GL_TRIANGLES, 0, 3);


		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//glBindVertexArray(0); // no need to unbind it every time 

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	//glDeleteVertexArrays(1, &VAO);
	//glDeleteBuffers(1, &VBO);
	//glDeleteProgram(shaderProgram);
	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
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
	printf("width = %d,height = %d\n", width, height);
	glViewport(0, 0, width, height);
}