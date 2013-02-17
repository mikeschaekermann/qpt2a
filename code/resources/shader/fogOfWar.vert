#version 110

varying vec2 positionNormalized;

void main() {
	positionNormalized = vec2(gl_Vertex.x, - gl_Vertex.y);
	gl_Position = gl_Vertex;
}