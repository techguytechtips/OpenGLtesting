#include "gltest.h"
int main(void)
{

	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit()) return -1;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// vertices
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
	GLfloat cubeVerts[] = {
	-0.1f, -0.1f, 0.1f,		
	-0.1f, -0.1f, -0.1f,
	 0.1f, -0.1f, -0.1f,
	 0.1f, -0.1f, 0.1f,
	-0.1f,  0.1f, 0.1f,
	-0.1f,  0.1f, -0.1f,
	 0.1f,  0.1f, -0.1f,
	 0.1f,  0.1f, 0.1f,
	};
	GLuint cubeIndices[] = {
		0, 1, 2,
		0, 2, 3,
		0, 4, 7,
		0, 7, 3,
		3, 7, 6,
		3, 6, 2,
		2, 6, 5,
		2, 5, 1,
		1, 5, 4,
		1, 4, 0,
		4, 5, 6,
		4, 6, 7,
	};
	/* Create a windowed mode window and its OpenGL context */
	unsigned int winWidth = 600;
	unsigned int winHeight = 600;
	window = glfwCreateWindow(winWidth, winHeight, "Day 6", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glewInit();
	
	// create and compile the shaders
	GLuint pyramidProgram = CreateProgram("src/shaders/vertex.shader", "src/shaders/fragment.shader");
	if (pyramidProgram == 0){
		printf("Shaders Failed!\n");
		return -1;
	}

	// create the OpenGL buffers
	Buffers buffers = Buffer(verts, indices, sizeof(verts), sizeof(indices));

	// tell the vertex shader the diffrent attributes of the vertices for the pyramid
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	UnbindAll();

	// tell the vertex shader the diffrent attributes of the vertices for the cube light
	GLuint cubeProgram = CreateProgram("src/shaders/cubevertex.shader", "src/shaders/cubefragment.shader");
	Buffers cubeBuffers = Buffer(cubeVerts, cubeIndices, sizeof(cubeVerts), sizeof(cubeIndices));
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	UnbindAll();	


	// setup the model matrix and position vector for the pyramid
	vec3 pyramidPos = {0.0f, 0.0f, 0.0f};
	mat4 pyramidModel;
	glm_mat4_identity(pyramidModel);
	glm_translate(pyramidModel, pyramidPos);


	// setup the model matrix and position vector for the cube light
	vec4 lightColor = {0.6f, 0.4f, 0.5f, 1.0f};
	vec3 cubePos = {0.5f, 0.5f, 0.5f};
	mat4 cubeModel;
	glm_mat4_identity(cubeModel);
	glm_translate(cubeModel, cubePos);

	// give the uniforms to the pyramid vertex shader
	glUseProgram(pyramidProgram);
	glUniformMatrix4fv(glGetUniformLocation(pyramidProgram, "model"), 1, GL_FALSE, *pyramidModel);
	glUniform4fv(glGetUniformLocation(pyramidProgram, "lightColor"), 1, lightColor);
	
	// give the uniforms to the cube vertex shader
	glUseProgram(cubeProgram);
	glUniformMatrix4fv(glGetUniformLocation(cubeProgram, "model"), 1, GL_FALSE, *cubeModel);
	glUniform4fv(glGetUniformLocation(cubeProgram, "lightColor"), 1, lightColor);


	// create texture
	GLuint texture = LoadTexture2D("src/textures/brick.png", GL_RGBA ,GL_TEXTURE0, GL_NEAREST, GL_NEAREST, GL_REPEAT, GL_REPEAT);

		
	// tell pyramid shader what texture slot we want to use
	glUniform1i(glGetUniformLocation(pyramidProgram, "tex0"), 0);
	
	// setup camera matrix
	mat4 cameraMatrix;
	glm_mat4_identity(cameraMatrix);
	
	// timer for FPS limiter
	double prevTime = glfwGetTime();
	
	// variable for input to check if its the first time
	// you have clicked on the window to make sure
	// it doesn't jump on the first click
	unsigned char firstClick = 1;
	
	// camera speed and sensitivity
	float speed = 0.1f;
	float sensitivity = 100.0f;
	
	// position and orientation for camera	
	vec3 position = {0.0f, 0.0f, 2.0f};
	vec3 orientation = {0.0f, 0.0f, -1.0f};

	// up and down reference vectors for camera
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
		glClearColor(0.2f, 0.4f, 0.5f, 1.0f);

		// clear the color and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// fps limiter	
		while(glfwGetTime() < prevTime + 1.0/60){}
		prevTime += 1.0/60;


		// get user input
		GetInput(window, winWidth, winHeight, &orientation, position, up, down, speed, sensitivity, &firstClick);

		// apply to camera
		UpdateCamera(&cameraMatrix, &position, orientation, 45.0f, winWidth, winHeight, 0.1f, 100.0f, up);	

		// give camera matrix to shader
		glUseProgram(pyramidProgram);
		CameraUniform(cameraMatrix, "camMatrix", pyramidProgram);

		// bind the texture
		glBindTexture(GL_TEXTURE_2D, texture);

		// Bind the pyramid Vertex Array Object to VAO
		glBindVertexArray(buffers.VAO);
			
		// draw the pyramid
		glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, 0);
		
		// switch to the cube shader program
		glUseProgram(cubeProgram);
	
		// give the camera matrix to the cube shader program
		CameraUniform(cameraMatrix, "camMatrix", cubeProgram);
		
		// bind the cube vertex array
		glBindVertexArray(cubeBuffers.VAO);

		// draw the cube
		glDrawElements(GL_TRIANGLES, sizeof(cubeIndices)/sizeof(int), GL_UNSIGNED_INT, 0);


		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();


	}

	// the program is closing, so delete the buffers, texture(s), and shader program(s) and destory the window
	glDeleteVertexArrays(1, &buffers.VAO);
	glDeleteBuffers(1, &buffers.VBO);
	glDeleteBuffers(1, &buffers.IBO);
	glDeleteBuffers(1, &cubeBuffers.VAO);
	glDeleteBuffers(1, &cubeBuffers.VBO);
	glDeleteBuffers(1, &cubeBuffers.IBO);
	glDeleteTextures(1, &texture);
	glDeleteProgram(pyramidProgram);
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
