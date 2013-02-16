#version 110

varying vec2 positionNormalized;

void main() {
	positionNormalized = vec2(gl_Vertex);
	gl_Position = gl_Vertex;
}