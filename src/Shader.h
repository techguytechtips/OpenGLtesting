typedef struct {
	GLuint vertexShader, fragmentShader;
} Shaders;
// function to check if the shader has compiled successfully
int CheckShader(GLuint Shader, char* message){
	int result;
	glGetShaderiv(Shader, GL_COMPILE_STATUS, &result);
	if(!result) {
		int length;
		glGetShaderiv(Shader, GL_INFO_LOG_LENGTH, &length);
		glGetShaderInfoLog(Shader, length, &length, message);
		return -1;
	}
	return 1;
}
// function to create and compile the shaders
GLuint CreateProgram(const char* vertexPath, const char* fragmentPath){
	Shaders shaders;
	GLuint shaderProgram;
	// read shaders from file
	FILE * vertexfp = fopen(vertexPath, "r");
	FILE * fragmentfp = fopen(fragmentPath, "r");
	if (!vertexfp || !fragmentfp){
		return 0;
	}
		int vertexLength = ReadLength(vertexfp);
		int fragmentLength =  ReadLength(fragmentfp);
		char* vertexShaderSource = malloc(vertexLength + 1);
	       	char* fragmentShaderSource = malloc(fragmentLength + 1);
		if (ReadFile(vertexfp, vertexLength, vertexShaderSource) < 0 ||
		ReadFile(fragmentfp, fragmentLength, fragmentShaderSource) < 0){
			return 0;
		}
		fragmentShaderSource[fragmentLength] = '\0';
		vertexShaderSource[vertexLength] = '\0';
		fclose(vertexfp);
		fclose(fragmentfp);

	// create shaders
	shaders.vertexShader = glCreateShader(GL_VERTEX_SHADER);
	shaders.fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// tell OpenGL where shaders are
	glShaderSource(shaders.vertexShader, 1, &vertexShaderSource, NULL);
	glShaderSource(shaders.fragmentShader, 1, &fragmentShaderSource, NULL);

	// compile shaders
	glCompileShader(shaders.vertexShader);
	glCompileShader(shaders.fragmentShader);

	// check to make sure the shaders compiled successfully
	char* shaderStatus = malloc(1000);
	if(CheckShader(shaders.vertexShader, shaderStatus) < 1) printf("Vertex Shader Compilation status: %s\n", shaderStatus);
	if(CheckShader(shaders.fragmentShader, shaderStatus) < 1) printf("Fragment Shader Compilation status: %s\n", shaderStatus);
	free(shaderStatus);
	free(vertexShaderSource);
	free(fragmentShaderSource);

	// create OpenGL GPU program
	// out of Vertex and Fragment shader
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, shaders.vertexShader);
	glAttachShader(shaderProgram, shaders.fragmentShader);

	// send her to the GPU
	glLinkProgram(shaderProgram);

	// delete the now useless shaders	
	glDeleteShader(shaders.vertexShader);
	glDeleteShader(shaders.fragmentShader);
	return shaderProgram;

}
