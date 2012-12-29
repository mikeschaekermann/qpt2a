#pragma once

#include "cinder/gl/gl.h"

class ModelBufferData
{
public:
	ModelBufferData(GLuint normalBufferObject, GLuint vertexBufferObject, 
					GLuint indexBufferObject, unsigned indicesCounter);
	~ModelBufferData(void);

	const GLuint& getNormalBufferObject() const { return normalBufferObject; }
	const GLuint& getVertexBufferObject() const { return vertexBufferObject; }
	const GLuint& getIndexBufferObject() const { return indexBufferObject; }
	const unsigned& getIndicesCounter() const { return indicesCounter; }

private:
	GLuint normalBufferObject;
	GLuint vertexBufferObject;
	GLuint indexBufferObject;
	unsigned indicesCounter;
};