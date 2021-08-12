void Buffer(Buffers *buffers, float* verts, int* indices){

	// create the Vertex Array, Vertex Array Array (Vertex Buffer), and Index/Element Buffer Object
	glGenVertexArrays(1, &buffers->VAO);
	glGenBuffers(1, &buffers->VBO);
	glGenBuffers(1, &buffers->IBO);

	// bind VAO as the Vertex Array and bind the Vertex Array to the Vertex Buffer
	glBindVertexArray(buffers->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, buffers->VBO);
	
	// fill the Vertex Array Object full of the Vertices
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts) * sizeof(float), verts, GL_STATIC_DRAW);
	
	// bind IBO as Element Array Object and fill it with the indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers->IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices) * sizeof(unsigned int), indices, GL_STATIC_DRAW);
}
