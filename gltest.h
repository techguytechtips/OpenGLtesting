// structure to hold shader id's
typedef struct {
	GLuint VBO, VAO, IBO;
} Buffers;
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
// function to read a a plain text file into memory
int ReadFile(FILE* fp, int length, char* shader){
	if (fp){
	       	if (fread(shader, 1, length, fp) > 0) return 1;
		else return -1;
	}
	else return -1;

}
// function to read length of a plain text file
int ReadLength(FILE* fp){
	if (fp){
		fseek (fp, 0, SEEK_END);
		int length = ftell (fp);
		fseek (fp, 0, SEEK_SET);
		return length;
	} else return -1;
}
// function to create and compile the shaders
Shaders CreateShader(){
	Shaders shaders;

	// read shaders from file
	FILE * vertexfp = fopen("shaders/vertex.shader", "r");
	FILE * fragmentfp = fopen("shaders/fragment.shader", "r");
	if (!vertexfp || !fragmentfp){
		shaders.vertexShader = 0;
		shaders.fragmentShader = 0;
		return shaders;	
	}
		int vertexLength = ReadLength(vertexfp);
		int fragmentLength =  ReadLength(fragmentfp);
		char* vertexShaderSource = malloc(vertexLength);
	       	char* fragmentShaderSource= malloc(fragmentLength);
		
		if (ReadFile(vertexfp, vertexLength, vertexShaderSource) < 0 ||
		ReadFile(fragmentfp, fragmentLength, fragmentShaderSource)< 0){
			shaders.vertexShader = 0;
			shaders.fragmentShader = 0;
			return shaders;
		}
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
	return shaders;
}

// function to load in a 2D texture
GLuint LoadTexture2D(const char* filename, GLuint colorFormat, GLuint slot, GLuint minFilter, GLuint magFilter, GLuint wrapX, GLuint wrapY){

	// texture
	stbi_set_flip_vertically_on_load(1);
	int widthImg, heightImg, numColch;
	unsigned char* imgBytes = stbi_load(filename, &widthImg, &heightImg, &numColch, 0);
	GLuint texture;
	glGenTextures(1, &texture);
	glActiveTexture(slot);
	glBindTexture(GL_TEXTURE_2D, texture);

	// a min filter is when the image is small/far away that multiple of the images pixels
	// take up a single display pixel
	// a mag filter is when the image is so big/close that multiple of the displays pixels
	// make up one image pixel
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapX);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapY);

	glTexImage2D(GL_TEXTURE_2D, 0, colorFormat, widthImg, heightImg, 0, colorFormat, GL_UNSIGNED_BYTE, imgBytes);

	// generates smaller sized versions of the texture
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(imgBytes);
	glBindTexture(GL_TEXTURE_2D, 0);
	return texture;
}
void GetInput(GLFWwindow* window, unsigned int winWidth, unsigned int winHeight, vec3 *orientation, vec3 position, vec3 up, vec3 down, float speed, float sensitivity, unsigned char *firstClick){
	vec3 leftRightLookCrossn;
	vec3 leftRightCrossn;
	if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		glm_vec3_muladds(*orientation, speed, position);

	if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
		glm_vec3_crossn(*orientation, up, leftRightCrossn);
		glm_vec3_muladds(leftRightCrossn, -speed, position);
	}
	if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		glm_vec3_muladds(*orientation, -speed, position);

	if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		glm_vec3_crossn(*orientation, up, leftRightCrossn);
		glm_vec3_muladds(leftRightCrossn, speed, position);
	}
	if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		glm_vec3_muladds(up, speed, position);

	if(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		glm_vec3_muladds(up, -speed, position);

	if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		speed = 0.4f;

	else if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
		speed = 0.1f;

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS){
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		if (*firstClick){
			glfwSetCursorPos(window, (winWidth / 2), (winHeight / 2));
			*firstClick = 0;
		}
		
		double mouseX, mouseY;	
		glfwGetCursorPos(window, &mouseX, &mouseY);	
		float rotX = sensitivity * (float)(mouseY - (winWidth / 2)) / winWidth;
		float rotY = sensitivity * (float)(mouseX - (winHeight / 2)) / winHeight;
		vec3 newOrientation;
		glm_vec3_crossn(*orientation, up, leftRightLookCrossn);
		glm_vec3_copy(*orientation, newOrientation);
		glm_vec3_rotate(newOrientation, glm_rad(-rotX), leftRightLookCrossn);
		if (!((glm_vec3_angle(newOrientation, up) <= glm_rad(5.0f)) || (glm_vec3_angle(newOrientation, down) <= glm_rad(5.0f))))
			glm_vec3_copy(newOrientation, *orientation);

		glm_vec3_rotate(*orientation, glm_rad(-rotY), up);
		glfwSetCursorPos(window, (winWidth / 2), (winHeight / 2));
	}
	else if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE){
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		*firstClick = 1;
	}


}
