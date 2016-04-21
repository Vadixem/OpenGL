#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

#include <thread>

// Other Libs
#include <SOIL/SOIL.h>
// GLM Mathematics
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

// Other includes
#include "Shader.h"
#include "Camera.h"
// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
// For pseudo-parrallel input handling
void do_movement();
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xpos, double ypos);

// Window dimensions
const GLuint WIDTH = 1200, HEIGHT = 800;


	// Walk around
	// Declare it outsides main() so key_callback could use it
	glm::vec3 cameraPos = glm::vec3(0.f, 0.f, 3.f);
	glm::vec3 cameraFront = glm::vec3(0.f, 0.f, -1.f);
	glm::vec3 cameraUp = glm::vec3(0.f, 1.f, 0.f);

// Camera
	Camera camera(glm::vec3(0.f, 0.f, 3.f));
// For input handling
	bool keys[1024];

// Time between current frame and last frame.
	GLfloat deltaTime = 0.f;
// Time of last frame
	GLfloat lastFrame = 0.f;
// Current frame for tracking time spent on rendering last frame
	GLfloat currentFrame;
	
// Set cursor to the center of the screen
	GLfloat lastX = 600, lastY = 400;

// Add global yaw and pitch values
	GLfloat yaw   = -90.0f;	// Yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right (due to how Eular angles work) so we initially rotate a bit to the left.
	GLfloat pitch = 0.f; 

// This bool helps in preventing cam jumps
	GLboolean firstMouse = true;

// Global field of view value
	GLfloat fov = 45.f;

	// Position on the light source.
	glm::vec3 lightPos(1.f, 1.f, 3.f);


// The MAIN function, from here we start the application and run the game loop
int main()
{
	
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);
	// Set callback func for mouse input
	glfwSetCursorPosCallback(window, mouse_callback);
	// Set callback func for mouse input
	glfwSetScrollCallback(window, scroll_callback);

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    glewInit();

    // Define the viewport dimensions
    glViewport(0, 0, WIDTH, HEIGHT);

	// Capture cursor and hide it
	// After this call, wherever we move the mouse it won�t be visible and it should not leave the window
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Build and compile our shader program
	    Shader lightningShader("C:\\Users\\Vadim\\Documents\\Visual Studio 2012\\Projects\\OpenGlFirst\\shaders\\lightning.vs", 
		"C:\\Users\\Vadim\\Documents\\Visual Studio 2012\\Projects\\OpenGlFirst\\shaders\\lightning.frag");
		Shader lampShader("C:\\Users\\Vadim\\Documents\\Visual Studio 2012\\Projects\\OpenGlFirst\\shaders\\lamp.vs", 
		"C:\\Users\\Vadim\\Documents\\Visual Studio 2012\\Projects\\OpenGlFirst\\shaders\\lamp.frag");

    // Set up vertex data (and buffer(s)) and attribute pointers for CUBE
   GLfloat vertices[] = 
   {
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f

   };
    

    GLuint VBO, boxVAO;
    glGenVertexArrays(1, &boxVAO);
    glGenBuffers(1, &VBO);
    
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(boxVAO);    
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0); // Unbind boxVAO

	// Set the light's VAO, VBO stays the same
	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Set the vert attribs (only position for the lamp)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE , 3 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);

	
	// Init model matrix.
	glm::mat4 model;
	// Enable depth testing
	glEnable(GL_DEPTH_TEST);

	// Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();
		do_movement();

        // Render
        // Clear the colorbuffer
        //glClearColor(.03f, .01f, .09f, 1.f);
		glClearColor(.1f, .1f, .1f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Use coresponding shader when setting uniforms/drawing objs
		lightningShader.Use();

	GLint objectColorLoc = glGetUniformLocation(lightningShader.Program, "objectColor");
	GLint lightColorLoc = glGetUniformLocation(lightningShader.Program, "lightColor");
	glUniform3f(objectColorLoc, 0.f, 0.2f, 0.9f);
	glUniform3f(lightColorLoc, 1.f, 0.5f, 1.f); // Set color white	

	// Create camera transformation
	glm::mat4 view;
	view = camera.GetViewMatrix();
	// Projection matrix.
	glm::mat4 projection;
	projection = glm::perspective(camera.Zoom, (float)WIDTH/(float)HEIGHT , .1f, 100.f);

	// Create uniforms for model, view and proj matrices
	GLint modelLoc = glGetUniformLocation(lightningShader.Program, "model");
	GLint viewLoc = glGetUniformLocation(lightningShader.Program, "view");
	GLint projLoc = glGetUniformLocation(lightningShader.Program, "projection");
	
	// Time from moment glfw initialized.
	currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
    
	// Draw container (using box vertex attribs)
		glBindVertexArray(boxVAO);
		glm::mat4 model;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

	// Draw the lamp obj, again binding the appropriate shader
		lampShader.Use();
		// Get location obj for the matrices on the lamp shader
		// (these could be different on different shaders)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");
	
		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4();
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // Make  a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	// Draw the light obj (using light's vert attributes)
		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		// Swap the screen buffers
        glfwSwapBuffers(window);
    }

    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            keys[key] = true;
        else if (action == GLFW_RELEASE)
            keys[key] = false;
    }
}

void do_movement()
{
    if (keys[GLFW_KEY_W])
		camera.ProcessKeyboard(FORWARD, deltaTime);
    if (keys[GLFW_KEY_S])
		camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (keys[GLFW_KEY_A])
		camera.ProcessKeyboard(LEFT, deltaTime);
    if (keys[GLFW_KEY_D])		
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (keys[GLFW_KEY_SPACE] || keys[GLFW_KEY_E])
		camera.ProcessKeyboard(UP, deltaTime);
	if (keys[GLFW_KEY_LEFT_SHIFT] || keys[GLFW_KEY_Q])
		camera.ProcessKeyboard(DOWN, deltaTime);
}

void mouse_callback(GLFWwindow * window, double xpos, double ypos)
{
	// Avoid cam jumps by checking whether the mouse cursor enters window for the first time
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
	
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);	
}