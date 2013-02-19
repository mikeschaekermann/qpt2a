#version 110

uniform float time;

varying vec4 vertexPositionModel;
varying vec4 vertexPositionEye;
varying vec4 vertexNormalEye;

void main()
{
	vertexPositionModel = gl_Vertex;
	vertexPositionEye = gl_ModelViewMatrix * gl_Vertex;      
	vertexNormalEye = normalize(vec4(gl_NormalMatrix * gl_Normal, 0.));

	gl_Position = gl_ProjectionMatrix * vertexPositionEye;
}
