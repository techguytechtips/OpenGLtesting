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
		float rotX = sensitivity * (float)(mouseY - (winHeight / 2)) / winHeight;
		float rotY = sensitivity * (float)(mouseX - (winWidth / 2)) / winWidth;
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
