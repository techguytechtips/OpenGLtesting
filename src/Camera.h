void UpdateCamera(mat4* cameraMatrix, vec3 *position, vec3 orientation, float FOVdeg, unsigned int winWidth, unsigned int winHeight, float viewMin, float viewMax, vec3 up){ 
		// create the 4x4 identity matrices
		mat4 view;
		mat4 proj;
		glm_mat4_identity(view);
		glm_mat4_identity(proj);

		// create a vec3 for the position plus the orientation
		vec3 popluso;
		glm_vec3_add(*position, orientation, popluso);
		glm_lookat(*position, popluso, up, view);


		// set the FOV, how much can be on the screen, and the view distance
		glm_perspective(glm_rad(FOVdeg), (float)(winWidth / winHeight), viewMin, viewMax, proj);

		glm_mat4_mul(proj, view, *cameraMatrix);	
}
void CameraUniform(mat4 cameraMatrix, const char* uniform, GLuint program){
	glUniformMatrix4fv(glGetUniformLocation(program, uniform), 1, GL_FALSE, *cameraMatrix);
}
