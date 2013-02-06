#include "ModelBufferData.h"

ModelBufferData::ModelBufferData() {}

ModelBufferData::ModelBufferData(GLuint normalBufferObject, GLuint vertexBufferObject, 
								 GLuint indexBufferObject, unsigned indicesCounter) :
	normalBufferObject(normalBufferObject),
	vertexBufferObject(vertexBufferObject),
	indexBufferObject(indexBufferObject),
	indicesCounter(indicesCounter)
{
}

ModelBufferData::~ModelBufferData(void)
{
	glDeleteBuffers(1, &indexBufferObject);
	glDeleteBuffers(1, &vertexBufferObject);
	glDeleteBuffers(1, &normalBufferObject);
}
