#include "gltest.h"
int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit()) return -1;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLfloat verts[] = {
		// verts for pyramid
		-0.5f, 0.0f,  0.5f, 	0.0f, 0.0f,
		-0.5f, 0.0f, -0.5f, 	5.0f, 0.0f,
		 0.5f, 0.0f, -0.5f, 	0.0f, 0.0f,
		 0.5f, 0.0f,  0.5f, 	5.0f, 0.0f,
		 0.0f, 0.8f,  0.0f,	2.5f, 5.0f
	};


	GLuint indices[] = {
		// indices for pyramid
		0, 1, 2,
		0, 2, 3,
		0, 1, 4,
		1, 2, 4,
		2, 3, 4,
		3, 0, 4
	};

	/* Create a windowed mode window and its OpenGL context */
	unsigned int winWidth = 1920;
	unsigned int winHeight = 1080;
	window = glfwCreateWindow(winWidth, winHeight, "Day 5", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glewInit();
	
	// create and compile the shaders
	GLuint shaderProgram = CreateProgram("src/shaders/vertex.shader", "src/shaders/fragment.shader");
	//if (!shaders.vertexShader || !shaders.fragmentShader){
	//	printf("Shaders failed!\n");
	//	return -1;
	//}
	


	// create the OpenGL buffers
	Buffers buffers = Buffer(verts, indices, sizeof(verts), sizeof(indices));

	// create the Vertex Attribute Pointer to point to the vertex data, it's arguments are
	// the starting offset of the vertex array, how many dimensions (x, y, z), the data type, wether or not its a int
	// and how big each vertex is (in this case it's 2 floats per vertex (x, y) * the size of a float)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	
	// unbind the GL Array and Element Buffer
	// (this is uneeded but is good practice for bigger programs
	// that might change the buffers after they have been bound
	// which will cause problems)
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
	// create texture
	GLuint texture = LoadTexture2D("src/textures/brick.png", GL_RGBA ,GL_TEXTURE0, GL_NEAREST, GL_NEAREST, GL_REPEAT, GL_REPEAT);

	// uniforms
	GLuint u_tex = glGetUniformLocation(shaderProgram, "tex0");
	GLuint u_cam = glGetUniformLocation(shaderProgram, "camMatrix");	
		
	// use the shader program we created
	// if we were to use multiple shader programs,
	// we whould need to put this in the main loop
	// and switch between them, same thing with 
	// textures
	glUseProgram(shaderProgram);

	// bind the texture
	glBindTexture(GL_TEXTURE_2D, texture);

	// tell shader what texture slot we want to use
	double prevTime = glfwGetTime();
	glUniform1i(u_tex, 0);
	unsigned char firstClick = 1;
	float speed = 0.1f;
	float sensitivity = 100.0f;
	vec3 position = {0.0f, 0.0f, 2.0f};
	vec3 orientation = {0.0f, 0.0f, -1.0f};
	vec3 up = {0.0f, 1.0f, 0.0f};
	vec3 down = {0.0f, -1.0f, 0.0f};

	// enable depth testing so opengl
	// knows what to draw on top
	glEnable(GL_DEPTH_TEST);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		// clear the color buffer by adding RGBA values to it
		//glClearColor(0.4f, 0.3f, 0.7f, 1.0f);
		glClearColor(0.2f, 0.4f, 0.5f, 1.0f);
		// clear the color and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
		while(glfwGetTime() < prevTime + 1.0/60){}
		prevTime += 1.0/60;


		// get user input
		GetInput(window, winWidth, winHeight, &orientation, position, up, down, speed, sensitivity, &firstClick);
		// apply to camera
		Camera(&position, orientation, u_cam, 45.0f, winWidth, winHeight, 0.1f, 100.0f, up);
		

		// Bind the Vertex Array Object to VAO
		glBindVertexArray(buffers.VAO);
		// draw the vertices in the order in the IBO and fill in the triangles
		glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, 0);


		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();


	}

	// the program is closing, so delete the buffers, texture(s), and shader program(s) and destory the window
	glDeleteVertexArrays(1, &buffers.VAO);
	glDeleteBuffers(1, &buffers.VBO);
	glDeleteBuffers(1, &buffers.IBO);
	glDeleteTextures(1, &texture);
	glDeleteProgram(shaderProgram);
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
