#version 110

uniform vec3 translation;

uniform float time;

varying vec4 vertexPositionModel;
varying vec4 vertexPositionEye;
varying vec4 vertexNormalEye;

void main()
{
	vertexPositionModel = gl_Vertex;
	
	float t = time;

	vec4 vertexMovement = vec4(cos((vertexPositionModel.x) * t) / 20.,
		cos((vertexPositionModel.y) * t) / 20.,
		cos((vertexPositionModel.z) * t) / 20.,
		0.);

	vertexPositionEye = gl_ModelViewMatrix * (gl_Vertex + vertexMovement);
	vertexNormalEye = normalize(vec4(gl_NormalMatrix * gl_Normal, 0.));

	gl_Position = gl_ProjectionMatrix * vertexPositionEye;
}