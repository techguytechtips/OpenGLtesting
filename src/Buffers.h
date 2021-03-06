// structure to hold shader id's
typedef struct {
	GLuint VAO, VBO, IBO;
} Buffers;
Buffers Buffer(GLfloat* verts, GLuint* indices, size_t sizeOfVerts, size_t sizeOfIndices){
	// generate the opengl buffers
	Buffers buffers;
	glGenVertexArrays(1, &buffers.VAO);
	glGenBuffers(1, &buffers.VBO);
	glGenBuffers(1, &buffers.IBO);

	// bind VAO as the Vertex Array and bind the Vertex Array to the Vertex Buffer
	glBindVertexArray(buffers.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, buffers.VBO);

	// bind IBO as Element Array Object and fill it with the indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers.IBO);

	// fill the Vertex Array Object full of the Vertices
	// and the element buffer full of elements
	glBufferData(GL_ARRAY_BUFFER, sizeOfVerts * sizeof(GLfloat), verts, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeOfIndices * sizeof(unsigned int), indices, GL_STATIC_DRAW);
	return buffers;


}
void UnbindAll(){
	// unbind the GL Array and Element Buffer
	// (this is uneeded but is good practice for bigger programs
	// that might change the buffers after they have been bound
	// which will cause problems)
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
