#version 330

uniform mat4 matModelView;
uniform mat4 matProjection;

out vec4 vertexPositionEye;
out vec4 vertexNormalEye;

void main(void) {
	vertexPositionEye = matModelView * vec4(gl_Position.xyz, 1.);
	vertexNormalEye = matModelView * vec4(gl_Normal.xyz, 0);

	gl_Position = matProjection * vertexPositionEye;
}