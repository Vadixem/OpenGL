#define GLEW_STATIC
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <conio.h>
#include <iostream>
#include <thread>
#include <chrono>

using namespace std;

void key_callback(GLFWwindow* window, int key, 
				  int scancode, int action, int mode);

const GLchar* vertexShaderSource = 
	"#version 330 core"

	"layout (location = 0) in vec3 position;"

"void main()"
"{"
"	gl_Position = vec4(position.x, position.y, position.z, 1.0);"
"}"
";"
;

const GLchar* fragmentShaderSource = 
	"#version 330 core"

	"out vec4 color;"

"void main()"
"{"
"	color = vec4(0.0f, 0.1f, 0.9f, 1.0f);"
"}"
";"
;

int main()
{
	//// initialize glew before we call any openGL func-ns
	//glewExperimental = GL_TRUE;
	//if (glewInit() != GLEW_OK)
	//{
	//	cout << "Failed to ini GLEW" << endl;
	//	return -1;
	//}

	// declare verts on stack
	GLfloat vertices[] = 
	{
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f, 0.5f, 0.0f
	};

	
	// id of vertices buffer obj
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	
	// copies vertex data(from vertices[]) into buffer's mem
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// VERTICES
	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// check if shaders compiled successfully
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(vertexShader, 521, NULL, infoLog);
		cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << 
		infoLog << endl;
	}

	// FRAGMENTS
	GLuint fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// SHADER PROGRAM
	GLuint shaderProgram;
	shaderProgram = glCreateProgram();

	// attach previously compiled shaders to program
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		cout << "Failed in linking shaders to program! " << endl;
	}

	glUseProgram(shaderProgram);

	// delete shaders couse once they were compiled we don't need 'em
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// tell the gl how to use buffer of vertices
	glVertexAttribLPointer(0, 3, GL_FLOAT, (GLsizei)3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	/* SOMETHING LIKE A SUMMARY (DUNNO) see p.45
	// 0. Copy out vertices array in a buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 1. Then set the vertex arrribs pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat),
		(GLvoid*)0);
	glEnableVertexAttribArray(0);
	// 2. Use our shader program when we want to render an obj
	glUseProgram(shaderProgram);
	// 3. Now draw the obj
	//someBlah...
	*/

	// generate Vertex Array Obj (VAO)
	GLuint VAO;
	glGenVertexArrays(1, &VAO);

//// ..:: Initialization code (done once (unless my obj frequently changes)) ::..
//	// 1. Bind VAO
//	glBindVertexArray(VAO);
//		// 2. Copy vertices arr in a buffer for opgl to use
//		glBindBuffer(GL_ARRAY_BUFFER, VBO);
//		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//		// 3. Then set vertex attribs pointers
//		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
//			(GLvoid*)0);
//		glEnableVertexAttribArray(0);
//	// 4. Unbind the VAO
//	glBindVertexArray(0);
//
//	// ..:: Drawing code (in Game loop) ::..
//	// 5. Draw the obj
//	glUseProgram(shaderProgram);
//	glBindVertexArray(VAO);
//	someOpenGLFunctionThatDrawsOurTriangle();
//	glBindVertexArray(0);
//
//// endof ..:: ::.. dafug I was doing all the time before ?@*@#>#

	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGlFirst", nullptr, nullptr);
	// if window == nullptr, terminate everything
	if (!window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	
	glfwMakeContextCurrent(window);

	glViewport(0, 0, 800, 600);

		//*float r, g, b;
		//*r = g = b = 0;
	while (!glfwWindowShouldClose(window))
	{

		// listen on users input
		glfwPollEvents();			
			//*r += 0.002f;
			//*g += 0.01f;
			//*b += 0.01f;
		// Rendering commands here
			//*glClearColor(r, g, b, 1.0f);
		glClearColor(1.0f, 0.3f, 0.1f, 1.0f);	
		glClear(GL_COLOR_BUFFER_BIT);
		// draw image
		glfwSwapBuffers(window);	
			//*this_thread::sleep_for(chrono::milliseconds(34));	
	}

	// register the func with the proper callback 
	glfwSetKeyCallback(window, key_callback);

	glfwTerminate();
	
	return 0;
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// When a user presses the escape key, we set the WindowShouldClose property to true, 
	// closing the application 
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	glfwSetWindowShouldClose(window, GL_TRUE);	
}