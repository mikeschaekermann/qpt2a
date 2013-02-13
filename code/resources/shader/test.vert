#version 110

varying vec4 vertexPositionEye;
varying vec4 vertexNormalEye;

void main()
{
	vertexPositionEye = gl_ModelViewMatrix * gl_Vertex;       
	vertexNormalEye = normalize(vec4(gl_NormalMatrix * gl_Normal, 0.));

	gl_Position = ftransform();
}
