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
