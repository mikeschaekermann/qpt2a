#version 110

uniform vec3 translation;

uniform float time;

varying vec4 vertexPositionWorld;
varying vec4 vertexPositionEye;
varying vec4 vertexNormalEye;

void main()
{
	vec4 vertexPositionModel = gl_Vertex;
	
	vertexPositionWorld = vertexPositionModel + vec4(translation, 1.);

	vec4 vertexMovement = vec4(cos((vertexPositionModel.x) * time) / 20.,
		cos((vertexPositionModel.y) * time) / 20.,
		cos((vertexPositionModel.z) * time) / 20.,
		0.);

	vertexPositionEye = gl_ModelViewMatrix * (gl_Vertex + vertexMovement);
	vertexNormalEye = normalize(vec4(gl_NormalMatrix * gl_Normal, 0.));

	gl_Position = gl_ProjectionMatrix * vertexPositionEye;
}